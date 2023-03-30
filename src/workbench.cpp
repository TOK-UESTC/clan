#include "workbench.h"

class Workbench
{
private:
    int id;                 // 工作台ID
    int type;               // 工作台类型 [1-9]
    int rest;               // 生产剩余时间 -1表示没有生产；0表示生产因输出格满而受阻塞；>=0 表示剩余生产帧数
    int productStatus;      // 产品格状态 0 表示无 1 表示有
    int planProductStatus;  // 规划产品格状态，由选手程序维护；不仅包含环境目前的状态，还包括预测状态。
    int materialStatus;     // 原材料格状态；二进制为表示，例如 48(110000),表示拥有物品4和5
    int lastMaterialStatus; //
    int planMaterialStatus; // 规划原材料格状态；不仅包含环境目前的状态，还包括预测状态。
    Vec pos;                // 工作台位置

public:
    Workbench(double x, double y, int type, int id)
    {
        this->type = type;
        this->pos.set(x, y);
        this->id = id;
        this->rest = getRestTime(type);
        this->productStatus = 0;
        this->lastMaterialStatus = 0;
        this->planProductStatus = 0;
        this->planMaterialStatus = 0;
    }

    /* 初始化时返回工作台剩余生产时间 */
    int getRestTime(int type)
    {
        if (type == 1 || type == 2 || type == 3)
        {
            return 50;
        }
        return -1;
    }

    void update(std::vector<std::string> info)
    {
        this->rest = std::stoi(info[3]);
    }
};