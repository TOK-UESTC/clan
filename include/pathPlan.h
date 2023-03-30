#ifndef PATH_PLAN_H
#define PATH_PLAN_H

#include <vector>
#include "vec.h"

using namespace std;
/*
 * @brief 管理路径规划算法，路径规划算法基类
 *
 *
 */

class PathPlan
{

public:
    vector<Vec> findPath();
};

#endif