#include "include/includeAll.h"

Robot::Robot(int id, double x, double y) : id(id), pos(x, y), actionModel(this), pidModel(this)
{
    productType = 0;
}


Robot::~Robot()
{
    // 释放dijkstra
    delete dijkstra;
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
    pidModel.control(ms, pos, v, w);
}

void Robot::update(int leftFrame)
{

    int id;
    double x, y, vx, vy;
    // 读取信息
    scanf("%d %d %lf %lf %lf %lf %lf %lf %lf %lf\n",
          &id, &productType, &timeCoefficients, &collisionCoefficients,
          &w, &vx, &vy, &heading, &x, &y);
    getchar();

    // 更新
    this->leftFrame = leftFrame;
    pos.set(x, y);
    velocity.set(vx, vy);
}

size_t Robot::hash() const
{
    return id;
}

// TODO
void Robot::checkDeal()
{
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
    this->taskChain->set(*taskChain);
    // 构建路径
    // rb->from 路径
    Workbench from = taskChain->getTask(0)->getFrom();
    std::list<Vec *> * result = dijkstra->getKnee(from.getMapRow(), from.getMapCol());
    for(auto p:*result){
        addPathPoint(new Vec(p->getX(), p->getY()));
        delete p;
    }
    delete result;
    // 加入任务路径
    for(auto t:taskChain->getTaskChain())
    {
        for(auto p:*(t->getRoad()))
        {
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
void Robot::addPathPoint(Vec* point)
{
    actionModel.addPathPoint(point);
}

void Robot::setDij(Dijkstra *dijkstra)
{
    this->dijkstra = dijkstra;
}

Dijkstra *Robot::getDij()
{
    return dijkstra;
}
