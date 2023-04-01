#ifndef PATH_PLAN_H
#define PATH_PLAN_H

#include "common.h"
/*
 * @brief 管理路径规划算法，路径规划算法基类
 *
 *
 */

class PathPlan
{

public:
    std::vector<Vec> findPath();
};

#endif