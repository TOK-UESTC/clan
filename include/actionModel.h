#ifndef ACTION_MODEL_H
#define ACTION_MODEL_H

#include "includeAll.h"
class Robot;       // 前向声明, 只能通过指针或者引用访问
class MotionState; // 前向声明, 只能通过指针或者引用访问
class MotionModel; // 前向声明, 只能通过指针或者引用访问

/*
 * @brief 管理机器人动作
 *
 *
 */

class ActionModel
{

private:
    Robot *rb = nullptr; // 机器人指针
    Action rotateAction;
    Action forwardAction;
    Action buyAction;
    Action sellAction;
    std::list<Vec *> paths; // 路径序列,TODO:用对象池管理
    // Vec *pridictedPos = nullptr;                  // 预测位置

    MotionModel *motionModel = nullptr;

public:
    ActionModel(Robot *r) : rb(r), rotateAction(ROTATE), forwardAction(FORWARD), buyAction(BUY), sellAction(SELL)
    {

        motionModel = new MotionModel();
    }
    // 获取MotionState对象
    MotionState *getMotionState()
    {
        return motionModel->getMotionState();
    }
    // 释放MotionState对象
    void releaseMotionState(MotionState *state)
    {
        motionModel->releaseMotionState(state);
    }

    // 更新动作序列
    void addPathPoint(Vec *v)
    {
        paths.push_back(v);
    }

    /* 产生动作 */
    void generate()
    {
        generateShopActions();
        generateMoveActions();
    }

    void popPath();
    void setAccessMap(int **accessMap); // 为motionModel设置访问地图
    void generateMoveActions();         // 产生移动动作序列
    void generateShopActions();         // 产生交易动作序列
    std::list<Vec *> getPaths()
    {
        return paths;
    }
    MotionState *predict(MotionState *state, double targetVelocity, double targetAngularVelocity)
    {
        return motionModel->predict(*state, targetVelocity, targetAngularVelocity);
    }

private:
    double eval_spline(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &a, double xi);
    void spline(const std::vector<double> &x, const std::vector<double> &y, std::vector<double> &a);
};

#endif