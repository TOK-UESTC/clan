#include "include/includeAll.h"

bool DStarLite::checkAccess(int r, int c, bool loaded, int id)
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

bool DStarLite::validCoord(int r, int c)
{
    // 检查位置是否合法
    if (r < 0 || r >= row || c < 0 || c >= col)
    {
        return false;
    }

    return true;
}

std::list<Vec *> *DStarLite::getKnee(int r, int c)
{
    std::list<Vec *> *result = new std::list<Vec *>();
    // 根据dist进行拐点查找
    int cr = r;
    int cc = c;

    result->push_back(rc2Coord(cr, cc, 0.25));

    while (cr != tc || cc != tr)
    {

        // 寻找下个访问点
        int lastDirect = -1;
        int direct = -1;
        double minCost = 1000000.;
        for (int i = 0; i < 8; i++)
        {
            int nr = unloadDir[i][0] + cr;
            int nc = unloadDir[i][1] + cc;

            if (!validCoord(nr, nc))
            {
                continue;
            }

            if (g[nr][nc] < minCost)
            {
                minCost = g[nr][nc];
                direct = i;
            }
        }
        if (direct == -1)
        {
            int i = 0;
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

    return result;
}

// 起点
void DStarLite::search(bool loaded, int id)
{
    auto start = std::chrono::steady_clock::now();
    while (true)
    {
        // 根据Key进行排序
        std::vector<std::pair<std::pair<int, int>, std::pair<double, double>>> sortedKeys(U.begin(), U.end());
        std::sort(sortedKeys.begin(), sortedKeys.end(), cmp_map_value);
        std::pair<int, int> state = sortedKeys[0].first;
        std::pair<double, double> key = sortedKeys[0].second;

        // 检查是否需要更新
        if (key >= calculateKey(sr, sc) && this->rhs[sr][sc] == this->g[sr][sc])
        {
            break;
        }

        // 删除节点
        U.erase(state);

        int r = state.first;
        int c = state.second;
        std::pair<double, double> newKeys = this->calculateKey(r, c);
        if (key < newKeys)
        {
            U[state] = newKeys;
        }
        else if (g[r][c] > rhs[r][c])
        {
            g[r][c] = rhs[r][c];
            for (int i = 0; i < 8; i++)
            {
                int nr = unloadDir[i][0] + r;
                int nc = unloadDir[i][1] + c;

                if (!validCoord(nr, nc))
                {
                    continue;
                }

                if (checkAccess(nr, nc, loaded, id))
                {
                    updateVertex(nr, nc, loaded, id);
                }
            }
        }
        else
        {
            g[r][c] = 9999;
            updateVertex(r, c, loaded, id);
            // 更新八个方向
            for (int i = 0; i < 8; i++)
            {
                int nr = unloadDir[i][0] + r;
                int nc = unloadDir[i][1] + c;

                if (!validCoord(nr, nc))
                {
                    continue;
                }

                if (checkAccess(nr, nc, loaded, id))
                {
                    updateVertex(nr, nc, loaded, id);
                }
            }
        }
    }
    auto end = std::chrono::steady_clock::now();
    double cost = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0;
    fprintf(stderr, "%.3fms\n", cost);
    fflush(stderr);
}

double DStarLite::cost(int r, int c, int nr, int nc)
{
    return sqrt((r - nr) * (r - nr) + (c - nc) * (c - nc));
}

double **DStarLite::getG()
{
    return g;
}

void DStarLite::updateVertex(int r, int c, bool loaded, int id)
{
    if (r != tr || c != tc)
    {
        rhs[r][c] = 9999;
        for (int i = 0; i < 8; i++)
        {
            int diffR = unloadDir[i][0];
            int diffC = unloadDir[i][1];
            int nr = diffR + r;
            int nc = diffC + c;

            if (!validCoord(nr, nc))
            {
                continue;
            }

            if (checkAccess(nr, nc, loaded, id))
            {
                rhs[r][c] = std::min(rhs[r][c], g[nr][nc] + std::sqrt(diffC * diffC + diffR * diffR));
            }
        }
    }

    std::pair<int, int> state(r, c);
    U.erase(state);

    if (g[r][c] != rhs[r][c])
    {
        U[state] = calculateKey(r, c);
    }
}

std::pair<double, double> DStarLite::calculateKey(int r, int c)
{
    return std::pair<double, double>(std::min(g[r][c], rhs[r][c]) + h(sr, sc, r, c) + km, std::min(g[r][c], rhs[r][c]));
}

double DStarLite::h(int sr, int sc, int tr, int tc)
{
    int x = sr - tr;
    int y = sc - tc;

    return sqrt(x * x + y * y);
}