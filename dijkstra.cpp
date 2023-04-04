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

double **Dijkstra::getDistMap()
{
    return dist;
}

// 起点
void Dijkstra::search(int r, int c, bool loaded, int id)
{
    double fill = 1000000.;
    dist = Maps::newDMap(accessMap, fill);
    // visited = Maps::newBMap(accessMap);
    bool visited[201][201];
    for (int i = 0; i < 201; i++)
    {
        for (int j = 0; j < 201; j++)
        {
            visited[i][j] = false;
        }
    }
    dist[r][c] = 0; // 起点标志

    std::queue<int> qx; // 存放遍历点x坐标
    std::queue<int> qy; // 存放遍历点y坐标

    // 将机器人位置加入队列
    qx.push(r);
    qy.push(c);
    visited[r][c] = true;

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

            // 得到currR, currC周围最短距离
            double increDist = 0.;
            double minDist = dist[currR][currC];

            // 遍历所有方位
            for (int j = 0; j < 8; j++)
            {
                int nr = unloadDir[j][0] + currR;
                int nc = unloadDir[j][1] + currC;

                // 是否遍历过，是否是有效坐标，根据是否Load进行判断
                // if (visited[cess(nr, nc, loaded, id) && dist[nr][nc] > fill - 1)
                // if (checkAccess(nr, nc, loaded, id) !visit[nr][nc]- 1)
                if (visited[nr][nc] == false && checkAccess(nr, nc, loaded, id))
                {
                    qx.push(nr);
                    qy.push(nc);
                    visited[nr][nc] = true;
                }

                if (j < 4)
                {
                    increDist = 1.0;
                }
                else
                {
                    increDist = 1.4142135623730951;
                }

                // 找到最小距离
                if (minDist > dist[nr][nc] + increDist)
                {
                    minDist = dist[nr][nc] + increDist;
                }
            }
            dist[currR][currC] = minDist;
        }
    }
}

std::list<Vec *> *Dijkstra::getKnee(int r, int c)
{
    std::list<Vec *> *result = new std::list<Vec *>();
    // 根据dist进行拐点查找
    int cr = r;
    int cc = c;

    // 添加开头
    result->push_back(rc2Coord(cr, cc, 0.25));

    // TODO: 如果多个方向有相同的代价
    // ，根据上一次选择的下降方向去进行临近选择
    while (dist[cr][cc] > 0.01)
    {
        // 寻找下个访问点
        int lastDirect = -1;
        int direct = -1;
        double minCost = 1000000.;
        for (int i = 0; i < 8; i++)
        {
            int nr = unloadDir[i][0] + cr;
            int nc = unloadDir[i][1] + cc;

            if (dist[nr][nc] < minCost)
            {
                minCost = dist[nr][nc];
                direct = i;
            }
        }

        // 找到最小方向
        if (lastDirect == -1)
        {
            lastDirect = direct;
        }

        // 更新位置
        cr += unloadDir[direct][0];
        cc += unloadDir[direct][1];

        // 方向发生了变化。则添加到拐点列表
        if (direct != lastDirect)
        {
            result->push_back(rc2Coord(cr, cc, 0.25));
        }
    }
    // 将结束工作台放置到列表中
    result->push_back(rc2Coord(cr, cc, 0.25));

    // 向量反转，方便进行遍历
    std::reverse(result->begin(), result->end());

    return result;
}