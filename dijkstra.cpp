#include "include/includeAll.h"

bool Dijkstra::checkAccess(int r, int c, bool loaded, int id)
{
    // 检查位置是否合法
    if (r < 0 || r >= row || c < 0 || c >= col)
    {
        return false;
    }

    // 检查位置是否可通过
    int status = accessMap[r][c];
    int shift = id;

    if (loaded)
    {
        shift += LOAD_SHIFT_BIT;
    }

    return ((1 << shift) & status) != 0;
}

int **Dijkstra::getDistMap()
{
    return dist;
}

// 起点
void Dijkstra::search(int r, int c, bool loaded, int id)
{
    dist = Maps::newMap(accessMap);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            dist[i][j] = -1;
        }
    }

    dist[r][c] = 0; // 起点标志

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

    // 距离代价
    int cost = 1;

    while (!qx.empty())
    {
        // 获取当前队列大小
        int size = qx.size();

        for (int i = 0; i < size; ++i)
        {
            int currR = qx.front();
            int currC = qy.front();
            qx.pop();
            qy.pop();

            dist[currR][currC] = cost;

            // 遍历所有方位
            for (int j = 0; j < 8; j++)
            {
                int nr = dir[i][0] + currR;
                int nc = dir[i][1] + currC;

                // 是否遍历过，是否是有效坐标，根据是否Load进行判断
                if (checkAccess(nr, nc, loaded, id) && dist[nr][nc] == -1)
                {
                    qx.push(nr);
                    qy.push(nc);
                }
            }
        }

        cost += 1;
    }
}