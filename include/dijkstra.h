#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "includeAll.h"

class Dijkstra
{
private:
    int **accessMap; // 访问图
    bool **visited;
    double **dist; // 距离图
    int row, col;  // 地图属性
public:
    Dijkstra(int **accessMap)
    {
        this->accessMap = accessMap;
        this->row = _msize(accessMap) / 8;
        this->col = _msize(accessMap[0]) / sizeof(accessMap[0][0]);
    }

    void search(int r, int c, bool loaded, int id); // 从起始点进行搜素，填充访问图
    std::list<Vec *> *getKnee(int r, int c);        // 根据访问图获取拐点，输入为目标位置

    bool validCoord(int r, int c);                       // 检查坐标是否合法
    bool checkAccess(int r, int c, bool loaded, int id); // 根据机器人状态判断是否可达

    double **getDistMap();
    void freeDist()
    {
        int row = _msize(dist) / 8;
        for (int i = 0; i < row; i++)
        {
            delete[] dist[i];
        }
        delete[] dist;
    }
    int **getAccessMap()
    {
        return accessMap;
    }
};

#endif
