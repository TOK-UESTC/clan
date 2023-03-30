#ifndef ACTION_MODEL_H
#define ACTION_MODEL_H

#include "robot.h"

/*
 * @brief 管理机器人动作
 *
 *
 */

class ActionModel
{

private:
    Robot rb;

public:
    ActionModel(Robot &rb)
    {
        this->rb = rb;
    }

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