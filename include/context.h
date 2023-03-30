#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <cstring>
#include <vector>

#include "const.h"
#include "workbench.h"
#include "robot.h"
#include "maps.h"

/*
 * @brief 用于管理程序运行的上下文信息
 *
 *
 */
class Context
{

private:
    char line[1024];
    int frameId = 0;
    int money = 0;
    int leftFrame = 0;
    Maps maps; // 存放地图信息
    std::vector<Workbench> workbenchList;
    std::vector<Robot> robotList;

public:
    Context(){};  // 构造函数
    ~Context(){}; // 析构函数

    int getFrameId();

    void init();          // 初始化地图程序
    void update();        // 与判题器交互
    void step(bool init); // 发起决策过程

    // 辅助方法，直接在context定义
    void readLine();
    void printLine(char *out);
    void endStep();
};

#endif