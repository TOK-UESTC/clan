#ifndef MAPS_H
#define MAPS_H

#include "includeAll.h"

/*
 * @brief 管理比赛地图
 *
 *
 */

class Maps
{

private:
    char **map05; // 最原始的地图, 每个elements代表0.5x0.5

public:
    Maps()
    {
        map05 = new char *[MAP_HEIGHT];
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            map05[i] = new char[MAP_WIDTH];
        }
    }

    ~Maps()
    {
        for (int i = 0; i < MAP_HEIGHT; i++)
        {
            delete[] map05[i];
        }
        delete[] map05;
    }

    char **getMap()
    {
        return map05;
    }

    static int **newMap(char** map)
    {
        int row = _msize(map) / 8;
        int col = _msize(map[0]);
        int **newMap = new int *[row];
        for (int i = 0; i < row; i++)
        {
            newMap[i] = new int[col];
            memset(newMap[i], 0, _msize(newMap[i]));
        }
        return newMap;
    }

    char **convert025();                                                 // 地图转换, 详情见实现
    void accessible(char **map, int **accessMap, int x, int y, int id); // 得到地图上的元素可访问性
    bool isAccessible(char **map, int x, int y, bool isLoad);           // map上x,y处在load or unload情况下是否可访问
    int **mapRoadWidthHV(char **map, bool isHorizon);                    // 得到地图元素的水平或垂直路宽
    int **mapRoadWidth(char **map);                                      // 得到地图元素的
    void writeMaptoFile(const char *file, char **map);                   // 将map数据写入file文件
    void writeMaptoFile(const char *file, int **map);

    void releaseMap(char **map); // 释放地图
    void releaseMap(int **map);
};

#endif