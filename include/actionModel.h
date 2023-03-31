#ifndef ACTION_MODEL_H
#define ACTION_MODEL_H

#include "includeAll.h"
/*
 * @brief 管理机器人动作
 *
 *
 */

class ActionModel
{

private:
    Robot *rb; // 机器人指针

public:
    void setRobot(Robot *rb) { this->rb = rb; } // 设置机器人指针

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