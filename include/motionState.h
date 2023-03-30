#ifndef MOTION_STATE_H
#define MOTION_STATE_H

#include <cmath>
#include "vec.h"

class MotionState
{
private:
    Vec pos;        // 时刻t机器人位置
    Vec velocity;   // 时刻t机器人速度
    bool loaded;    // 时刻t机器人负载状态， true: 负载
    double heading; // 时刻t机器人位置朝向
    double w;       // 时刻t机器人角速度
public:
    MotionState();

    // void set(Robot rb);
    void set(const MotionState &s);
    void set(double x, double y, double vx, double vy, double heading, double w);

    double vMod();
    double getHeading();
    double getW();

    bool isLoaded();

    Vec getPos() const;
    Vec getVelocity() const;
};

#endif