#ifndef ACTION_MODEL_H
#define ACTION_MODEL_H

#include "includeAll.h"
class Robot;       // 前向声明, 只能通过指针或者引用访问
class MotionState; // 前向声明, 只能通过指针或者引用访问
/*
 * @brief 管理机器人动作
 *
 *
 */

class ActionModel
{

private:
    Robot *rb; // 机器人指针
    Action rotateAction;
    Action forwardAction;
    Action buyAction;
    Action sellAction;
    ObjectPool<MotionState> *statePool; // 动作状态池
    std::list<Vec *> paths;             // 路径序列,TODO:用对象池管理

public:
    ActionModel(Robot *r) : rb(r), rotateAction(ROTATE), forwardAction(FORWARD), buyAction(BUY), sellAction(SELL)
    {
        statePool = new ObjectPool<MotionState>(100);
    }
    // 构造函数
    // void setRobot(Robot *rb) { this->rb = rb; } // 设置机器人指针

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

    void generateMoveActions(); // 产生移动动作序列
    void generateShopActions(); // 产生交易动作序列
    std::list<Vec *> getPaths()
    {
        return paths;
    }
private:
    double eval_spline(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &a, double xi);
    void spline(const std::vector<double> &x, const std::vector<double> &y, std::vector<double> &a);
};

#endif