#include "include/includeAll.h"

void PIDModel::set(const PIDModel &s)
{
    this->KpAngle = s.KpAngle;
    this->KiAngle = s.KiAngle;
    this->KdAngle = s.KdAngle;
    this->lastErrAngle = s.lastErrAngle;
    this->intAngle = s.intAngle;
    this->intMaxAngle = s.intMaxAngle;
    this->KpDist = s.KpDist;
    this->KiDist = s.KiDist;
    this->KdDist = s.KdDist;
    this->KpDistLoad = s.KpDistLoad;
    this->KiDistLoad = s.KiDistLoad;
    this->KdDistLoad = s.KdDistLoad;
    this->lastErrDist = s.lastErrDist;
    this->intDist = s.intDist;
    this->intMaxDist = s.intMaxDist;
    this->rb = s.rb;
}

void PIDModel::set(double KpDist, double KiDist, double KdDist, double KpDistLoad, double KiDistLoad,
                   double KdDistLoad)
{
    this->KpDist = KpDist;
    this->KiDist = KiDist;
    this->KdDist = KdDist;
    this->KpDistLoad = KpDistLoad;
    this->KiDistLoad = KiDistLoad;
    this->KdDistLoad = KdDistLoad;
}

void PIDModel::control(MotionState *ms, Vec *targetPos, double &v, double &w)
{
    Vec *pos = ms->getPos();
    double posX = pos->getX();
    double posY = pos->getY();
    double heading = ms->getHeading();

    // 获取距离误差
    double distErr = computeDist(pos, targetPos);

    // 计算角度误差，根据两者的坐标
    double diffX = targetPos->getX() - posX;
    double diffY = targetPos->getY() - posY;
    double quadrant = 1.; // 象限
    if (diffY < 0)
    {
        quadrant = -1.;
    }
    double angle = 0;
    if (distErr > 0.04)
    {
        angle = quadrant * acos(diffX / distErr);
    }

    double angleErr = angle - ms->getHeading();
    angleErr = getAngleRanged(angleErr);

    /// 判断离墙是否太近，TODO
    double minWallDist = 4;
    // 减速系数
    double deceleration = 0.3;
    double minDistWallX = posX < 25 ? posX : (50 - posX);
    double minDistWallY = posY < 25 ? posY : (50 - posY);
    bool closetoX = false;
    bool closetoY = false;
    // 根据当前角度与离墙距离的关系，进行距离误差的修正
    if (posX < minWallDist && (heading > PI * 5 / 8 || heading < -PI * 5 / 8))
    {
        closetoX = true;
    }
    else if (posX > 50 - minWallDist && heading < PI * 3 / 8 && heading > -PI * 3 / 8)
    {
        closetoX = true;
    }
    if (posY < minWallDist && heading < -PI / 8 && heading > -PI * 7 / 8)
    {
        closetoY = true;
    }
    else if (posY > 50 - minWallDist && heading > PI / 8 && heading < PI * 7 / 8)
    {
        closetoY = true;
    }

    if (closetoX || closetoY)
    {
        distErr = std::min(distErr, deceleration * std::min(minDistWallX, minDistWallY));
    }

    // 距离环控制
    double lineVelocity = 0;

    // 根据载重情况决定使用何种pid
    if (rb->isLoaded())
    {
        lineVelocity = (KpDistLoad * distErr + KiDistLoad * intDist + KdDistLoad * (distErr - lastErrDist));
    }
    else
    {
        lineVelocity = (KpDist * distErr + KiDist * intDist + KdDist * (distErr - lastErrDist));
    }

    // 记录误差
    lastErrDist = distErr;
    intDist += distErr;
    intDist = std::min(intDist, intMaxDist);
    intDist = std::max(intDist, -intMaxDist);

    // 角度环控制
    double angularVelocity = (KpAngle * angleErr + KiAngle * intAngle + KdAngle * (angleErr - lastErrAngle));

    // 记录误差
    lastErrAngle = angleErr;
    intAngle += angleErr;
    intAngle = std::min(intAngle, intMaxAngle);
    intAngle = std::max(intAngle, -intMaxAngle);

    // 对输出值进行规范化，保证输出的最大值是规范的
    lineVelocity = lineVelocity > MAX_FORWARD_VELOCITY ? MAX_FORWARD_VELOCITY : lineVelocity;
    lineVelocity = lineVelocity < MAX_BACKWARD_VELOCITY ? MAX_BACKWARD_VELOCITY : lineVelocity;
    angularVelocity = angularVelocity > MAX_ANGULAR_VELOCITY ? MAX_ANGULAR_VELOCITY : angularVelocity;
    angularVelocity = angularVelocity < -MAX_ANGULAR_VELOCITY ? -MAX_ANGULAR_VELOCITY : angularVelocity;

    // 策略：角度大就先停止
    // if (abs(angleErr) > PI / 32) {
    // lineVelocity = 0;
    // }

    // 策略:角度越大,速度越小
    if (fabs(angleErr) > PI / 4)
    {
        // lineVelocity = lineVelocity * deceleration / fabs(angleErr);
        lineVelocity = 0;
    }

    v = lineVelocity;
    w = angularVelocity;
}
