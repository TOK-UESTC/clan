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
    return nullptr;
}

// 起点
void DStarLite::search(int r, int c, bool loaded, int id)
{
    std::queue<int> qx;
    std::queue<int> qy;

    qx.push(r);
    qy.push(c);
}

void DStarLite::calculateKey(int r, int c)
{
    std::min(g[r][c], rhs[r][c]) + h(sr, sc, r, c) + k, std::min(g[r][c], rhs[r][c]);
    return;
}

double DStarLite::h(int sr, int sc, int tr, int tc)
{
    int x = sr - tr;
    int y = sc - tc;

    return sqrt(x * x + y * y);
}