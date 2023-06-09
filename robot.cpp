#include "include/includeAll.h"

Robot::Robot(int id, double x, double y) : id(id), pos(x, y), actionModel(this), PID(this), taskChain()
{
    productType = 0;
    pidPool = new ObjectPool<PIDModel>(100);
}

void Robot::setDij(int **accessMap)
{
    partDijk = new PartDijk(accessMap);
    this->accessMap = accessMap;
}

Robot::~Robot()
{
    if (partDijk != nullptr)
    {
        delete partDijk;
    }
}

Vec *Robot::predict(std::list<Vec *> *paths)
{
    clearStates();

    // 迭代器
    std::list<Vec *>::iterator currIdx = paths->begin();

    // 状态拷贝
    MotionState *state = getMotionState();
    state->update(this);
    PIDModel *pidModel = pidPool->acquire();
    pidModel->set(PID);

    motionStates[frameId] = state;

    int nextFrameId = frameId + 1;
    int predictFrameLength = PRED_TIME;
    Workbench *wb = getProductType() == 0 ? getTask()->getFrom() : getTask()->getTo();

    for (int i = 0; i < predictFrameLength; i++)
    {
        if (currIdx == paths->end() || isArrive(wb->getPos(), *currIdx, 0.2))
        {
            // while (i < predictFrameLength)
            // {
            //     MotionState *newState = getMotionState();
            //     newState->set(*state);
            //     motionStates[nextFrameId] = newState;
            //     i++;
            //     nextFrameId++;
            // }
            break;
        }
        double targetVelocity, targetAngularVelocity;
        // pidModel->control(state, nextPos, targetVelocity, targetAngularVelocity);
        pidModel->control(state, *currIdx, targetVelocity, targetAngularVelocity);
        state = actionModel.predict(state, targetVelocity, targetAngularVelocity);
        motionStates[nextFrameId] = state;

        bool isCollided = false;
        MotionState *otherState = nullptr;
        for (Robot *rb : *robotList)
        {
            if (rb == this)
            {
                continue;
            }

            // 获取同一时刻其他机器人状态
            if (rb->motionStates.find(nextFrameId) == rb->motionStates.end())
            {
                continue;
            }
            otherState = rb->motionStates[nextFrameId];

            // 检测是否碰撞， TODO: 碰撞距离
            if (computeDist(state->getPos(), otherState->getPos()) < 1.5)
            {
                isCollided = true;
                break;
            }
        }

        if (isCollided)
        {
            // 传输碰撞位置，进行检测
            Vec *nextPos = findNext(state);

            if (nextPos == nullptr)
            {
                break;
            }

            // TODO: objectpool防止重复释放
            pidPool->release(pidModel);
            return nextPos;
        }

        // 检测当前点位是否到达
        while (currIdx != paths->end() && isArrive(*currIdx, state->getPos(), 0.2))
        {
            currIdx++;
        }
        nextFrameId++;
    }
    pidPool->release(pidModel);
    Vec *next = pools.getVec();
    next->set(paths->front());
    return next;
}

// 根据地图更改PID？？？
void Robot::updatePid(int count)
{
    // PID.set(5.5, 0.07, 1.3, 5.5, 0.07, 1.3);
}

bool Robot::isLoaded()
{
    return productType != 0;
}

bool Robot::isFree()
{
    return task == nullptr;
}

void Robot::control(MotionState *ms, Vec *pos, double &v, double &w)
{
    PID.control(ms, pos, v, w);
}

void Robot::update(int frameid)
{

    double x, y, vx, vy;
    // 读取信息
    lastProductType = productType;
    scanf("%d %d %lf %lf %lf %lf %lf %lf %lf %lf",
          &workbenchIdx, &productType, &timeCoefficients, &collisionCoefficients,
          &w, &vx, &vy, &heading, &x, &y);
    getchar();

    // 更新
    this->frameId = frameid;
    if (((pos.getX() - x) * (pos.getX() - x) + (pos.getY() - y) * (pos.getY() - y)) < 0.01 && productType != 0)
    {
        count += 1;
    }
    pos.set(x, y);
    velocity.set(vx, vy);
}

size_t Robot::hash() const
{
    return id;
}
// TODO:添加leftframe优化
void Robot::checkDeal()
{
    if (task == nullptr)
    {
        return;
    }

    Workbench *from = task->getFrom();
    Workbench *to = task->getTo();

    // 买成功，不持有->持有
    // 生产工作台规划产品格被释放:from.setPlanProductStatus(0);
    // 如果这时规划原料格已满，那么清除原料格状态
    if (lastProductType == 0 && productType != 0)
    {
        // 放开原料购买控制台
        from->setPlanProductStatus(0);
        // 弹出from地址
        actionModel.popPath();
    }

    // 卖成功，持有->不持有
    // 如果系统返回的原料格信息为0，那么清空规划原料格信息
    if (lastProductType != 0 && productType == 0)
    {
        actionModel.popPath();
        to->updatePlanMaterialStatus(from->getType(), true);
        taskChain.removeTask(0);
        task = taskChain.getNextTask();
    }

}

Vec Robot::getPos() const
{
    return pos;
}

Vec Robot::getVelocity() const
{
    return velocity;
}

// 优先级排序
// double Robot::getPriority()
// {
//     if (task == nullptr)
//     {
//         return 10000.;
//     }

//     return -taskChain.getProfit();
// }

double Robot::getPriority()
{
    if (task == nullptr)
    {
        return 10000.;
    }

    if (productType != 0) {
        return -productType;
    }

    Workbench *wb = productType == 0 ? task->getFrom() : task->getTo();

    double dist = wb->getDij()->getDistMap(isLoaded())[getMapRow()][getMapCol()];

    return dist;
}


bool Robot::operator<(Robot *o)
{
    return this->getPriority() < o->getPriority();
}

bool Robot::operator==(const Robot &o) const
{
    return this->id == o.id;
}

Task *Robot::getTask()
{
    return task;
}

int Robot::getProductType() const
{
    return productType;
}

int Robot::getLastProductType() const
{
    return lastProductType;
}
int Robot::getId() const
{
    return id;
}
int Robot::getWorkbenchIdx() const
{
    return workbenchIdx;
}
void Robot::addAction(Action *action)
{
    actions.push_back(action);
}

void Robot::bindChain(TaskChain *taskChain)
{
    this->taskChain.set(taskChain);
    this->task = taskChain->getTask(0);
    // 构建路径
    // rb->from 路径
    Workbench *from = task->getFrom();
    double distance = from->getDij()->getDistMap(false)[getMapRow()][getMapCol()];
    int nr = getMapRow();
    int nc = getMapCol();
    while (distance > 999999.)
    {
        for (int i = 0; i < 8; i++)
        {
            int nr = getMapRow() + unloadDir[i][0];
            int nc = getMapCol() + unloadDir[i][1];
            distance = from->getDij()->getDistMap(false)[nr][nc];
        }
    }
    std::list<Vec *> *result = from->getDij()->getKnee(nr, nc, false, false);
    // 在目标周围就不添加路径点
    for (auto p : *result)
    {
        if (computeDist(from->getPos(), p) < 0.25)
        {
            pools.release(p);
            continue;
        }
        Vec *next = pools.getVec();
        next->set(p);
        addPathPoint(next);
    }

    delete result;
    Vec *next = pools.getVec();
    next->set(from->getPos());
    addPathPoint(next);
    // 加入任务路径
    for (auto t : taskChain->getTaskChain())
    {
        for (auto p : *(t->getRoad()))
        {
            if (computeDist(from->getPos(), p) < 0.25)
            {
                continue;
            }
            next = pools.getVec();
            next->set(p);
            addPathPoint(next);
        }
    }
}

void Robot::step()
{
    // 清空动作列表
    actions.clear();
    // 更新action列表
    actionModel.generate();
}

std::vector<Action *> Robot::getActions() const
{
    return actions;
}

double Robot::getHeading() const
{
    return heading;
}

double Robot::getW() const
{
    return w;
}

/* 获取工作台在dijkstra算法获得的图中的坐标 */
int Robot::getMapRow()
{
    return ((int)((49.75 - pos.getY()) / 0.5)) * 2 + 1;
}

int Robot::getMapCol()
{
    return ((int)((pos.getX() - 0.25) / 0.5)) * 2 + 1;
}
void Robot::addPathPoint(Vec *point)
{
    actionModel.addPathPoint(point);
}

void Robot::setAccessMap(int **accessMap)
{
    actionModel.setAccessMap(accessMap);
    // 根据map初始化局部搜索
    setDij(accessMap);
}
MotionState *Robot::getMotionState()
{
    return actionModel.getMotionState();
}
void Robot::releaseMotionState(MotionState *ms)
{
    actionModel.releaseMotionState(ms);
}
void Robot::clearStates()
{
    // 遍历motionStates的值
    for (auto &p : motionStates)
    {
        // 释放motionState
        actionModel.releaseMotionState(p.second);
    }
    // 清空motionStates
    motionStates.clear();
}

void Robot::setRobotList(std::vector<Robot *> *robotList)
{
    this->robotList = robotList;
}

Vec *Robot::findNext(MotionState *crash)
{
    Vec *result = nullptr;
    // 将x,y坐标转换为地图坐标
    int crashR = ((int)((49.75 - crash->getPos()->getY()) / 0.5)) * 2 + 1;
    int crashC = ((int)((crash->getPos()->getX() - 0.25) / 0.5)) * 2 + 1;

    Workbench *wb = productType == 0 ? task->getFrom() : task->getTo();

    // 
    int nr = getMapRow();
    int nc = getMapCol();

    double crashDist = fabs(wb->getDij()->getDistMap(isLoaded())[crashR][crashC] - wb->getDij()->getDistMap(isLoaded())[nr][nc]);
    if (crashDist > 9999)
    {
        crashDist = computeDist(crash->getPos(), &pos);
    }
    else
    {
        crashDist *= 0.25;
    }
    std::vector<std::pair<int, int> *> *node2BeSearched = partDijk->search(nr, nc, isLoaded(), crashDist, crashR, crashC);

    std::deque<std::pair<int, int> *> *node2BeSearched = partDijk->search(getMapRow(), getMapCol(), isLoaded(), crashDist, crashR, crashC);

    // 进行稀疏化，去除比较近的点，从后向前遍历
    for (int i = node2BeSearched->size() - 1; i > 0; i--)
    {
        auto p = node2BeSearched->at(i);
        auto q = node2BeSearched->at(i - 1);
        // 如果两个点的距离小于0.5，则删除后面的点
        if ((p->first - q->first) * (p->first - q->first) + (p->second - q->second) * (p->second - q->second) <= 1.5)
        {
            delete p;
            node2BeSearched->erase(node2BeSearched->begin() + i);
        }
    }
    //

    for (auto p : *node2BeSearched)
    {
        // 从当前位置到新的检测点的路径
        std::list<Vec *> *newPath = getPartDij()->getKnee(p->first, p->second, isLoaded());
        // 从新的中间点到目标工作台的路径
        std::list<Vec *> *nextPaths = wb->getDij()->getKnee(p->first, p->second, isLoaded(), false);

        // 拼接路径
        newPath->splice(newPath->end(), *nextPaths);
        delete nextPaths;

        // 进行预测
        clearStates();
        // 迭代器
        std::list<Vec *>::iterator currIdx = newPath->begin();

        // 状态拷贝
        MotionState *state = getMotionState();
        state->update(this);
        PIDModel *pidModel = pidPool->acquire();
        pidModel->set(PID);

        motionStates[frameId] = state;

        int nextFrameId = frameId + 1;
        int predictFrameLength = PRED_TIME;

        bool isCollided = false;
        for (int i = 0; i < predictFrameLength; i++)
        {
            if (currIdx == newPath->end() || isArrive(wb->getPos(), *currIdx, 0.2))
            {
                // while (i < predictFrameLength)
                // {
                //     MotionState *newState = getMotionState();
                //     newState->set(*state);
                //     motionStates[nextFrameId] = newState;
                //     i++;
                //     nextFrameId++;
                // }
                break;
            }
            double targetVelocity, targetAngularVelocity;
            pidModel->control(state, *currIdx, targetVelocity, targetAngularVelocity);
            state = actionModel.predict(state, targetVelocity, targetAngularVelocity);
            motionStates[nextFrameId] = state;

            MotionState *otherState = nullptr;
            for (Robot *rb : *robotList)
            {
                if (rb == this)
                {
                    continue;
                }

                // 获取同一时刻其他机器人状态
                if (rb->motionStates.find(nextFrameId) == rb->motionStates.end())
                {
                    continue;
                }
                otherState = rb->motionStates[nextFrameId];

                // 检测是否碰撞， TODO: 碰撞距离
                if (computeDist(state->getPos(), otherState->getPos()) < 1.5)
                {
                    isCollided = true;
                    break;
                }
            }

            // 碰撞直接跳过，查看下一个点
            if (isCollided)
            {
                break;
            }

            // 检测当前点位是否到达
            while (currIdx != newPath->end() && isArrive(*currIdx, state->getPos(), 0.2))
            {
                currIdx++;
            }
            nextFrameId++;
        }

        // 每一轮进行释放
        pidPool->release(pidModel);

        for (auto item : *newPath)
        {
            pools.release(item);
        }
        delete newPath;

        // 200内没有碰撞，可以作为新路径
        if (!isCollided)
        {
            result = rc2Coord(p->first, p->second, 0.25);

            break;
        }
    }

    // 释放所有搜索点
    for (auto p : *node2BeSearched)
    {
        delete p;
    }
    delete node2BeSearched;

    return result;
}

void Robot::releaseVec(Vec *v)
{
    pools.release(v);
}