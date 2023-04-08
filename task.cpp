#include "include/includeAll.h"

double Task::getWeight()
{
    // 先供给原料格占用多的工作台
    int status = to->getPlanMaterialStatus() | to->getMaterialStatus();
    double weightCommand = 1.0;
    for (int i = 1; i < 7; i++)
    {
        weightCommand += (((status & (1 << i)) != 0) ? 1.0 : 0.0);
    }

    // 添加跨生产线衰减
    double weightCross = isCross();

    // 4,5,6均衡生产
    double weightBalance = 1.;
    if (to->getType() == 4 || to->getType() == 5 || to->getType() == 6)
    {
        weightBalance = (balanceMapper[to->getType()] == getMinValue()) ? 4. : 1.;
    }

    return weightCommand / weightCross * weightBalance;
}

int Task::getMinValue()
{
    int min1 = balanceMapper[4] < balanceMapper[5] ? balanceMapper[4] : balanceMapper[5];
    int min2 = balanceMapper[4] < balanceMapper[6] ? balanceMapper[4] : balanceMapper[6];
    return min1 < min2 ? min1 : min2;
}

double Task::isCross()
{
    if (from->getType() == 1 || from->getType() == 2 || from->getType() == 3)
    {
        if (to->getType() == 4 || to->getType() == 5 || to->getType() == 6)
        {
            return 1.;
        }

        if (to->getType() == 9)
        {
            return 4.;
        }
    }

    if (from->getType() == 4 || from->getType() == 5 || from->getType() == 6)
    {
        if (to->getType() == 7)
        {
            return 1.;
        }

        if (workbenchMapper[7] == 0)
        {
            // 没有7,9就就不算跨级
            return 1.;
        }
        if (to->getType() == 9)
        {
            return 4.;
        }
    }

    return 1.;
}