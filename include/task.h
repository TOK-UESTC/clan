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
    Workbench *from;                             // 任务来源
    Workbench *to;                               // 任务目的
    double priority;                             // 任务优先级，值越大优先级越高
    double price;                                // 任务买入价格
    double sellPrice;                            // 任务售出价格
    double distance;                             // 任务距离
    std::list<Vec *> *road;                      // 任务路径
    std::vector<Task *> *postTaskList = nullptr; // 后继任务列表

public:
    Task(Workbench* from, Workbench* to) : from(from), to(to)
    {
        this->price = priceMapper[from->getType()][0];
        this->sellPrice = priceMapper[from->getType()][1];
    }

    ~Task()
    {
        // 释放road
        for (auto r : *road)
        {
            delete r;
        }
        delete road;
    }

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
        return distance;
    }

    std::list<Vec *> *getRoad()
    {
        return road;
    }

    double getProfit()
    {
        double timeCoefficient;
        double predictedFrame = distance / MAX_FORWARD_FRAME;
        if (predictedFrame >= 9000)
        {
            timeCoefficient = 0.8;
        }
        else
        {
            timeCoefficient = (1 - sqrt(1 - pow(1 - (predictedFrame / 9000), 2))) * (1 - 0.8) + 0.8;
        }
        return sellPrice * timeCoefficient - price;
    }

    void setpostTaskList(std::vector<Task *> *postTaskList)
    {
        this->postTaskList = postTaskList;
    }

    void setDist(double dist)
    {
        this->distance = dist * 0.25;
    }

    void setRoad(std::list<Vec *> *road)
    {
        this->road = road;
    }

    std::vector<Task *> *getPostTaskList()
    {
        return this->postTaskList;
    }
};
#endif