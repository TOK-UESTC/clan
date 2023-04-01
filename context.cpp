#include "include/context.h"

Context::Context()
{
}

Context::~Context() {}

void Context::init()
{
    int row = 0;
    double x, y;            // 地图坐标
    int workbenchCount = 0; // 工作台数量
    int robotCount = 0;     // 机器人数量
    char **m = maps.getMap();

    while (true)
    {
        readLine();

        // 地图数据读取完毕
        if (line[0] == 'O' && line[1] == 'K')
        {
            break;
        }

        // 坐标
        for (int col = 0; col < strlen(line); col++)
        {
            // 存储地图信息
            m[row][col] = line[col];
            // 第一行第一列为(0.25, 49.75)
            x = col * 0.5 + 0.25;
            y = 49.75 - row * 0.5;

            switch (line[col])
            {
            // 空地
            case '.':
            {
                break;
            }
            // 机器人
            case 'A':
            {
                Robot *robot = new Robot(robotCount++, x, y);
                robotList.push_back(robot);
                break;
            }
            // 障碍
            case '#':
            {
                break;
            }
            // 工作台
            default:
            {
                int workbenchType = line[col] - '0';
                Workbench *workbench = new Workbench(workbenchCount++, x, y, workbenchType);
                workbenchList.push_back(workbench);

                // 将同一型号的工作台放置到map中
                // if (workbenchTypeMap.containsKey(workbenchType))
                // {
                //     workbenchTypeMap.get(workbenchType).add(workbench);
                // }
                // else
                // {
                //     ArrayList<Workbench> storage = new ArrayList<>();
                //     storage.add(workbench);
                //     workbenchTypeMap.put(workbenchType, storage);
                // }
                break;
            }
            }
        }
        row++;
    }

    // for (int i = 0; i < sizeof(m) / sizeof(m[0]); i++)
    // {
    //     fprintf(stderr, "%s\n", m[i]);
    //     fflush(stderr);
    // }

    // sortRobotList.addAll(robotList);

    // for (int t = 1; t <= 9; t++)
    // {
    //     if (workbenchTypeMap.get(t) == null)
    //     {
    //         Const.workbenchMapper.put(t, 0);
    //         continue;
    //     }
    //     Const.workbenchMapper.put(t, workbenchTypeMap.get(t).size());
    // }

    // dispatcher = new Dispatcher(robotList, workbenchList, workbenchTypeMap, chainPool, statePool);

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
    (void)!scanf("%d %d\n", &frameId, &money);
    leftFrame = TOTAL_FRAME - frameId;

    // 更新工作台信息
    int k;
    (void)!scanf("%d\n", &k);
    for (Workbench *wb : workbenchList)
    {
        wb->update();
    }

    // 更新机器人信息
    for (Robot *rb : robotList)
    {
        rb->update();
    }
}

void Context::step(bool init)
{
    if (frameId == 0)
    {
        int i = 0;
    }
    if (init)
    {
        // dispatcher->dispatch();
        // for(Robot rb : robotList){
        //     rb->step();
        // }
    }
    else
    {
        // printLine
    }
    printf("%d\n", frameId);
    // 告知判题器操作结束
    endStep();
}

int Context::getFrameId()
{
    return frameId;
}

void Context::readLine()
{
    (void)!fgets(line, sizeof line, stdin);
}

void Context::printLine(char *out)
{
    printf("%s\n", out);
}

void Context::endStep()
{
    puts("OK");
    fflush(stdout);
}
