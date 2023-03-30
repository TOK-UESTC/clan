#include "robot.h"

class Robot
{
private:
    int id;                       // 机器人id
    int wbId;                     // 所处工作台下标, -1表示没有处于任何工作台, [0, K-1]表是某工作台下标
    int productType;              // 携带物品类型[0, 7], 0表示未携带物品
    int lastProductType;          // 上一帧携带的品
    double timeCoefficients;      // 时间价值系数 [0.8, 1]
    double collisionCoefficients; // 碰撞价值系数 [0.8, 1]
    double angularVelocity;       // 角速度 单位：弧度每秒， 正数表示顺时针， 负数表示逆时针
    double heading;               // 朝向 [-pi, pi] 0 表示右方向, pi/2表示上方向
    Vec pos;                      // 机器人坐标位置
    Vec velocity;                 // 线速度， 二维向量描述, m/s

public:
    Robot(int id, double x, double y)
    {
        this->id = id;
        this->pos.set(x, y);
    }

    // 优先级排序
    double getPriority() const
    {
        return 1.0;
    }

    bool operator<(const Robot &o) const
    {
        return this->getPriority() < o.getPriority();
    }
};