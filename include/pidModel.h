#ifndef PID_MODEL_H
#define PID_MODEL_H

class PIDModel
{
private:
    // 角度PID参数
    double KpAngle;
    double KiAngle;
    double KdAngle;

    double lastErrAngle;
    double intAngle;
    // 积分值上限
    double intMaxAngle;

    // 距离PID参数
    double KpDist;
    double KiDist;
    double KdDist;

    double KpDistLoad;
    double KiDistLoad;
    double KdDistLoad;

    double lastErrDist;
    double intDist;
    // 积分值上限
    double intMaxDist;
    Robot *rb;

public:
    PIDModel(Robot *rb) : rb(rb)
    {
        KpAngle = 10;
        KiAngle = 0.1;
        KdAngle = 0.0;
        lastErrAngle = 0.0;
        intAngle = 0.0;
        intMaxAngle = 0.5;

        KpDist = 6.2;
        KiDist = 0.1;
        KdDist = 1.0;
        KpDistLoad = 6.2;
        KiDistLoad = 0.1;
        KdDistLoad = 1.0;
        lastErrDist = 0.0;
        intDist = 0.0;
        intMaxDist = 5.7;
    }

    void set(const PIDModel &s);
    void set(double KpDist, double KiDist, double KdDist, double KpDistLoad, double KiDistLoad, double KdDistLoad);

    void control(MotionState *ms, Vec *targetPos, double &v, double &w);
};

#endif