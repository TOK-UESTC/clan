#include "include/includeAll.h"

TaskChain::TaskChain()
{
}

TaskChain::TaskChain(double totalFrame)
{
    this->totalFrame = totalFrame;
}

void TaskChain::set(const TaskChain &s)
{
    this->taskChain = s.taskChain;
    this->totalFrame = s.totalFrame;
}

void TaskChain::set(double totalFrame)
{
    this->taskChain.clear();
    this->totalFrame = totalFrame;
}

void TaskChain::occupy()
{
    for (Task *task : taskChain)
    {
        Workbench from = task->getFrom();
        Workbench to = task->getTo();

        from.setPlanProductStatus(1);
        to.updatePlanMaterialStatus(from.getType(), false);
    }
}

bool TaskChain::isOccupied() const
{
    // 判断该任务链是否被占据
    // 1. 生产工作台产品格未被占据: task.getFrom().getPlanProductStatus() == 0
    // 2. 消费工作台原料格未被占据: task.getTo().hasPlanMaterial(task.getFrom().getType())
    for (Task *task : taskChain)
    {
        Workbench from = task->getFrom();
        Workbench to = task->getTo();
        if (from.getPlanProductStatus() != 0 || to.hasPlanMaterial(from.getType()))
        {
            return true;
        }
    }
    return false;
}

Task *TaskChain::getTask(int index)
{
    if (taskChain.size() == 0)
    {
        return nullptr;
    }

    if (index == -1)
    {
        return taskChain.at(taskChain.size() - 1);
    }

    return taskChain.at(index);
}

void TaskChain::addTask(Task *task)
{
    this->taskChain.push_back(task);

    this->totalFrame += task->getDist() / MAX_FORWARD_FRAME;
}

// 删除Task，同时删除take的时间
// void TaskChain::removeTask(int index)
// {
//     this->taskChain.
// }

double TaskChain::getProfit()
{
    double profit = 0.;
    for (Task *task : taskChain)
    {
        // TODO: 这里的计算是临时的
        profit += task->getProfit(1., 1.);
    }
    return profit;
}

/** 完成任务预估需要的帧数 */
double TaskChain::getTotalFrame()
{
    return totalFrame;
}

bool TaskChain::operator<(TaskChain *o)
{
    return getProfit() < o->getProfit();
}