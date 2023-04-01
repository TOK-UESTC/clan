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
        // map025[i][MAP_WIDTH * 2] = '#';
    }
    for (int j = 0; j < MAP_WIDTH * 2 + 1; j++)
    {
        map025[0][j] = '#';
        // map025[MAP_HEIGHT * 2][j] = '#';
    }

    int col = _msize(map05[0]);
    int row = _msize(map05) / 8;
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

/*
 * @brief 获取给定地图的通道宽度
 * @param map: 给定地图，默认是map025
 * @param isHorizon: true 计算水平方向的宽度 false 计算垂直方向的宽度
 *
 * @return 返回的地图交给使用方释放
 */
int8_t **Maps::mapRoadWidth(char **map, bool isHorizon)
{
    int col = _msize(map[0]);
    int row = _msize(map) / 8;
    if (isHorizon)
    {
        int8_t **mapRoadWidthH = new int8_t *[row];
        for (int i = 0; i < row; i++)
        {
            mapRoadWidthH[i] = new int8_t[col];
        }
        for (int i = 0; i < row; i++)
        {
            int left = 0, right = 0;
            while (right != col)
            {
                right++;
                if (map[i][right] != '#')
                {
                    right++;
                    continue;
                }

                int8_t width = right - left;
                if (width == 1)
                {
                    // 相邻两个点都在障碍上，那么之间也在障碍上
                    mapRoadWidthH[i][left] = 0;
                    mapRoadWidthH[i][left + 1] = 0;
                }
                else
                {
                    mapRoadWidthH[i][left] = 0;
                    for (int k = left + 1; k < right; k++)
                    {
                        mapRoadWidthH[i][k] = width;
                    }
                    mapRoadWidthH[i][right] = 0;
                }
                left = right;
            }
        }
        return mapRoadWidthH;
    }
    else
    {
        int8_t **mapRoadWidthV = new int8_t *[row];
        for (int i = 0; i < row; i++)
        {
            mapRoadWidthV[i] = new int8_t[col];
        }
        for (int j = 0; j < col; j++)
        {
            int left = 0, right = 0;
            while (right != row)
            {
                right++;
                if (map[right][j] != '#')
                {
                    right++;
                    continue;
                }

                int8_t width = right - left;
                if (width == 1)
                {
                    // 相邻两个点都在障碍上，那么之间也在障碍上
                    mapRoadWidthV[left][j] = 0;
                    mapRoadWidthV[left + 1][j] = 0;
                }
                else
                {
                    mapRoadWidthV[left][j] = 0;
                    for (int k = left + 1; k < right; k++)
                    {
                        mapRoadWidthV[k][j] = width;
                    }
                    mapRoadWidthV[right][j] = 0;
                }
                left = right;
            }
        }
        return mapRoadWidthV;
    }
}
