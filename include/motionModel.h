#ifndef MOTION_MODEL_H
#define MOTION_MODEL_H

#include "includeAll.h"

class MotionModel
{
private:
    const double FRAME_TIME = 0.02;                 // 时间常量
    const double LINEAR_ACC = 19.591362775;         // 单位：m/s
    const double LOADED_LINEAR_ACC = 14.081;        // 单位：m/s
    const double ANGULAR_ACC = 38.695931425;        // 单位：rad/s
    const double LOADED_ANGULAR_ACC = 20.130082965; // 单位：rad/s

    const double PI2 = PI * PI;
    const double sqrtPI = std::sqrt(PI);
    const double MIN_ERROR = 0.0001;
    double a;
    double b;
    double bc;
    double bs;
    double b0;
    double b0c;
    double b0s;
    double c;

    ObjectPool<MotionState> *statePool = nullptr;
    ObjectPool<MotionFrag> *fragPool = nullptr;

    double pow(double x, double p);
    double sqrt(double x);
    double FresnelC(double x);
    double FresnelS(double x);
    double getIntegralYFront(double v0, double angularAcc, double theta0, double omega0, double t);
    double getIntegralYBack(double linearAcc, double angularAcc, double theta0, double omega0, double t);
    double getIntegralXBack(double linearAcc, double angularAcc, double theta0, double omega0, double t);
    double getIntegralXFront(double v0, double angularAcc, double theta0, double omega0, double t);
    double getIntegralYAngular0(double v0, double linearAcc, double theta0, double omega0, double t);
    double getIntegralXAngular0(double v0, double linearAcc, double theta0, double omega0, double t);
    double getAngularAcc(bool isloaded, double targetAngularVelocity, double w, double t);
    bool predictFrag(MotionState *state, MotionFrag *frag);
    bool checkObstacle(MotionState *ms);
    int **accessMap = nullptr;

public:
    MotionModel()
    {
        statePool = new ObjectPool<MotionState>(100);
        fragPool = new ObjectPool<MotionFrag>(100);
    }
    MotionState *predict(MotionState &state, double targetVelocity, double targetAngularVelocity);
    void releaseMotionState(MotionState *state);
    void setAccessMap(int **accessMap);
    // 提供motionstate对象
    MotionState *getMotionState()
    {
        return statePool->acquire();
    }
};

#endif