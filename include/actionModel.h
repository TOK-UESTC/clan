#ifndef ACTION_MODEL_H
#define ACTION_MODEL_H

#include "includeAll.h"

// 前向引用，避免回环引用报错，并不美观
class Robot;
/*
 * @brief 管理机器人动作
 *
 *
 */
class ActionModel
{

private:
    Action rotateAction;
    Action forwardAction;
    Action buyAction;
    Action sellAction;
    ObjectPool<MotionState> motionStatePool; // 动作状态池
    ObjectPool<Vec> coordinatePool;          // 坐标池
    Robot *rb;

public:
    ActionModel(ObjectPool<MotionState> &motionStatePool, ObjectPool<Vec> &coordinatePool)
        : rotateAction(ROTATE), forwardAction(FORWARD), buyAction(BUY), sellAction(SELL), motionStatePool(motionStatePool), coordinatePool(coordinatePool)
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