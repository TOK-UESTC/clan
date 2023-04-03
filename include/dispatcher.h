#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "includeAll.h"

class Dispatcher
{
private:
    std::vector<Robot *> &robotList;
    std::vector<Robot*> freeRobots;
    std::vector<Workbench *> &workbenchList;
    std::unordered_map<int, std::vector<Workbench *> *> &workbenchTypeMap; // {工作台类型:工作台列表}字典
    std::unordered_map<int, std::vector<Task *> *> &workbenchIdTaskMap;    // {工作台id:该id工作台发出的任务列表}字典

    // key: 工作台种类, value: 以工作台产物为原料的所有任务
    std::unordered_map<int, std::vector<Task *> *> taskTypeMap;
    std::unordered_map<Robot*, std::priority_queue<TaskChain*>*> taskChainQueueMap;
    std::priority_queue<TaskChain*> *tempQueue;

    // 任务链池子
    ObjectPool<TaskChain> *chainPool;
    // ObjectPool<MotionState*> * statePool;

public:
    Dispatcher(std::vector<Robot *> &robotList, std::vector<Workbench *> &workbenchList, std::unordered_map<int, std::vector<Workbench *> *> &workbenchTypeMap, std::unordered_map<int, std::vector<Task *> *> &workbenchIdTaskMap);
    ~Dispatcher();

    void init();
    void clearChainMap(Robot* rb);
    bool isQueueMapEmpty(Robot* rb);
    void generateTaskChains(); // 生成任务链
    void updateTaskChain();    // 添加任务链 
    void copyQueue(std::priority_queue<TaskChain*> * source, std::priority_queue<TaskChain*> * target); // 复制任务链
    void updateFreeBot(); // 更新机器人列表
    void dispatch(); // 分配
};

#endif