#include "context.h"

class Context
{
private:
    char line[1024];

public:
    Context()
    {
    }

    ~Context()
    {
    }

    void init()
    {
        int row = 0;
        double x, y;            // 地图坐标
        int workbenchCount = 0; // 工作台数量
        int robotCount = 0;     // 机器人数量

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
                    // Robot robot = new Robot(
                    //     new Coordinate(x, y),
                    //     robotList, args, statePool, fragPool, coordPool, pidPool, robotCount++);
                    // robotList.add(robot);
                    break;
                // 工作台
                default:
                    // int workbenchType = line[col] - '0';
                    // Workbench workbench = new Workbench(new Vec(x, y), workbenchType, workbenchCount++);
                    // workbenchList.add(workbench);

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
            row++;
        }

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

    void readLine()
    {
        fgets(line, sizeof line, stdin);
    }

    void printLine(char *out)
    {
        std::cout << out << std::endl;
    }

    void endStep()
    {
        puts("OK");
        fflush(stdout);
    }
};