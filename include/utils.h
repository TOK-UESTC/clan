#ifndef UTILS_H
#define UTILS_H

#include "includeAll.h"

// 计算坐标距离
inline double computeDist(Vec *left, Vec *right)
{
    double x = right->getX() - left->getX();
    double y = right->getY() - left->getY();

    return sqrt(x * x + y * y);
}

// 将w映射到题目范围内
inline double getAngleRanged(double angle)
{
    if (angle > PI)
    {
        angle = angle - 2 * PI;
    }
    else if (angle < -PI)
    {
        angle = angle + 2 * PI;
    }
    return angle;
}

static void getDeliverableType(std::vector<int> &types, int wbType)
{
    switch (wbType)
    {
    case 1: // 类型4，5，9可回收1类型工作台的产品
    {
        types.push_back(4);
        types.push_back(5);
        types.push_back(9);
        break;
    }
    case 2: // 类型4，6，9可回收2类型工作台的产品
    {
        types.push_back(4);
        types.push_back(6);
        types.push_back(9);
        break;
    }
    case 3: // 类型5，6，9可回收3类型工作台的产品
    {
        types.push_back(5);
        types.push_back(6);
        types.push_back(9);
        break;
    }
    case 4: // 类型7， 9可回收4，5，6类型工作台的产品
    case 5:
    case 6:
    {
        types.push_back(7);
        types.push_back(9);
        break;
    }
    case 7: // 类型8，9可回收7类型工作台的产品
    {
        types.push_back(8);
        types.push_back(9);
        break;
    }
    default:
        break;
    }
}

/** 获取从from到to的直线连线角度 */
inline double getAngle(Vec *from, Vec *to)
{
    double x = from->getX() - to->getX();
    double y = from->getY() - to->getY();
    double quadrant = 1.; // 象限
    if (y < 0)
    {
        quadrant = -1.;
    }

    // 避免除0
    double mod = sqrt(x * x + y * y);
    if (mod < 0.0001)
    {
        return 0.;
    }
    else
    {
        return quadrant * acos(x / mod); // (-pi/2, pi/2)
    }
}

/** 获取当前位置, 中间位置, 和目标位置连线夹角差 */
inline double getAngleDiff(Vec *curr, Vec *middle, Vec *target)
{
    double diff = getAngle(middle, curr) - getAngle(target, middle);
    if (diff > PI)
    {
        diff -= 2 * PI;
    }
    else if (diff < -PI)
    {
        diff += 2 * PI;
    }

    return diff;
}

/** 当前位置，中间位置和碰撞位置是否在同一条连线上，当前判断角度为10° */
inline bool online(Vec *curr, Vec *pos, Vec *crash)
{
    return getAngleDiff(curr, pos, crash) < 10. * PI / 180.0;
}
/** 判断是否在地图外 */

inline bool isOutMap(Vec *pos)
{
    double x = pos->getX();
    double y = pos->getY();

    if (x < 0.53 || x > 50 - 0.53 || y < 0.53 || y > 50 - 0.53)
    {
        return true;
    }

    return false;
}
// TODO validCoord
#endif