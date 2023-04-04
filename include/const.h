#ifndef CONST_H
#define CONST_H

/*
 * @brief 程序用到的常量和枚举类型
 *
 *
 */

#define PI 3.1415926535897932384626                               // 圆周率
#define DURATION_OF_GAME 300                                      // 比赛时长，单位: s
#define ROBOT_NUMBER 4                                            // 机器人数量
#define FRAME_PER_SECOND 50                                       // 每秒帧数
#define INIT_FUND 200000                                          // 初始资金
#define ROBOT_IN_WORKBENCH 0.4                                    // 机器人在工作台范围判断， 单位 m
#define ROBOT_RADIUS_UNLOAD 0.45                                  // 机器人空载半径
#define ROBOT_RADIUS_LOADED 0.53                                  // 机器人负载半径
#define ROBOT_DENSITY 20                                          // 机器人密度， 20kg/m^2
#define MAX_FORWARD_VELOCITY 6                                    // 机器人最大前进速度
#define MAX_BACKWARD_VELOCITY -2                                  // 机器人最大后退速度
#define MAX_ANGULAR_VELOCITY PI                                   // 机器人最大角速度
#define MAX_DISTANCE 100                                          // 最大距离100， 大于50*sqrt(2)即可
#define MAP_LENGTH 50.0                                           // 地图长度
#define TOTAL_FRAME DURATION_OF_GAME *FRAME_PER_SECOND            // 总时长
#define MAX_FORWARD_FRAME MAX_FORWARD_VELOCITY / FRAME_PER_SECOND // 机器人最大前进速度/frame
#define MAP_HEIGHT 100                                            // 字符地图高度
#define MAP_WIDTH 100                                             // 字符地图宽度
#define LOAD_SHIFT_BIT 4                                          // 机器人在load的情况下，可访问标志符向左移动LOAD_SHIFT_BIT位

static int unloadDir[][2] = {
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1},
    {1, -1},
    {-1, -1},
    {-1, 1},
    {1, 1}};

static int loadedDir[][2] = {
    {-1, -1},
    {-2, 0},
    {-1, 1},
    {0, -2},
    {0, 2},
    {1, -1},
    {2, 0},
    {1, 1}};

#endif