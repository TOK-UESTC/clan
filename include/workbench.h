#ifndef WORKBENCH_H
#define WORKBENCH_H

#include "includeAll.h"

/*
 * @brief 工作台代理类
 *
 *
 */

class Workbench
{
private:
    int id = 0;                   // 工作台ID
    int type = 0;                 // 工作台类型 [1-9]
    int rest = 0;                 // 生产剩余时间 -1表示没有生产；0表示生产因输出格满而受阻塞；>=0 表示剩余生产帧数
    int productStatus = 0;        // 产品格状态 0 表示无 1 表示有
    int planProductStatus = 0;    // 规划产品格状态，由选手程序维护；不仅包含环境目前的状态，还包括预测状态。
    int materialStatus = 0;       // 原材料格状态；二进制为表示，例如 48(110000),表示拥有物品4和5
    int lastMaterialStatus = 0;   // 上一帧原料状态
    int planMaterialStatus = 0;   // 规划原材料格状态；不仅包含环境目前的状态，还包括预测状态。
    Vec pos;                      // 工作台位置
    Dijkstra *dijkstra = nullptr; // 寻找最短路径，用于生成任务

public:
    Workbench(int id, double x, double y, int type);
    ~Workbench();

    int getRestTime(int type);
    Vec *getPos();
    void newDij(int **accessMap);

    void update();
    bool hasMaterial(int type);
    bool hasPlanMaterial(int type);
    void updatePlanMaterialStatus(int type, bool Sell);
    void setPlanProductStatus(int planProductStatus);
    int getPlanProductStatus();
    bool isFree();
    bool isReady();
    int getType();
    int getRest();
    int getId();
    bool isBlocked();
    int getWorkbenchIdx();
    int getMapRow();
    int getMapCol();
    Dijkstra *getDij();

    bool operator==(const Workbench &o) const;
    size_t hash() const;
};

#endif