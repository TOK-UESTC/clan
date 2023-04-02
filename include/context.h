#ifndef CONTEXT_H
#define CONTEXT_H

#include "includeAll.h"
#include <iostream>
/*
 * @brief 用于管理程序运行的上下文信息
 *
 *
 */
class Context
{

private:
    int leftFrame; // 剩余帧数
    char line[1024];
    int frameId = 0;
    int money = 0;
    std::vector<Workbench *> workbenchList;
    std::vector<Robot *> robotList;
    char **map05;       // 原图
    char **map025;      // 转换后地图
    int **mapRoadWidth; // 路宽图
    int **accessMap;    // 机器人可访问标志图
    Maps maps;          // 存放地图信息
public:
    // Context();  // 构造函数
    ~Context(); // 析构函数

    int getFrameId();

    void init();          // 初始化地图程序
    void update();        // 与判题器交互
    void step(bool init); // 发起决策过程

    // 辅助方法，直接在context定义
    void readLine();
    void printLine(const char *out);
    void printLine(const std::string &out);
    void printLineErr(const std::string &out);
    void endStep();
};

#endif