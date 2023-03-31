#ifndef ROBOT_H
#define ROBOT_H

#include "includeAll.h"
/*
 * @brief 机器人代理类
 *
 *
 */

class Robot
{

private:
    int id;                       // 机器人id
    int workbenchIdx;             // 所处工作台下标, -1表示没有处于任何工作台, [0, K-1]表是某工作台下标
    int productType;              // 携带物品类型[0, 7], 0表示未携带物品
    int lastProductType;          // 上一帧携带的品
    double timeCoefficients;      // 时间价值系数 [0.8, 1]
    double collisionCoefficients; // 碰撞价值系数 [0.8, 1]
    double w;                     // 角速度 单位：弧度每秒， 正数表示顺时针， 负数表示逆时针
    double heading;               // 朝向 [-pi, pi] 0 表示右方向, pi/2表示上方向
    int leftFrame;                // 剩余帧数
    Vec pos;                      // 机器人坐标位置
    Vec velocity;                 // 线速度， 二维向量描述, m/s
    ActionModel actionModel;      // 机器人动作模型

public:
    Robot(int id, double x, double y);

    int getId() const;
    double getPriority() const;
    Vec getPos() const;
    Vec getVelocity() const;

    void checkDeal();
    void update(int leftFrame);
    void updatePid(int count);
    void control();

    bool isLoaded();
    bool operator<(const Robot &o) const; // 重载为降序排列
};
#endif