#include "include/includeAll.h"

Robot::Robot(int id, double x, double y) : id(id), pos(x, y), actionModel(this), pidModel(this)
{
    productType = 0;
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