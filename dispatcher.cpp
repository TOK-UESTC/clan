#include "include/includeAll.h"

Dispatcher::Dispatcher(std::vector<Robot *> &robotList, std::vector<Workbench *> &workbenchList, std::unordered_map<int, std::vector<Workbench *> *> &workbenchTypeMap, std::unordered_map<int, std::vector<Task *> *> &workbenchIdTaskMap, int **accessMap) : robotList(robotList), workbenchList(workbenchList), workbenchTypeMap(workbenchTypeMap), workbenchIdTaskMap(workbenchIdTaskMap), taskTypeMap{}, taskChainQueueMap{}
{
    this->chainPool = new ObjectPool<TaskChain>(100);
    this->tempQueue = new std::priority_queue<TaskChain *>();
    this->accessMap = accessMap;
    for (auto rb : robotList)
    {
        taskChainQueueMap[rb] = new std::priority_queue<TaskChain *>();
    }

    init();
}

Dispatcher::~Dispatcher()
{
    // 释放map
    for (auto iter = taskTypeMap.begin(); iter != taskTypeMap.end(); iter++)
    {
        delete iter->second;
    }

    for (auto iter = taskChainQueueMap.begin(); iter != taskChainQueueMap.end(); iter++)
    {
        delete iter->second;
    }

    // 释放池子
    delete chainPool;
    // 释放临时队列
    delete tempQueue;
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
        // 如果工作台类型是8、9或者工作台在空载的时候不可访问，那么不生成任务
        if (wbType == 8 || wbType == 9 || (accessMap[wb->getMapRow()][wb->getMapCol()] & 0b1111) == 0)
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
            for (Workbench *target : *((*workbenchTypeMap.find(type)).second))
            {
                // 从wb -> target 在负载的情况下不可达target，那么不生成任务
                if (wb->getDij()->getDistMap(true)[target->getMapRow()][target->getMapCol()] > 999999.)
                {
                    continue;
                }
                workbenchIdTaskMap[wb->getId()]->push_back(new Task(wb, target));
            }
        }

        // 将同原料的所有任务储存到一起，方便产生任务链
        if (taskTypeMap.find(wbType) == taskTypeMap.end())
        {
            taskTypeMap[wbType] = new std::vector<Task *>();
        }
        taskTypeMap[wbType]->insert(taskTypeMap[wbType]->end(), workbenchIdTaskMap[wb->getId()]->begin(), workbenchIdTaskMap[wb->getId()]->end());
    }

    // 为每个任务添加最短距离
    for (Workbench *wb : workbenchList)
    {
        int wbType = wb->getType();
        // 如果工作台类型是8、9或者工作台在空载的时候不可访问，那么不生成任务
        if (wbType == 8 || wbType == 9 || (accessMap[wb->getMapRow()][wb->getMapCol()] & 0b1111) == 0 || workbenchIdTaskMap[wb->getId()]->size() == 0)
        {
            continue;
        }
        int row = wb->getMapRow();
        int col = wb->getMapCol();

        double **dijkstraMap = wb->getDij()->getDistMap(true);

        for (auto task : *(workbenchIdTaskMap[wb->getId()]))
        {
            Workbench *to = task->getTo();
            task->setDist(dijkstraMap[to->getMapRow()][to->getMapCol()]);
            // 设置最短路径
            std::list<Vec *> *result = wb->getDij()->getKnee(to->getMapRow(), to->getMapCol(), true);
            task->setRoad(result);
        }
    }

    // 为每个任务添加后续任务
    for (auto tasks : taskTypeMap)
    {
        for (auto task : *(tasks.second))
        {
            task->setpostTaskList(workbenchIdTaskMap[task->getTo()->getId()]);
        }
    }
}

/*
 * @brief 清空任务链字典
 *
 *
 */
void Dispatcher::clearChainMap(Robot *rb)
{
    if (rb == nullptr)
    {
        // 释放所有的chain对象
        for (auto iter : taskChainQueueMap)
        {
            while (!iter.second->empty())
            {
                chainPool->release(iter.second->top());
                iter.second->pop();
            }
        }
    }
    else
    {
        std::priority_queue<TaskChain *> *queue = (*taskChainQueueMap.find(rb)).second;
        while (!queue->empty())
        {
            chainPool->release(queue->top());
            queue->pop();
        }
    }
}

/*
 * @brief 判断任务链是否为空
 *
 *
 */
bool Dispatcher::isQueueMapEmpty(Robot *rb)
{
    if (rb == nullptr)
    {
        for (auto iter : taskChainQueueMap)
        {
            if (iter.second->size() != 0)
            {
                return false;
            }
        }
        return true;
    }

    std::priority_queue<TaskChain *> *queue = (*taskChainQueueMap.find(rb)).second;
    return queue->size() == 0;
}

/*
 * @brief 构建任务链
 *
 *
 */
void Dispatcher::generateTaskChains()
{
    clearChainMap(nullptr);

    // 如果工作台可以投入生产，继续进行判断
    for (Robot *rb : freeRobots)
    {
        // 遍历task
        for (auto taskListPair : taskTypeMap)
        {
            for (Task *task : *(taskListPair.second))
            {
                Workbench *from = task->getFrom();
                Workbench *to = task->getTo();
                /*
                 * 环境应满足：
                 * 1. from工作台必须已经投入生产: from.isFree() true 表示未生产
                 * 2. to工作台原材料格没有被占用: to.hasMaterial(from.getType())
                 *
                 * 规划应满足:
                 * 1. from工作台规划产品格没被占领:from.getPlanProductStatus() == 1 true表示被占据
                 * 2. to工作台的规划原料格(planMaterialStatus)没被占用:to.hasPlanMaterial(from.getType())
                 *
                 * 访问应满足:
                 * 1. from工作台空载能访问 accessFrom&(1<<rb->getId())==0表示空载不能访问
                 * 2. to工作台负载能访问 accessTo&(1<<(rb->getId()+LOAD_SHIFT_BIT))==0 负载不能访问
                 */
                // 环境判断
                if (from->isFree() || to->hasMaterial(from->getType()))
                {
                    continue;
                }
                // 规划判断
                if (from->getPlanProductStatus() == 1 || to->hasPlanMaterial(from->getType()))
                {
                    continue;
                }
                // 可访问性判断
                int accessFrom = accessMap[from->getMapRow()][from->getMapCol()];
                int accessTo = accessMap[to->getMapRow()][to->getMapCol()];
                if ((accessFrom & (1 << rb->getId())) == 0 || (accessTo & (1 << (rb->getId() + LOAD_SHIFT_BIT))) == 0)
                {
                    continue;
                }

                double distance = from->getDij()->getDistMap(false)[rb->getMapRow()][rb->getMapCol()] * 0.25;
                double receiveTaskFrame = distance / MAX_FORWARD_FRAME;

                // 接收时间小于生产时间，需要等待，直接放弃
                if (receiveTaskFrame < from->getRest())
                {
                    continue;
                }
                // 更新任务最快完成时间
                TaskChain *taskChain = chainPool->acquire();
                taskChain->set(receiveTaskFrame);
                taskChain->addTask(task);

                // 保存任务链
                // 检测taskChainQueueMap[rb]是否存在
                taskChainQueueMap[rb]->push(taskChain);
            }
        }
    }

    if (isQueueMapEmpty(nullptr))
    {
        return;
    }

    // 这里更新两次是因为最长链长为3，减去初始链长1, 所以两次
    updateTaskChain();
    updateTaskChain();
}
/*
 * @brief 复制source到target
 *
 *
 */
void Dispatcher::copyQueue(std::priority_queue<TaskChain *> *source, std::priority_queue<TaskChain *> *target)
{
    while (!target->empty())
    {
        target->pop();
    }

    while (!source->empty())
    {
        target->push(source->top());
        source->pop();
    }
}

void Dispatcher::updateTaskChain()
{
    for (auto rb : robotList)
    {
        if (isQueueMapEmpty(rb))
        {
            continue;
        }

        // 将旧的queue内容拷贝一份，避免在遍历的过程中热更新产生错误
        // auto temp = *taskChainQueueMap.find(rb);
        std::priority_queue<TaskChain *> *queue = (*taskChainQueueMap.find(rb)).second;
        copyQueue(queue, tempQueue);

        // 是否有更改
        while (!tempQueue->empty())
        {
            TaskChain *taskChain = tempQueue->top();
            tempQueue->pop();

            Task *lastTask = taskChain->getTask(-1);
            if (lastTask->getPostTaskList() == nullptr)
            {
                queue->push(taskChain);
                continue;
            }
            bool addPost = false;
            for (Task *postTask : *(lastTask->getPostTaskList()))
            {
                Workbench *postFrom = postTask->getFrom(), *postTo = postTask->getTo(), *lastFrom = lastTask->getFrom();
                // 未生产，直接访问下个后续任务 或者 lastTask生产的产品已经出现在产品格中
                // 假设我们能够维护好预测的原料格状态和生产格状态，那么在生成任务链中
                /*
                 * 环境应满足：
                 * 1. postFrom工作台必须已经投入生产: postFrom.isFree() true： 表示未生产
                 * 2. postTo工作台的原料格没被占用
                 *
                 * 规划应满足：
                 *
                 * 1. postFrom工作台产品格未被占据:postFrom.getPlanProductStatus() == 1 true表示被占据
                 * 2. postFrom工作台的规划原料格没被占用：postFrom.hasPlanMaterial(lastFrom.getType())
                 * true表示被占据
                 * 3. postTo工作台的规划原料格没被占用：postTo.hasPlanMaterial(postFrom.getType()) true表示被占据
                 *
                 * 访问应满足
                 * 1. to负载能访问
                 */
                // 环境判断
                if (postFrom->isFree() || postTo->hasMaterial(postFrom->getType()))
                {
                    continue;
                }
                // 规划判断
                if (postFrom->getPlanProductStatus() == 1 || postFrom->hasPlanMaterial(lastFrom->getType()) || postTo->hasPlanMaterial(postFrom->getType()))
                {
                    continue;
                }
                // 访问判断
                int accessTo = accessMap[postTo->getMapRow()][postTo->getMapCol()];
                if ((accessTo & (1 << (rb->getId() + LOAD_SHIFT_BIT))) == 0)
                {
                    continue;
                }

                // 开始生产, 如果生产剩余时间比机器人最快到达时间更久，说明会出现等待
                double frame = taskChain->getTotalFrame();
                if (postFrom->getType() == 7)
                {
                    int i = 0;
                }
                if (postFrom->getRest() > taskChain->getTotalFrame())
                {
                    continue;
                }

                // 获取完成任务时间
                double totalTime = taskChain->getTotalFrame() + postTask->getDist() / MAX_FORWARD_FRAME;

                addPost = true;
                // 更新任务最早完成时间，并把该任务加入到这条任务链中
                TaskChain *newTaskChain = chainPool->acquire();
                newTaskChain->set(taskChain);
                newTaskChain->addTask(postTask);

                // 保存
                queue->push(newTaskChain);
            }

            if (!addPost)
            {
                queue->push(taskChain);
            }
            else
            {
                chainPool->release(taskChain);
            }
        }
    }
}

/** 筛选空闲机器人 */
void Dispatcher::updateFreeBot()
{
    freeRobots.clear();
    for (Robot *rb : robotList)
    {
        if (rb->isFree())
        {
            freeRobots.push_back(rb);
        }
    }
}

/** 开始任务分配 */
void Dispatcher::dispatch()
{
    // 筛选空闲机器人，供后续使用
    updateFreeBot();

    // 有空闲，构建任务链并分配给机器人
    if (!freeRobots.empty())
    {
        // 构建任务链
        generateTaskChains();

        // 任务链为空
        if (isQueueMapEmpty(nullptr))
        {
            return;
        }

        // 分配任务链
        while (!isQueueMapEmpty(nullptr))
        {
            double max = 0.;
            Robot *receiver = nullptr;
            TaskChain *bindChain = nullptr;

            for (Robot *rb : robotList)
            {
                // 检测机器人对应queue是否为空
                if (isQueueMapEmpty(rb))
                {
                    continue;
                }

                std::priority_queue<TaskChain *> *queue = taskChainQueueMap[rb];

                // 进行分配，寻找所有可用的chain
                while (true)
                {
                    TaskChain *chain = queue->top();
                    if (chain == nullptr || queue->empty())
                    {
                        bindChain = nullptr;
                        receiver = rb;
                        break;
                    }

                    // 占用后将当前chain释放
                    if (chain->isOccupied())
                    {
                        chainPool->release(chain);
                        queue->pop();
                        continue;
                    }

                    if (chain->getProfit() > max)
                    {
                        receiver = rb;
                        bindChain = chain;
                        max = chain->getProfit();
                    }
                    break;
                }
            }

            // 没有可以分配的，将对应的chain全部释放
            if (bindChain == nullptr)
            {
                clearChainMap(receiver);
                continue;
            }

            bindChain->occupy();
            receiver->bindChain(bindChain);
            // 绑定后将原始chain删除
            clearChainMap(receiver);
        }
    }
}