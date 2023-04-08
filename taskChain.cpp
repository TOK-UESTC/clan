#include "include/includeAll.h"

TaskChain::TaskChain()
{
    this->totalFrame = 0;
}

TaskChain::TaskChain(double totalFrame)
{
    this->totalFrame = totalFrame;
}

void TaskChain::set(TaskChain *s)
{
    this->taskChain.clear();
    for (Task *task : s->taskChain)
    {
        this->taskChain.push_back(task);
    }
    this->totalFrame = s->totalFrame;
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
        Workbench *from = task->getFrom();
        Workbench *to = task->getTo();

        from->setPlanProductStatus(1);
        to->updatePlanMaterialStatus(from->getType(), false);

        // 更新balanceMapper
        if (to->getType() == 4 || to->getType() == 5 || to->getType() == 6)
        {
            balanceMapper[to->getType()]++;
        }
    }
}

bool TaskChain::isOccupied() const
{
    // 判断该任务链是否被占据
    // 1. 生产工作台产品格未被占据: task.getFrom().getPlanProductStatus() == 0
    // 2. 消费工作台原料格未被占据: task.getTo().hasPlanMaterial(task.getFrom().getType())
    for (Task *task : taskChain)
    {
        Workbench *from = task->getFrom();
        Workbench *to = task->getTo();
        if (from->getPlanProductStatus() != 0 || to->hasPlanMaterial(from->getType()))
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
        if (taskChain.at(taskChain.size() - 1) == nullptr)
        {
            return nullptr;
        }
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
void TaskChain::removeTask(int index)
{
    if (taskChain.size() != 0)
    {
        this->taskChain.erase(this->taskChain.begin() + index);
    }
}

Task *TaskChain::getNextTask()
{
    if (taskChain.size() != 0)
    {
        return taskChain.at(0);
    }
    return nullptr;
}

double TaskChain::getProfit()
{
    double profit = 0.;
    for (Task *task : taskChain)
    {
        // TODO: 这里的计算是临时的
        profit += task->getProfit();
    }
    return profit/totalFrame;
}

/** 完成任务预估需要的帧数 */
double TaskChain::getTotalFrame()
{
    return totalFrame;
}

// bool TaskChain::operator<(TaskChain *o)
// {
//     return o->getProfit() < getProfit();
// }

std::vector<Task *> TaskChain::getTaskChain()
{
    return taskChain;
}