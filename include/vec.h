#ifndef VEC_H
#define VEC_H

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

    void set(double x, double y)
    {
        this->x = x;
        this->y = y;
    }
};

#endif