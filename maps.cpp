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

    int col = MAP05;
    int row = MAP05;
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
    int row = MAP025;
    int col = MAP025;
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
                        mapRoadWidthH[i][k] = (k - left) < (right - k) ? (k - left) : (right - k);
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
                        mapRoadWidthV[k][j] = (k - left) < (right - k) ? (k - left) : (right - k);
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
    int row = MAP025;
    int col = MAP025;

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
    int row = MAP025;
    int col = MAP025;

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

void Maps::writeMaptoFile(const char *file, double **map)
{
    int row = MAP025;
    int col = MAP025;

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
            outfile << std::setw(10) << map[i][j];
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
    int row = MAP025;
    int col = MAP025;

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
void Maps::releaseMap(char **map, int row)
{
    for (int i = 0; i < row; i++)
    {
        delete[] map[i];
    }
    delete[] map;
}

void Maps::releaseMap(int **map)
{
    int row = MAP025;
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
    int row = MAP025;
    int col = MAP025;
    bool **visit = new bool *[row];
    std::queue<int> qx; // 存放遍历点x坐标
    std::queue<int> qy; // 存放遍历点y坐标
    /* 下面空载搜索 */
    for (int i = 0; i < row; i++)
    {
        visit[i] = new bool[col];
        memset(visit[i], false, MAP025);
    }
    // 将机器人位置加入队列
    qx.push(r);
    qy.push(c);
    // 初始化起点可访问性
    accessMap[r][c] = accessMap[r][c] | (1 << id);
    visit[r][c] = true;
    while (!qx.empty())
    {
        // 当前的x,y坐标
        int cx = qx.front();
        int cy = qy.front();
        qx.pop();
        qy.pop();
        if (isAccessible(map, accessMap, cx, cy, false))
        {
            accessMap[cx][cy] = accessMap[cx][cy] | (1 << id);
        }

        // 将下一次节点加入队列
        for (int i = 0; i < 8; i++)
        {
            int nx = unloadDir[i][0] + cx;
            int ny = unloadDir[i][1] + cy;
            if (map[nx][ny] == '#' || visit[nx][ny])
            {
                continue;
            }
            visit[nx][ny] = true;
            qx.push(nx);
            qy.push(ny);
        }
    }

    /* 下面负载搜索 */
    for (int i = 0; i < row; i++)
    {
        visit[i] = new bool[col];
        memset(visit[i], false, MAP025);
    }
    // 将机器人位置加入队列
    qx.push(r);
    qy.push(c);
    // 初始化起点可访问性
    accessMap[r][c] = accessMap[r][c] | (1 << (LOAD_SHIFT_BIT + id));
    visit[r][c] = true;
    while (!qx.empty())
    {
        // 当前的x,y坐标
        int cx = qx.front();
        int cy = qy.front();
        qx.pop();
        qy.pop();
        if (isAccessible(map, accessMap, cx, cy, true))
        {
            accessMap[cx][cy] = accessMap[cx][cy] | (1 << (LOAD_SHIFT_BIT + id));
        }

        // 将下一次节点加入队列
        for (int i = 0; i < 8; i++)
        {
            int nx = unloadDir[i][0] + cx;
            int ny = unloadDir[i][1] + cy;
            if (map[nx][ny] == '#' || visit[nx][ny] || !isAccessible(map, accessMap, cx, cy, true))
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
// 从当前的点开始，检查是否出现甬道情况
void Maps::isWbAccessible(int **accessMap, int r, int c)
{
    // 从当前坐标出发，使用BFS检测最近的负载可访问点
    // 如果说最近的负载可访问点距离当前点的距离大于5，那么当前点就是甬道
    int minR = r;
    int minC = c;
    // bfs搜索,
    std::queue<int> qx_init; // 存放遍历点坐标
    std::queue<int> qy_init; // 存放遍历点坐标
    // 将机器人位置加入队列
    qx_init.push(r);
    qy_init.push(c);

    // 标记已经访问过的点,使用数组
    std::unordered_map<int, std::unordered_set<int>> visited;

    // TDOD:
    int id = 0;

    // 标记起点
    visited[r].insert(c);
    int len = 0;
    // 开始搜索
    while (!qx_init.empty() && len < 5)
    {
        int size = qx_init.size();

        for (int i = 0; i < size; i++)
        {
            // 取出队首元素
            int curr_x = qx_init.front();
            int curr_y = qy_init.front();
            qx_init.pop();
            qy_init.pop();

            // 检查是否是目标点
            if ((accessMap[curr_x][curr_y] & (1 << (LOAD_SHIFT_BIT + id))) != 0)
            {
                minR = curr_x;
                minC = curr_y;
                break;
            }

            // 遍历所有方位
            for (int j = 0; j < 8; j++)
            {
                int nr = unloadDir[j][0] + curr_x;
                int nc = unloadDir[j][1] + curr_y;

                // 检查地址合法性,同时检查是否已经访问过
                if (!validCoord(nr, nc) && visited[nr].find(nc) != visited[nr].end())
                {
                    continue;
                }

                // 检查非负载情况下是否可以经过
                if (accessMap[nr][nc] & 0b1111 == 0)
                {
                    continue;
                }

                // 将当前点加入队列
                qx_init.push(nr);
                qy_init.push(nc);

                // 标记已经访问过的点
                visited[nr].insert(nc);
            }
        }
        ++len;
    }
    if (len >= 5)
    {
        // 修改访问过的点的可访问性为不可访问
        for (auto it = visited.begin(); it != visited.end(); it++)
        {
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                accessMap[it->first][*it2] = 0;
            }
        }
    }
}
bool Maps::validCoord(int r, int c)
{
    // 检查位置是否合法
    if (r < 0 || r >= MAP025 || c < 0 || c >= MAP025)
    {
        return false;
    }

    return true;
}

/*
 * @brief 释放堆内存
 * @param map: 用于路径规划的地图，默认map025
 * @param x: 横坐标位置
 * @param y: 纵坐标位置
 * @param isLoad: 负载或空载条件, true: 表示负载
 */

bool Maps::isAccessible(char **map, int **accessMap, int x, int y, bool isLoad)
{
    int maxX = MAP025 - 1;
    int maxY = MAP025 - 1;
    // 如果该点是障碍，那么该点不可访问
    if (map[x][y] == '#')
    {
        return false;
    }
    // 下面分空载和负载讨论
    if (isLoad)
    {
        // 负载情况下
        if (map[x][y] == '.')
        {
            // 如果该点是空地, 那么2格内不包含障碍
            for (int i = 0; i < 8; i++)
            {
                int tx = loadedDir[i][0] + x;
                int ty = loadedDir[i][1] + y;
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
        // 如果周围没有负载可访问点，那么该点不可访问
        bool noAccess = true;
        if ((accessMap[x][y] & 0b11110000) != 0)
        {
            noAccess = false;
        }
        for (int i = 0; i < 8; i++)
        {
            int tx = unloadDir[i][0] + x;
            int ty = unloadDir[i][1] + y;
            if ((accessMap[tx][ty] & 0b11110000) != 0)
            {
                noAccess = false;
            }
        }
        if (noAccess)
        {
            return false;
        }
    }
    else
    {
        // 空载情况下
        if (map[x][y] == '.')
        {
            for (int i = 0; i < 8; i++)
            {
                int tx = unloadDir[i][0] + x;
                int ty = unloadDir[i][1] + y;
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
