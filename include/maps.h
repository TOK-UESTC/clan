#ifndef MAPS_H
#define MAPS_H

#include "const.h"

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
    /* 原始的map每个元素表示0.5x0.5, 经分析元素表示0.25对今后的路径规划有好处
       下面的convert025就是将原图转换成元素表示0.25的地图
    */
    char **convert025()
    {
    }
};

#endif