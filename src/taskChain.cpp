#include "includeAll.h"

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
        Workbench *from = task->getFrom();
        Workbench *to = task->getTo();

        from->setPlanProductStatus(1);
        to->updatePlanMaterialStatus(from->getType(), false);
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

void TaskChain::addTask(Task *task)
{
    this->taskChain.push_back(task);

    this->totalFrame += task->getDist() / MAX_FORWARD_FRAME;
}

// 删除Task，同时删除take的时间
void TaskChain::removeTask(int index)
{
    this->taskChain.
}