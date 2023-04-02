#include "include/includeAll.h"

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
int **Maps::mapRoadWidthHV(char **map, bool isHorizon)
{
    int row = _msize(map) / 8;
    int col = _msize(map[0]);
    if (isHorizon)
    {
        int **mapRoadWidthH = new int *[row];
        for (int i = 0; i < row; i++)
        {
            mapRoadWidthH[i] = new int[col];
        }
        for (int i = 0; i < row; i++)
        {
            int left = 0, right = 0;
            while (right != col - 1)
            {
                right++;
                if (map[i][right] != '#')
                {
                    continue;
                }

                int width = right - left;
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
        int **mapRoadWidthV = new int *[row];
        for (int i = 0; i < row; i++)
        {
            mapRoadWidthV[i] = new int[col];
        }
        for (int j = 0; j < col; j++)
        {
            int left = 0, right = 0;
            while (right != row - 1)
            {
                right++;
                if (map[right][j] != '#')
                {
                    continue;
                }

                int width = right - left;
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

int **Maps::mapRoadWidth(char **map)
{

    int **mapH = mapRoadWidthHV(map, true);
    int **mapV = mapRoadWidthHV(map, false);
    int row = _msize(map) / 8;
    int col = _msize(map[0]);

    int **mapRoadWidth = new int *[row];
    for (int i = 0; i < row; i++)
    {
        mapRoadWidth[i] = new int[col];
    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            mapRoadWidth[i][j] = mapH[i][j] < mapV[i][j] ? mapH[i][j] : mapV[i][j];
        }
    }
    // 释放临时路宽图
    releaseMap(mapH);
    releaseMap(mapV);

    return mapRoadWidth;
}

/*
 * @brief 将map数据写入file文件检查
 */
void Maps::writeMaptoFile(const char *file, char **map)
{
    int row = _msize(map) / 8;
    int col = _msize(map[0]);

    std::ofstream outfile;
    // 打开文件
    outfile.open(file);
    // 写入文件
    for (int i = 0; i < row; i++)
    {
        // 写入前刷新缓冲区
        outfile.flush();
        for (int j = 0; j < col; j++)
        {
            outfile << map[i][j];
        }
        outfile << std::endl;
        // 写入后刷新缓冲区
        outfile.flush();
    }
    // 关闭文件
    outfile.close();
}

void Maps::writeMaptoFile(const char *file, int **map)
{
    int row = _msize(map) / 8;
    int col = _msize(map[0]) / (sizeof(map[0][0]));

    std::ofstream outfile;
    // 打开文件
    outfile.open(file);
    // 写入文件
    for (int i = 0; i < row; i++)
    {
        // 写入前刷新缓冲区
        outfile.flush();
        for (int j = 0; j < col; j++)
        {
            outfile << std::setw(4) << map[i][j];
        }
        outfile << std::endl;
        // 写入后刷新缓冲区
        outfile.flush();
    }
    // 关闭文件
    outfile.close();
}
/*
 * @brief 释放堆内存
 */
void Maps::releaseMap(char **map)
{
    int row = _msize(map) / 8;
    int col = _msize(map[0]);
    for (int i = 0; i < row; i++)
    {
        delete[] map[i];
    }
    delete[] map;
}

void Maps::releaseMap(int **map)
{
    int row = _msize(map) / 8;
    int col = _msize(map[0]);
    for (int i = 0; i < row; i++)
    {
        delete[] map[i];
    }
    delete[] map;
}

/*
 * @brief 机器人可访问标志图 1000000, 表示机器人4负载可访问，01000001：表示机器人3负载可访问，机器人1空载可访问
 * @param map: 用于路径规划的地图
 * @param accessMap: 用于标记可访问性的地图
 * @param r: 机器人所在行,map025地图
 * @param c: 机器人所在列,map025地图
 * @param id: 机器人id
 */
void Maps::accessible(char **map, int **accessMap, int r, int c, int id)
{
    // 置0
    int row = _msize(accessMap) / 8;
    int col = _msize(accessMap[0]) / (sizeof(accessMap[0][0]));
    bool **visit = new bool *[row];
    for (int i = 0; i < row; i++)
    {
        visit[i] = new bool[col];
        memset(visit[i], false, _msize(visit[i]));
    }

    std::queue<int> qx; // 存放遍历点x坐标
    std::queue<int> qy; // 存放遍历点y坐标

    // 将机器人位置加入队列
    qx.push(r);
    qy.push(c);

    int dir[][2] = {
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0, -1},
        {0, 1},
        {1, -1},
        {1, 0},
        {1, 1}};

    visit[r][c] = true;
    while (!qx.empty())
    {
        // 当前的x,y坐标
        int cx = qx.front();
        int cy = qy.front();
        qx.pop();
        qy.pop();
        if (isAccessible(map, cx, cy, true))
        {
            accessMap[cx][cy] = accessMap[cx][cy] | (1 << (LOAD_SHEFT_BIT + id));
        }
        if (isAccessible(map, cx, cy, false))
        {
            accessMap[cx][cy] = accessMap[cx][cy] | (1 << id);
        }

        // 将下一次节点加入队列
        for (int i = 0; i < 8; i++)
        {
            int nx = dir[i][0] + cx;
            int ny = dir[i][1] + cy;
            if (map[nx][ny] == '#' || visit[nx][ny])
            {
                continue;
            }
            visit[nx][ny] = true;
            qx.push(nx);
            qy.push(ny);
        }
    }
    // 释放bool数组
    for (int i = 0; i < row; i++)
    {
        delete[] visit[i];
    }
    delete[] visit;
    
}

/*
 * @brief 释放堆内存
 * @param map: 用于路径规划的地图，默认map025
 * @param x: 横坐标位置
 * @param y: 纵坐标位置
 * @param isLoad: 负载或空载条件, true: 表示负载
 */

bool Maps::isAccessible(char **map, int x, int y, bool isLoad)
{
    int maxX = _msize(map) / 8 - 1;
    int maxY = _msize(map[0]) - 1;
    // 如果该点是障碍，那么该点不可访问
    if (map[x][y] == '#')
    {
        return false;
    }
    // 下面分空载和负载讨论
    if (isLoad)
    {
        int dir[][2] = {
            {-1, -1},
            {-2, 0},
            {-1, 1},
            {0, -2},
            {0, 2},
            {1, -1},
            {2, 0},
            {1, 1}};
        // 负载情况下
        if (map[x][y] == '.')
        {
            // 如果该点是空地, 那么2格内不包含障碍
            for (int i = 0; i < 8; i++)
            {
                int tx = dir[i][0] + x;
                int ty = dir[i][1] + y;
                // 如果靠近边缘
                if (tx <= 0 || tx >= maxX || ty <= 0 || ty >= maxY)
                {
                    return false;
                }

                if (map[tx][ty] == '#')
                {
                    return false;
                }
            }
        }
        else
        {
            if (map[x - 1][y - 1] == '#' && map[x + 1][y + 1] == '#')
            {
                return false;
            }

            if (map[x - 1][y + 1] == '#' && map[x + 1][y - 1] == '#')
            {
                return false;
            }
        }
    }
    else
    {
        int dir[][2] = {
            {-1, -1},
            {-1, 0},
            {-1, 1},
            {0, -1},
            {0, 1},
            {1, -1},
            {1, 0},
            {1, 1}};
        // 空载情况下
        if (map[x][y] == '.')
        {
            for (int i = 0; i < 8; i++)
            {
                int tx = dir[i][0] + x;
                int ty = dir[i][1] + y;
                if (map[tx][ty] == '#')
                {
                    return false;
                }
            }
        }
        else
        {
            if (map[x][y - 1] == '#' && map[x][y + 1] == '#')
            {
                return false;
            }

            if (map[x - 1][y] == '#' && map[x + 1][y] == '#')
            {
                return false;
            }
        }
    }

    return true;
}
