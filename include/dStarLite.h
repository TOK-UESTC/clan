#ifndef D_STAR_LITE_H
#define D_STAR_LITE_H

#include "includeAll.h"

// 排序用比较函数
static bool cmp(const std::pair<double, double> &a, const std::pair<double, double> &b)
{
    if (a.first != b.first)
    {
        return a.first < b.first;
    }
    else
    {
        return a.second < b.second;
    }
}

static bool cmp_map_value(const std::pair<std::pair<int, int>, std::pair<double, double>> &a, const std::pair<std::pair<int, int>, std::pair<double, double>> &b)
{
    return cmp(a.second, b.second);
}

class DStarLite
{
private:
    int **accessMap; // 可访问性图
    int row, col;    // 地图属性

    double **g;                                                 // 当前点到终点的实际代价
    double **rhs;                                               // 一个点从父代节点到达这个点的最小代价
    std::map<std::pair<int, int>, std::pair<double, double>> U; // 优先队列，用于存储待搜索的节点

    double km;  // 原方法中的km
    int sr, sc; // 起始位置，行列
    int tr, tc; // 目标位置，行列

    int count; // 计数
public:
    DStarLite(int **accessMap, int sr, int sc, int tr, int tc)
    {
        this->sr = sr;
        this->sc = sc;
        this->tr = tr;
        this->tc = tc;

        this->accessMap = accessMap;
        this->row = MAP025;
        this->col = MAP025;

        this->g = Maps::newDMap(accessMap, 9999);
        this->rhs = Maps::newDMap(accessMap, 9999);
        this->rhs[tr][tc] = 0;
        this->km = 0;

        this->U[std::pair<int, int>(tr, tc)] = calculateKey(tr, tc);
        this->count = 0;
    }

    void search(bool loaded, int id);        // 从起始点进行搜素，填充访问图
    std::list<Vec *> *getKnee(int r, int c); // 根据访问图获取拐点，输入为目标位置

    bool validCoord(int r, int c);                       // 检查坐标是否合法
    bool checkAccess(int r, int c, bool loaded, int id); // 根据机器人状态判断是否可达
    double cost(int r, int c, int nr, int nc);           // 移动代价

    double **getG();

    std::pair<double, double> calculateKey(int r, int c); // 计算排序用key
    void updateVertex(int r, int c, bool loaded, int id); // 更新节点
    double h(int sr, int sc, int tr, int tc);             // 计算当前点到起点的估计值

    double **getDistMap();
    int **getAccessMap()
    {
        return accessMap;
    }
};

#endif