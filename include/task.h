#ifndef TASK_H
#define TASK_H

#include "includeAll.h"

/*
 * @brief 基础任务类
 *
 *
 */

class Task
{

private:
    Workbench *from;                  // 任务来源
    Workbench *to;                    // 任务目的
    double priority;                  // 任务优先级，值越大优先级越高
    double price;                     // 任务买入价格
    double sellPrice;                 // 任务售出价格
    double distance;                  // 任务距离
    Vec *road;                        // 任务路径
    std::vector<Task *> postTaskList; // 后继任务列表

public:
    Task(Workbench &from, Workbench &to);

    Workbench *getFrom()
    {
        return from;
    }

    Workbench *getTo()
    {
        return to;
    }

    double getDist()
    {
        return computeDist(from->getPos(), to->getPos());
    }
};
#endif