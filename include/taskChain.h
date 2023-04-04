#ifndef TASK_CHAIN_H
#define TASK_CHAIN_H

#include "includeAll.h"
class Robot;
/*
 * @brief 组织任务构成任务链
 *
 *
 */
class TaskChain
{

private:
    std::vector<Task *> taskChain; // 用来存储任务链中的任务
    double totalFrame;             // 在已有任务链的条件下，完成任务所需最快帧数

public:
    TaskChain();
    TaskChain(double totalFrame); // 通过任务总时间初始化任务链
    // ~TaskChain();                 // 清理内存
    // TaskChain(TaskChain &chain);  // 通过任务任务链初始化任务链

    void set(const TaskChain &s); // 通过任务链更新任务链
    void set(double totalFrame);  // 通过任务时间更新任务链
    void occupy();                // 将该任务链上的工作台都置为使用中，避免后续机器人重复领取
    bool isOccupied() const;      // 判断该链条上是否有工作台被占用
    void addTask(Task *task);     // 为任务链添加任务，同时更新任务链完成所需要的总帧数
    void removeTask(int index);   // 删除taskChain[index]
    double getProfit();           // 任务链的单位时间收益
    Task* getTask(int index);     // 根据下标获取任务
    double getTotalFrame();       // 完成任务预估需要的帧数

    bool operator<(TaskChain *o); // 重载为降序排列
};

#endif