#include "include/includeAll.h"

bool Dijkstra::checkAccess(int r, int c, bool loaded)
{
    // 检查位置是否可通过
    int status = accessMap[r][c];
    int shift = 0b1111;

    if (loaded)
    {
        shift = 0b11110000;
    }

    return (shift & status) != 0;
}

double **Dijkstra::getDistMap(bool isLoad)
{
    return isLoad ? loadedDist : unloadDist;
}

void Dijkstra::fillDist(double fill, bool isLoad)
{

    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->col; j++)
        {
            if (isLoad)
            {
                loadedDist[i][j] = fill;
            }
            else
            {
                unloadDist[i][j] = fill;
            }
        }
    }
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
void Dijkstra::search(int r, int c, bool loaded)
{
    double fill = 1000000.;
    fillDist(fill, loaded);
    double **dist = loaded ? loadedDist : unloadDist;

    // 当wb对于rb是不可达的时候，退出
    if (accessMap[r][c] == 0)
    {
        return;
    }
    // 如果是负载情况，首先搜索最近的255的点作为起点
    if (loaded && (accessMap[r][c] & 0b11110000) == 0)
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
            if ((accessMap[curr_x][curr_y] & 0b11110000) != 0)
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
                if (!checkAccess(nr, nc, false))
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
            if (!validCoord(nr, nc) || !checkAccess(nr, nc, loaded))
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

std::list<Vec *> *Dijkstra::getKnee(int r, int c, bool isLoad, bool reverse)
{
    std::list<Vec *> *result = new std::list<Vec *>();
    // 根据dist进行拐点查找
    int cr = r;
    int cc = c;

    // 添加开头
    // result->push_back(rc2Coord(cr, cc, 0.25));

    // TODO: 如果多个方向有相同的代价
    // ，根据上一次选择的下降方向去进行临近选择
    // 寻找下个访问点
    int lastDirect = -1;
    int direct = -1;
    double **dist = isLoad ? loadedDist : unloadDist;
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

        // 方向发生了变化。则添加到拐点列表
        if (direct != lastDirect)
        {
            result->push_back(rc2Coord(cr, cc, 0.25));
        }

        // 更新位置
        cr += unloadDir[direct][0];
        cc += unloadDir[direct][1];
        // 找到最小方向
        lastDirect = direct;
    }
    // 将结束工作台放置到列表中
    Vec *lastVec = rc2Coord(cr, cc, 0.25);
    if (result->size() == 0)
    {
        result->push_back(lastVec);
    }
    if (computeDist(lastVec, result->back()) < 0.1)
    {
        pools.release(lastVec);
    }
    else
    {
        result->push_back(lastVec);
    }

    // 向量反转，方便进行遍历
    if (reverse)
    {
        std::reverse(result->begin(), result->end());
    }
    // // // 整理拐点
    // removeKnee(result, isLoad);

    return result;
}
// 消除直线上的拐点
void Dijkstra::removeKnee(std::list<Vec *> *knee, bool isLoad)
{
    // 只有两点，无需消除
    if (knee->size() < 3)
    {
        return;
    }
    // 从第一个点开始与倒数的点进行比较，如果直线上没有障碍物，则删除中间的点
    std::list<Vec *>::iterator it = knee->begin();
    std::list<Vec *>::iterator it2 = knee->end();
    it2--;
    while (it != it2)
    {
        // 检查两点之间是否有障碍物
        std::list<std::pair<int, int> *> *line = new std::list<std::pair<int, int> *>();
        getLine(*it, *it2, line);
        // anotherGetline(*it, *it2, line);
        bool hasObstacle = false;
        for (std::list<std::pair<int, int> *>::iterator it3 = line->begin(); it3 != line->end(); it3++)
        {
            // 如果当前负载状态下可以访问，则说明无障碍物
            if (accessMap[(*it3)->first][(*it3)->second] & ((isLoad ? 0b11110000 : 0b00001111) != 0))
            {
                continue;
            }
            // 有障碍物，跳出循环
            hasObstacle = true;
            break;
        }
        // 释放内存
        for (std::list<std::pair<int, int> *>::iterator it3 = line->begin(); it3 != line->end(); it3++)
        {
            delete *it3;
        }
        delete line;

        // 如果没有障碍物，则删除中间的点
        if (!hasObstacle)
        {
            // 释放内存
            pools.release(*it);
            // 删除it与it2之间的点
            it = knee->erase(it, it2);
        }
        else
        {
            it++;
        }
    }
}

// Bresenham算法计算两点之间的所有格子
void Dijkstra::getLine(Vec *start, Vec *end, std::list<std::pair<int, int> *> *result)
{
    double startX = start->getX();
    double startY = start->getY();
    double endX = end->getX();
    double endY = end->getY();
    int lineWidth = 0;

    // 计算两个点之间的距离
    double dist = std::sqrt((endX - startX) * (endX - startX) + (endY - startY) * (endY - startY));

    // 计算线段的单位向量
    double dx = (endX - startX) / dist;
    double dy = (endY - startY) / dist;

    // 计算线段上的所有点
    for (int i = 0; i <= dist; i++)
    {
        // 计算当前点的坐标
        double x = startX + dx * i;
        double y = startY + dy * i;

        // 将线段上的点加入到结果集合中
        int row = ((int)((49.75 - y) / 0.5)) * 2 + 1;
        int col = ((int)((x + 49.75) / 0.5)) * 2 + 1;
        result->push_back(new std::pair<int, int>(row, col));
        // 如果线段宽度大于1，需要将线段宽度范围内的点也加入到结果集合中
        for (int j = 1; j <= lineWidth; j++)
        {
            double offsetX = dy * j;
            double offsetY = dx * j;
            row = ((int)((49.75 - (y + offsetY)) / 0.5)) * 2 + 1;
            col = ((int)((x + offsetX + 49.75) / 0.5)) * 2 + 1;
            result->push_back(new std::pair<int, int>(row, col));
            row = ((int)((49.75 - (y - offsetY)) / 0.5)) * 2 + 1;
            col = ((int)((x - offsetX + 49.75) / 0.5)) * 2 + 1;
            result->push_back(new std::pair<int, int>(row, col));
        }
    }
}
void Dijkstra::anotherGetline(Vec *start, Vec *end, std::list<std::pair<int, int> *> *result)
{
    double x0 = start->getX();
    double y0 = start->getY();
    double x1 = end->getX();
    double y1 = end->getY();

    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0); // 判断是否需要交换x和y
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1)
    { // 确保x0小于等于x1
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    double dx = x1 - x0;
    double dy = std::abs(y1 - y0);
    double error = dx / 2.0;
    double ystep = (y0 < y1) ? 1.0 : -1.0;
    double y = y0;

    // 使用Bresenham算法将直线段网格化
    for (double x = x0; x <= x1; x += 1.0)
    {
        double real_x = x;
        double real_y = y;
        if (steep)
        {
            std::swap(real_x, real_y);
        }
        int col = ((int)((real_x - 0.25) / 0.5)) * 2 + 1;
        int row = ((int)((49.75 - real_y) / 0.5)) * 2 + 1;
        result->push_back(new std::pair<int, int>(row, col));
        error -= dy;
        if (error < 0)
        {
            y += ystep;
            error += dx;
        }
    }
}