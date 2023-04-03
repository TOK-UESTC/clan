#include "include/includeAll.h"

Dispatcher::Dispatcher(std::vector<Robot *> &robotList, std::vector<Workbench *> &workbenchList, std::unordered_map<int, std::vector<Workbench *> *> &workbenchTypeMap, std::unordered_map<int, std::vector<Task *> *> &workbenchIdTaskMap) : robotList(robotList), workbenchList(workbenchList), workbenchTypeMap(workbenchTypeMap), workbenchIdTaskMap(workbenchIdTaskMap)
{
    chainPool = new ObjectPool<TaskChain>(100);
    init();
}

Dispatcher::~Dispatcher()
{
    // 释放map
    for (auto iter = taskTypeMap.begin(); iter != taskTypeMap.end(); iter++)
    {
        delete iter->second;
    }

    // 释放池子
    delete chainPool;
}

/*
 * @brief 构建任务
 *
 *
 */
void Dispatcher::init()
{
    for (Workbench *wb : workbenchList)
    {
        int wbType = wb->getType();
        if (wbType == 8 || wbType == 9)
        {
            continue;
        }
        std::vector<int> types;
        getDeliverableType(types, wbType);
        for (auto type : types)
        {
            // 如果当前地图上不含某些工作台，直接跳过
            if (workbenchTypeMap.find(type) == workbenchTypeMap.end())
            {
                continue;
            }
            // 生成任务
            for (Workbench *target : *(workbenchTypeMap.find(type)->second))
            {
                workbenchIdTaskMap[wb->getId()]->push_back(new Task(*wb, *target));
            }
        }

        // 将同原料的所有任务储存到一起，方便产生任务链
        if (taskTypeMap.find(wbType) == taskTypeMap.end())
        {
            taskTypeMap[wbType] = new std::vector<Task *>();
        }
        taskTypeMap[wbType]->insert(taskTypeMap[wbType]->end(), workbenchIdTaskMap[wb->getId()]->begin(), workbenchIdTaskMap[wb->getId()]->end());
    }

    // 为每个任务添加后续任务
    for (auto tasks : taskTypeMap)
    {
        for (auto task : *(tasks.second))
        {
            task->setpostTaskList(workbenchIdTaskMap[task->getTo().getId()]);
        }
    }
}

/*
 * @brief 构建任务链
 *
 *
 */
void Dispatcher::clearChainMap(Robot *rb)
{
    if (rb == nullptr)
    {
        // 释放所有的chain对象
    }
    else
    {
    }
}

/*
 * @brief 构建任务链
 *
 *
 */
void Dispatcher::generateTaskChains(int leftFrame)
{
}