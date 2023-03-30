#ifndef VEC_H
#define VEC_H

#include <cmath>

/*
 * @brief 与矢量相关的基类
 *
 *
 */
class Vec
{

private:
    double x;
    double y;

public:
    Vec()
    {
        this->x = 0.;
        this->y = 0.;
    }

    Vec(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    void set(const Vec &v)
    {
        this->x = v.x;
        this->y = v.y;
    }

    void set(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    double mod()
    {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }
};

#endif