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

public:
    PIDModel();
    void set(const PIDModel &s);
};

#endif