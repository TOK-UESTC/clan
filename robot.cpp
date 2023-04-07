#include "include/includeAll.h"

Robot::Robot(int id, double x, double y) : id(id), pos(x, y), actionModel(this), PID(this), taskChain()
{
    productType = 0;
}

Robot::~Robot()
{
}

Vec *Robot::predict()
{
    if (task == nullptr)
    {
        return nullptr;
    }
    clearStates();
    Workbench *wb = productType == 0 ? task->getFrom() : task->getTo();
    MotionState *state = getMotionState();
    state->update(this);
    motionStates[frameId] = state;

    PIDModel *pidModel = pidPool->acquire();
    pidModel->set(PID);

    int nextFrameId = frameId + 1;
    int predictFrameLength = 200;

    for (int i = 0; i < predictFrameLength; i++)
    {
        double targetVelocity, targetAngularVelocity;
        pidModel->control(state, wb->getPos(), targetVelocity, targetAngularVelocity);
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
            otherState = rb->motionStates[nextFrameId];
            if (otherState == nullptr)
            {
                continue;
            }
            if (computeDist(state->getPos(), otherState->getPos()) < 1.5)
            {
                isCollided = true;
                break;
            }
        }

        if (isCollided)
        {
            pidPool->release(pidModel);
            return findMiddle(state);
        }
        nextFrameId++;
    }
    pidPool->release(pidModel);
    Vec *next = vecPool->acquire();
    next->set(wb->getPos());
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
double Robot::getPriority() const
{
    return 1.0;
}

bool Robot::operator<(const Robot &o) const
{
    return this->getPriority() < o.getPriority();
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
    std::list<Vec *> *result = from->getDij()->getKnee(getMapRow(), getMapCol(), false);

    for (auto p : *result)
    {
        if (computeDist(from->getPos(), p) < 0.25)
        {
            continue;
        }
        addPathPoint(new Vec(p->getX(), p->getY()));
        delete p;
    }
    delete result;
    addPathPoint(new Vec(from->getPos()->getX(), from->getPos()->getY()));
    // 加入任务路径
    for (auto t : taskChain->getTaskChain())
    {
        for (auto p : *(t->getRoad()))
        {
            if (computeDist(from->getPos(), p) < 0.25)
            {
                continue;
            }
            addPathPoint(new Vec(p->getX(), p->getY()));
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
Vec *Robot::findMiddle(MotionState *crash)
{
    int predictFrameLength = 200;
    double range = 1.5;
    bool isFindNextWaypoint = false;

    while (!isFindNextWaypoint)
    {
        MotionState *s = getMotionState();
        s->update(this);
        std::vector<Vec *> *nextWaypoints = new std::vector<Vec *>();
        searchNextWaypoints(s, crash, range, nextWaypoints);
        // 寻找完后迅速释放
        releaseMotionState(s);

        for (Vec *next : (*nextWaypoints))
        {
            clearStates();
            s = getMotionState();
            s->update(this);
            motionStates[frameId] = s;

            PIDModel *p = pidPool->acquire();
            p->set(PID);
            int searchNextFrameId = frameId + 1;
            for (int j = 0; j < predictFrameLength; j++)
            {
                double targetV, targetW;
                p->control(s, next, targetV, targetW);
                s = actionModel.predict(s, targetV, targetW);
                bool isSearchCollided = false;
                // 检测新点是否会碰撞，内部遍历
                for (Robot *r : (*robotList))
                {
                    if (r == this)
                    {
                        continue;
                    }
                    auto tempIterator = r->motionStates.find(searchNextFrameId);

                    if (tempIterator == r->motionStates.end())
                    {
                        continue;
                    }
                    else
                    {
                        MotionState *searchOtherState = (r->motionStates.find(searchNextFrameId))->second;
                        if (searchOtherState == nullptr)
                        {
                            std::cerr << "searchOtherState is nullptr" << std::endl;
                            continue;
                        }

                        if (computeDist(s->getPos(), searchOtherState->getPos()) < 1.5)
                        {
                            isSearchCollided = true;
                            break;
                        }
                    }
                }
                motionStates[searchNextFrameId] = s;
                searchNextFrameId++;

                if (isSearchCollided)
                {
                    break;
                }
                if (j == predictFrameLength - 1)
                {
                    pidPool->release(p);

                    for (Vec *points : (*nextWaypoints))
                    {
                        if (points != next)
                        {
                            vecPool->release(points);
                        }
                    }
                    return next;
                }
            }
            // 每一轮进行释放
            pidPool->release(p);
        }

        for (Vec *next : (*nextWaypoints))
        {
            vecPool->release(next);
        }

        range += 0.5;
        if (range > 4)
        {
            break;
        }
    }
    // 到这里什么都没有找到
    Workbench *wb = productType == 0 ? task->getFrom() : task->getTo();
    // System.err.println(statePool.usedSize() + statePool.availableSize());
    Vec *next = vecPool->acquire();
    next->set(wb->getPos());
    return next;
}

void Robot::searchNextWaypoints(MotionState *state1, MotionState *state2, double range, std::vector<Vec *> *nextWaypoints)
{
    Vec *pos = state2->getPos();
    Vec *v = state2->getVelocity();
    Workbench *wb = productType == 0 ? task->getFrom() : task->getTo();

    // 根据速度生成待遍历的点位
    if (v->mod() < 0.001)
    {
        // 机器人当前状态为移动，速度为零
        // 沿速度方向的单位向量
        Vec *eH = vecPool->acquire();
        eH->set(0, 1);
        // 沿速度垂直方向的单位向量
        Vec *eV = vecPool->acquire();
        eV->set(1, 0);
        // 沿速度45方向的单位向量
        Vec *e45 = vecPool->acquire();
        e45->set(SQRT2 / 2, SQRT2 / 2);
        // 沿速度135方向的单位向量
        Vec *e135 = vecPool->acquire();
        e135->set(-SQRT2 / 2, SQRT2 / 2);
        double tempRange[]{-range, range};
        for (double offset : tempRange)
        {
            Vec *item = vecPool->acquire();
            item->set(pos->getX() + offset * eH->getX(), pos->getY() + offset * eH->getY());
            nextWaypoints->emplace_back(item);
            item = vecPool->acquire();
            item->set(pos->getX() + offset * eV->getX(), pos->getY() + offset * eV->getY());
            nextWaypoints->emplace_back(item);
            item = vecPool->acquire();
            item->set(pos->getX() + offset * e45->getX(), pos->getY() + offset * e45->getY());
            nextWaypoints->emplace_back(item);
            item = vecPool->acquire();
            item->set(pos->getX() + offset * e135->getX(), pos->getY() + offset * e135->getY());
            nextWaypoints->emplace_back(item);
        }

        vecPool->release(eH);
        vecPool->release(eV);
        vecPool->release(e45);
        vecPool->release(e135);
    }
    else
    {
        // 机器人当前状态为移动，速度为零
        // 沿速度方向的单位向量
        Vec *eH = vecPool->acquire();
        eH->set(v->getX() / v->mod(), v->getY() / v->mod());
        // 沿速度垂直方向的单位向量
        Vec *eV = vecPool->acquire();
        eV->set(-v->getY() / v->mod(), v->getX() / v->mod());
        // 沿速度45方向的单位向量
        Vec *e45 = vecPool->acquire();
        e45->set(SQRT2 / 2 * v->getX() / v->mod(), SQRT2 / 2 * v->getY() / v->mod());
        // 沿速度135方向的单位向量
        Vec *e135 = vecPool->acquire();
        e135->set(-SQRT2 / 2 * v->getY() / v->mod(), SQRT2 / 2 * v->getX() / v->mod());

        // 机器人当前状态正在移动，移动垂直方向搜索
        double tempRange[]{-range, range};
        for (double offset : tempRange)
        {
            Vec *item = vecPool->acquire();
            item->set(pos->getX() + offset * eH->getX(), pos->getY() + offset * eH->getY());
            nextWaypoints->emplace_back(item);
            item = vecPool->acquire();
            item->set(pos->getX() + offset * eV->getX(), pos->getY() + offset * eV->getY());
            nextWaypoints->emplace_back(item);
            item = vecPool->acquire();
            item->set(pos->getX() + offset * e45->getX(), pos->getY() + offset * e45->getY());
            nextWaypoints->emplace_back(item);
            item = vecPool->acquire();
            item->set(pos->getX() + offset * e135->getX(), pos->getY() + offset * e135->getY());
            nextWaypoints->emplace_back(item);
        }

        vecPool->release(eH);
        vecPool->release(eV);
        vecPool->release(e45);
        vecPool->release(e135);
    }

    // 组装 当前点，目标点，中间点 点位，方便后续排序
    std::vector<std::vector<Vec *>> groupCoordinate;
    for (Vec *next : (*nextWaypoints))
    {
        Vec *curr = state1->getPos();
        Vec *target = wb->getPos();

        // 在同一条线上或者超出地图，都抛弃掉
        if (online(curr, next, target) || isOutMap(next))
        {
            // 释放不满足条件的point
            vecPool->release(next);
            continue;
        }
        std::vector<Vec *> a;
        a.emplace_back(curr);
        a.emplace_back(next);
        a.emplace_back(target);

        groupCoordinate.emplace_back(a);
    }

    // 为搜索点排序
    std::sort(groupCoordinate.begin(), groupCoordinate.end(), [](std::vector<Vec *> a1, std::vector<Vec *> a2)
              { return getAngleDiff(a1[0], a1[1], a1[2]) < getAngleDiff(a2[0], a2[1], a2[2]); });

    // 释放所有nextWaypoints
    for (Vec *next : (*nextWaypoints))
    {
        vecPool->release(next);
    }
    nextWaypoints->clear();
    for (std::vector<Vec *> gc : groupCoordinate)
    {
        Vec *nextpoint = vecPool->acquire();
        // TODO:这里为什么是1?
        nextpoint->set(gc[1]);
        nextWaypoints->emplace_back(nextpoint);

        // 释放所有groupCoordinate
        for (Vec *cd : gc)
        {
            vecPool->release(cd);
        }
    }
}