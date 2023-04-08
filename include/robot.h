#ifndef ROBOT_H
#define ROBOT_H

#include "includeAll.h"
#define SQRT2 1.4142135623730950488016887242097
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
    int frameId;                  // 帧id
    Vec pos;                      // 机器人坐标位置
    Vec velocity;                 // 线速度， 二维向量描述, m/s
    ActionModel actionModel;      // 机器人动作模型
    Task *task = nullptr;         // 机器人任务
    TaskChain taskChain;
    std::vector<Action *> actions;                       // 机器人动作序列
    PIDModel PID;                                        // 机器人PID模型
    ObjectPool<PIDModel> *pidPool = nullptr;             // 机器人PID模型对象池
    std::unordered_map<int, MotionState *> motionStates; // 用于记录机器人的运动状态序列

    std::vector<Robot *> *robotList = nullptr; // 机器人列表

    Vec *findMiddle(MotionState *crash);
    void searchNextWaypoints(MotionState *state1, MotionState *state2, double range, std::vector<Vec *> *nextWaypoints);

public:
    Robot(int id, double x, double y);
    ~Robot();

    Vec *predict();

    int getId() const;
    double getPriority() const;
    Vec getPos() const;
    Vec getVelocity() const;
    Task *getTask();
    int getProductType() const;
    int getLastProductType() const;
    int getWorkbenchIdx() const;
    std::vector<Action *> getActions() const;
    double getHeading() const;
    double getW() const;
    int getMapRow();
    int getMapCol();
    void clearStates();

    MotionState *getMotionState();
    void releaseMotionState(MotionState *ms);
    Vec *getVec();
    void releaseVec(Vec *v);
    void setRobotList(std::vector<Robot *> *robotList);
    void step();
    void checkDeal();
    void update(int frameId);
    void updatePid(int count);
    void control(MotionState *ms, Vec *pos, double &v, double &w);
    void addAction(Action *action);
    void bindChain(TaskChain *taskChain);
    void addPathPoint(Vec *point);

    bool isLoaded();
    bool isFree();
    void setAccessMap(int **accessMap);
    bool operator<(const Robot &o) const; // 重载为降序排列
    bool operator==(const Robot &o) const;
    size_t hash() const;
};
#endif