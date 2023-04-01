#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include "const.h"
#include "vec.h"

class Utils
{
    // 计算坐标距离
public:
    static double computeDist(Vec *left, Vec *right)
    {
        double x = right->getX() - left->getX();
        double y = right->getY() - left->getY();

        return std::sqrt(x * x + y * y);
    }

    // 将w映射到题目范围内
    static double getAngleRanged(double angle)
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
};

#endif