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

    char **convert025(); // 地图转换, 详情见实现
};

#endif