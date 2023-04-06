#include "include/includeAll.h"

Context::~Context()
{
    // 释放机器人内存
    for (Robot *rb : robotList)
    {
        delete rb;
    }

    // 释放工作台内存
    for (Workbench *wb : workbenchList)
    {
        delete wb;
    }

    // 释放地图内存
    maps.releaseMap(map05);
    maps.releaseMap(map025);
    maps.releaseMap(mapRoadWidth);
    maps.releaseMap(accessMap);

    // 释放map
    for (auto iter = workbenchTypeMap.begin(); iter != workbenchTypeMap.end(); iter++)
    {
        delete iter->second;
    }

    for (auto iter = workbenchIdTaskMap.begin(); iter != workbenchIdTaskMap.end(); iter++)
    {
        for (auto t : *(iter->second))
        {
            delete t;
        }
        delete iter->second;
    }
}

void Context::init()
{
    int row = 0;
    double x, y;            // 地图坐标
    int workbenchCount = 0; // 工作台数量
    int robotCount = 0;     // 机器人数量
    map05 = maps.getMap();

    while (true)
    {
        readLine();

        // 地图数据读取完毕
        if (line[0] == 'O' && line[1] == 'K')
        {
            break;
        }

        // 坐标
        for (int col = 0; col < strlen(line) - 1; col++)
        {
            // 存储地图信息
            map05[row][col] = line[col];
            // 第一行第一列为(0.25, 49.75)
            x = col * 0.5 + 0.25;
            y = 49.75 - row * 0.5;

            switch (line[col])
            {
            // 空地
            case '.':
                break;
            // 机器人
            case 'A':
            {
                Robot *robot = new Robot(robotCount++, x, y);
                robotList.push_back(robot);
                break;
            }
            // 障碍
            case '#':
                break;
            // 工作台
            default:
                int workbenchType = line[col] - '0';
                if (workbenchType != 8 && workbenchType != 9)
                {
                    workbenchIdTaskMap[workbenchCount] = new std::vector<Task *>();
                }
                Workbench *workbench = new Workbench(workbenchCount++, x, y, workbenchType);
                workbenchList.push_back(workbench);

                // 将同一型号的工作台放置到map中
                if (workbenchTypeMap.find(workbenchType) != workbenchTypeMap.end())
                {
                    workbenchTypeMap[workbenchType]->push_back(workbench);
                }
                else
                {
                    workbenchTypeMap[workbenchType] = new std::vector<Workbench *>();
                    workbenchTypeMap[workbenchType]->push_back(workbench);
                }
                break;
            }
        }

        row++;
    }
    // 获得转换后地图
    map025 = maps.convert025();
    accessMap = Maps::newMap(map025);
    // 计算访问权限图
    for (Robot *rb : robotList)
    {
        int row = ((int)((49.75 - rb->getPos().getY()) / 0.5)) * 2 + 1;
        int col = ((int)((rb->getPos().getX() - 0.25) / 0.5)) * 2 + 1;
        maps.accessible(map025, accessMap, row, col, rb->getId());
        // 绑定dijkstra, 机器人维护
        rb->setDij(new Dijkstra(accessMap));
    }

    // 对每个工作台检查可达性
    for (Workbench *wb : workbenchList)
    {
        maps.isWbAccessible(accessMap, wb->getMapRow(), wb->getMapCol());
    }

    // 获得路宽地图
    mapRoadWidth = maps.mapRoadWidth(map025);

    // 将地图写入log
    maps.writeMaptoFile("./log/map05.txt", map05);
    maps.writeMaptoFile("./log/map025.txt", map025);
    maps.writeMaptoFile("./log/mapRoadWidth.txt", mapRoadWidth);
    maps.writeMaptoFile("./log/accessMap.txt", accessMap);

    dispatcher = new Dispatcher(robotList, workbenchList, workbenchTypeMap, workbenchIdTaskMap, accessMap);
    // 搜索参数时停止pid更改
    // if (args.length == 7)
    // {
    //     return;
    // }

    // // 根据地图工作台情况，动态调整pid
    // for (Robot rb : robotList)
    // {
    //     rb.updatePID(workbenchCount);
    // }
}
void Context::update()
{
    // readLine();
    // 快速读入
    scanf("%d %d", &frameId, &money);
    getchar();
    leftFrame = TOTAL_FRAME - frameId;

    // 更新工作台信息
    int k;
    scanf("%d", &k);
    getchar();
    for (Workbench *wb : workbenchList)
    {
        wb->update();
    }

    // 更新机器人信息
    for (Robot *rb : robotList)
    {
        rb->update(leftFrame);
        rb->checkDeal();
    }

    readLine();
}

void Context::step(bool init)
{
    printf("%d\n", frameId);
    if (frameId == 950)
    {
        int i = 0;
    }
    if (init)
    {
        dispatcher->dispatch();
        for (Robot *rb : robotList)
        {
            rb->step();
        }
    }
    else
    {
        // printLine
        dispatcher->dispatch();
        for (Robot *rb : robotList)
        {
            rb->step();
        }
        for (Robot *rb : robotList)
        {
            for (Action *action : rb->getActions())
            {
                printLine(action->toString(rb->getId()));
            }
        }
    }

    // 告知判题器操作结束
    printf("OK\n");
    fflush(stdout);
    // endStep();
}

int Context::getFrameId()
{
    return frameId;
}

void Context::readLine()
{
    fgets(line, sizeof line, stdin);
}

void Context::printLine(const char *out)
{
    printf("%s\n", out);
}

void Context::printLine(const std::string &out)
{
    printf("%s\n", out.c_str());
}
// 打印到标准错误
void Context::printLineErr(const std::string &out)
{
    fprintf(stderr, "%s\n", out.c_str());
    fflush(stderr);
}
void Context::endStep()
{
    puts("OK");
    fflush(stdout);
}
