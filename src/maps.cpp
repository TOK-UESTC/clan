#include "includeAll.h"

/*
 * @brief 地图转换
 * 原始地图: 每个元素代表一个0.5x0.5的方格
 * 转换后地图: 每个元素代表两条线的交点位置状态
 *
 * 返回的地图交给使用方释放
 */
char **Maps::convert025()
{
    char **map025 = new char *[MAP_HEIGHT * 2 + 1];
    for (int i = 0; i < MAP_HEIGHT * 2 + 1; i++)
    {
        map025[i] = new char[MAP_WIDTH * 2 + 1];
    }
    // 初始化边界墙
    for (int i = 0; i < MAP_HEIGHT * 2 + 1; i++)
    {
        map025[i][0] = '#';
        map025[i][MAP_WIDTH * 2] = '#';
    }
    for (int j = 0; j < MAP_WIDTH * 2 + 1; j++)
    {
        map025[0][j] = '#';
        map025[MAP_HEIGHT * 2][j] = '#';
    }

    int col = sizeof(map05[0]) / sizeof(map05[0][0]);
    int row = sizeof(map05) / sizeof(map05[0]);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            map025[2 * i + 1][2 * j + 1] = map05[i][j] == 'A' ? '.' : map05[i][j];

            if (j == col - 1)
            {
                map025[2 * i + 1][2 * j + 2] = '#';
            }
            else
            {
                map025[2 * i + 1][2 * j + 2] = (map05[i][j] == '#' || map05[i][j + 1] == '#') ? '#' : '.';
            }

            if (i == row - 1)
            {
                map025[2 * i + 2][2 * j + 1] = '#';
            }
            else
            {
                map025[2 * i + 2][2 * j + 1] = (map05[i][j] == '#' || map05[i + 1][j] == '#') ? '#' : '.';
            }

            if (j == col - 1 || i == row - 1)
            {
                map025[2 * i + 2][2 * j + 2] = '#';
            }
            else
            {
                map025[2 * i + 2][2 * j + 2] = (map05[i][j] == '#' || map05[i + 1][j] == '#' || map05[i][j + 1] == '#' || map05[i + 1][j + 1] == '#') ? '#' : '.';
            }
        }
    }

    return map025;
}