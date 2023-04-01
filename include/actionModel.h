#ifndef ACTION_MODEL_H
#define ACTION_MODEL_H

#include "includeAll.h"
class Robot; // 前向声明, 只能通过指针或者引用访问
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
    // ObjectPool<MotionState> motionStatePool; // 动作状态池
    // ObjectPool<Vec> coordinatePool;          // 坐标池

public:
    ActionModel(Robot *r) : rb(r), rotateAction(ROTATE), forwardAction(FORWARD), buyAction(BUY), sellAction(SELL)
    {
    } // 构造函数
    // void setRobot(Robot *rb) { this->rb = rb; } // 设置机器人指针

    /* 产生动作序列 */
    void generate()
    {
        generateShopActions();
        generateMoveActions();
    }

    void generateMoveActions(); // 产生移动动作序列
    void generateShopActions(); // 产生交易动作序列
};

#endif