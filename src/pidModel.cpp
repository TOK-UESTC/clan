#include "pidModel.h"

class PIDModel
{
private:
    // 角度PID参数
    double KpAngle = 10;
    double KiAngle = 0.1;
    double KdAngle = 0.0;

    double lastErrAngle = 0;
    double intAngle = 0;
    // 积分值上限
    double intMaxAngle = 0.5;

    // 距离PID参数
    double KpDist = 6.2;
    double KiDist = 0.1;
    double KdDist = 1;

    double KpDistLoad = 6.2;
    double KiDistLoad = 0.1;
    double KdDistLoad = 1;

    double lastErrDist = 0;
    double intDist = 0;
    // 积分值上限
    double intMaxDist = 5.7;

public:
    PIDModel()
    {
    }

    void copy(const PIDModel &s)
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
        // this->rb = model.rb;
    }
};