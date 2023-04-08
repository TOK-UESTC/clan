#ifndef CONTEXT_H
#define CONTEXT_H

#include "includeAll.h"

/*
 * @brief 用于管理程序运行的上下文信息
 *
 *
 */
class Context
{

private:
    int fastDebug = 0;
    char line[200];                                                       // 读取缓冲区
    int frameId = 0;                                                      // 当前帧数
    int money = 0;                                                        // 当前金额
    char **map05;                                                         // 原图
    char **map025;                                                        // 转换后地图
    int **mapRoadWidth;                                                   // 路宽图
    int **accessMap;                                                      // 机器人可访问标志图
    Maps maps;                                                            // 存放地图信息
    std::vector<Workbench *> workbenchList;                               // 工作台列表
    std::vector<Robot *> robotList;                                       // 机器人列表
    std::vector<Robot *> sortedRobot;                                     // 机器人根据queue排序
    std::unordered_map<int, std::vector<Workbench *> *> workbenchTypeMap; // {工作台类型:工作台列表}字典
    std::unordered_map<int, std::vector<Task *> *> workbenchIdTaskMap;    // {工作台id:该id工作台发出的任务列表}字典
    Dispatcher *dispatcher;                                               // 调度器
public:
    // Context();  // 构造函数
    ~Context(); // 析构函数

    int getFrameId();

    void init();   // 初始化地图程序
    void update(); // 与判题器交互
    void step();   // 发起决策过程

    // 辅助方法，直接在context定义
    void readLine();
    void printLine(const char *out);
    void printLine(const std::string &out);
    void printLineErr(const std::string &out);
    void endStep();
};

#endif