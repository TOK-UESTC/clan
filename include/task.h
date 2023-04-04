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
    Workbench &from;                   // 任务来源
    Workbench &to;                     // 任务目的
    double priority;                   // 任务优先级，值越大优先级越高
    double price;                      // 任务买入价格
    double sellPrice;                  // 任务售出价格
    double distance;                   // 任务距离
    std::vector<Vec*>* road;                         // 任务路径
    std::vector<Task *> *postTaskList; // 后继任务列表

public:
    Task(Workbench &from, Workbench &to) : from(from), to(to)
    {
    }

    ~Task(){
        // 释放road
        for(auto r:*road){
            delete r;
        }
        delete road;
    }

    Workbench getFrom()
    {
        return from;
    }

    Workbench getTo()
    {
        return to;
    }

    double getDist()
    {
        return distance;
    }

    std::vector<Vec*>* getRoad(){
        return road;
    }

    double getProfit(double timeCoefficients, double collisionCoefficients)
    {
        return sellPrice * timeCoefficients * collisionCoefficients - price;
    }

    void setpostTaskList(std::vector<Task *> *postTaskList)
    {
        this->postTaskList = postTaskList;
    }

    void setDist(double dist){
        this->distance = dist;
    }

    void setRoad(std::vector<Vec*>* road){
        this->road = road;
    }

    std::vector<Task *> *getPostTaskList()
    {
        return this->postTaskList;
    }
};
#endif