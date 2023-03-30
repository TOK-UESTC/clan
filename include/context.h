#ifndef CONTEXT_H
#define CONTEXT_H

#include <iostream>
#include <cstring>

/*
 * @brief 用于管理程序运行的上下文信息
 *
 *
 */
class Context
{
public:
    Context();  // 构造函数
    ~Context(); // 析构函数

    void init();   // 初始化地图程序
    void update(); // 与判题器交互
    void step();   // 发起决策过程

    // 辅助方法，直接在context定义
    void readLine();
    void printLine(char *out);
    void endStep();
};

#endif