#include "robot.h"

Robot::Robot(int id, double x, double y)
{
    this->id = id;
    this->pos.set(x, y);
    this->actionModel.setRobot(this);
}

// 根据地图更改PID？？？
void Robot::updatePid(int count)
{
    // PID.set(5.5, 0.07, 1.3, 5.5, 0.07, 1.3);
}

void Robot::update(int leftFrame)
{
    int id;
    double x, y, vx, vy;
    // 读取信息
    scanf("%d %d %f %f %f %f %f %f %f %f\n",
          &id, &productType, &timeCoefficients, &collisionCoefficients,
          &w, &vx, &vy, &heading, &x, &y);

    // 更新
    this->leftFrame = leftFrame;
    pos.set(x, y);
    velocity.set(x, y);
}

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
