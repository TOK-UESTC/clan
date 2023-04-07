#include "include/includeAll.h"

bool Dijkstra::checkAccess(int r, int c, bool loaded, int id)
{
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
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};
bool Dijkstra::validCoord(int r, int c)
{
    // 检查位置是否合法
    if (r < 0 || r >= row || c < 0 || c >= col)
    {
        return false;
    }

    return true;
}

// 起点
void Dijkstra::search(int r, int c, bool loaded, int id)
{
    // 如果是负载情况，首先搜索最近的255的点作为起点
    if (loaded && (accessMap[r][c] & (1 << (LOAD_SHIFT_BIT + id))) == 0)
    {
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

        // 标记起点
        visited[r].insert(c);

        // 开始搜索
        while (!qx_init.empty())
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
                if (!checkAccess(nr, nc, false, id))
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
        r = minR;
        c = minC;
    }
    double fill = 1000000.;
    fillDist(fill);
    dist[r][c] = 0; // 起点标志

    std::queue<int> qx; // 存放遍历点x坐标
    std::queue<int> qy; // 存放遍历点y坐标

    // 将机器人位置加入队列
    qx.push(r);
    qy.push(c);

    while (!qx.empty())
    {
        int currR = qx.front();
        int currC = qy.front();
        qx.pop();
        qy.pop();

        // 得到currR, currC周围最短距离
        double cost = 0.;

        // 遍历所有方位
        for (int j = 0; j < 8; j++)
        {
            int nr = unloadDir[j][0] + currR;
            int nc = unloadDir[j][1] + currC;

            // 检查地址合法性
            if (!validCoord(nr, nc) || !checkAccess(nr, nc, loaded, id))
            {
                continue;
            }

            if (j < 4)
            {
                cost = 1.0;
            }
            else
            {
                cost = 1.4142135623730951;
            }

            // 找到最小距离
            double newCost = dist[currR][currC] + cost;
            if (newCost < dist[nr][nc])
            {
                dist[nr][nc] = newCost;
                qx.push(nr);
                qy.push(nc);
            }
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
    // 寻找下个访问点
    int lastDirect = -1;
    int direct = -1;
    while (dist[cr][cc] > 0.01)
    {
        double minCost = 1000000.;
        for (int i = 0; i < 8; i++)
        {
            int nr = unloadDir[i][0] + cr;
            int nc = unloadDir[i][1] + cc;

            if (!validCoord(nr, nc))
            {
                continue;
            }

            if (dist[nr][nc] < minCost)
            {
                minCost = dist[nr][nc];
                direct = i;
            }
        }
        if (direct == -1)
        {
            int i = 0;
        }

        // 更新位置
        cr += unloadDir[direct][0];
        cc += unloadDir[direct][1];

        // 方向发生了变化。则添加到拐点列表
        if (direct != lastDirect)
        {
            result->push_back(rc2Coord(cr, cc, 0.25));
        }
        // 找到最小方向
        lastDirect = direct;
    }
    // 将结束工作台放置到列表中
    Vec *lastVec = rc2Coord(cr, cc, 0.25);
    if (computeDist(lastVec, result->back()) < 0.1)
    {
        delete lastVec;
    }
    else
    {
        result->push_back(lastVec);
    }

    // 向量反转，方便进行遍历
    std::reverse(result->begin(), result->end());

    return result;
}