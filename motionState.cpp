#include "include/includeAll.h"

MotionState::MotionState()
{
    pos.set(0, 0);
    velocity.set(0, 0);
    loaded = false;
    heading = 0.0;
    w = 0.0;
}

// void set(Robot rb);
void MotionState::set(const MotionState &s)
{
    pos.set(s.pos);
    velocity.set(s.velocity);
    heading = s.heading;
    w = s.w;
    loaded = s.loaded;
}

void MotionState::set(double x, double y, double vx, double vy, double heading, double w)
{
    pos.set(x, y);
    velocity.set(vx, vy);

    this->heading = heading;
    this->w = w;
}

double MotionState::vMod()
{
    return velocity.mod();
}

double MotionState::getHeading()
{
    return heading;
}

double MotionState::getW()
{
    return w;
}

bool MotionState::isLoaded()
{
    return loaded;
}

Vec *MotionState::getPos()
{
    return &pos;
}

Vec *MotionState::getVelocity()
{
    return &velocity;
}

void MotionState::update(Robot *rb)
{
    pos.set(rb->getPos());
    velocity.set(rb->getVelocity());
    heading = rb->getHeading();
    w = rb->getW();
    loaded = rb->isLoaded();
}

void MotionState::update(MotionState &ms)
{
    pos.set(ms.pos);
    velocity.set(ms.velocity);
    heading = ms.heading;
    w = ms.w;
    loaded = ms.loaded;
}

// setter and getter
void MotionState::setTargetVelocity(double targetVelocity)
{
    this->targetVelocity = targetVelocity;
}
void MotionState::setTargetAngularVelocity(double targetAngularVelocity)
{
    this->targetAngularVelocity = targetAngularVelocity;
}

void MotionState::setPos(double x, double y)
{
    this->pos.set(x, y);
}
void MotionState::setHeading(double heading)
{
    this->heading = heading;
}
void MotionState::setW(double w)
{
    this->w = w;
}
void MotionState::setVelocity(double vx, double vy)
{
    this->velocity.set(vx, vy);
}