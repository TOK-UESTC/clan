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

inline void getDeliverableType(std::vector<int>& types, int wbType)
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

inline Vec* rc2Coord(int r, int c, double step) {
    double x = c * step;
    double y = 50.0 - r * step;

    Vec *result = new Vec(x, y);

    return result;
}

#endif