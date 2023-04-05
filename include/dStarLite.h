#ifndef D_STAR_LITE_H
#define D_STAR_LITE_H

#include "includeAll.h"

class SearchNode
{
private:
    int r, c;
    double key1, key2;

public:
    SearchNode(int r, int c, double key1, double key2)
    {
        this->r = r;
        this->c = c;
        this->key1 = key1;
        this->key2 = key2;
    }

    // 用于优先队列的比较
    bool operator<(const SearchNode &o) const
    {
        if (key1 == o.key1)
        {
            return key2 < o.key2;
        }
        else
        {
            return key1 < o.key1;
        }
    }

    int getR()
    {
        return r;
    }

    int getC()
    {
        return c;
    }
};

class DStarLite
{
private:
    int **accessMap; // 可访问性图
    int row, col;    // 地图属性

    double **g;   // 当前点到终点的实际代价
    double **rhs; // 一个点从父代节点到达这个点的最小代价

    double k;   // 原方法中的km
    int sr, sc; // 起始位置，行列
    int tr, tc; // 目标位置，行列
public:
    DStarLite(int **accessMap)
    {
        this->accessMap = accessMap;
        this->row = _msize(accessMap) / 8;
        this->col = _msize(accessMap[0]) / sizeof(accessMap[0][0]);
    }

    void search(int r, int c, bool loaded, int id); // 从起始点进行搜素，填充访问图
    std::list<Vec *> *getKnee(int r, int c);        // 根据访问图获取拐点，输入为目标位置

    bool validCoord(int r, int c);                       // 检查坐标是否合法
    bool checkAccess(int r, int c, bool loaded, int id); // 根据机器人状态判断是否可达

    void calculateKey(int r, int c);          // 计算排序用key
    double h(int sr, int sc, int tr, int tc); // 计算当前点到起点的估计值

    double **getDistMap();
    int **getAccessMap()
    {
        return accessMap;
    }
};

#endif