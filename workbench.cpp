#include "include/includeAll.h"

Workbench::Workbench(int id, double x, double y, int type)
{
    this->type = type;
    this->pos.set(x, y);
    this->id = id;
    this->rest = getRestTime(type);
    this->productStatus = 0;
    this->lastMaterialStatus = 0;
    this->planProductStatus = 0;
    this->planMaterialStatus = 0;
}

Workbench::~Workbench()
{
    delete dijkstra;
}

/* 初始化时返回工作台剩余生产时间 */
int Workbench::getRestTime(int type)
{
    if (type == 1 || type == 2 || type == 3)
    {
        return 50;
    }
    return -1;
}

void Workbench::setPlanProductStatus(int planProductStatus)
{
    this->planProductStatus = planProductStatus;
}

int Workbench::getPlanProductStatus()
{
    return this->planProductStatus;
}

int Workbench::getPlanMaterialStatus()
{
    return planMaterialStatus;
}
int Workbench::getMaterialStatus()
{
    return materialStatus;
}

/** 只更新剩余生产时间，原材料状态以及产品格状态 */
void Workbench::update()
{
    // 空白变量，用于接受用不到的内容
    int id;
    double x, y;
    lastMaterialStatus = materialStatus;
    scanf("%d %lf %lf %d %d %d", &id, &x, &y, &rest, &materialStatus, &productStatus);
    getchar();
}

/** 查看是否含有某个原材料 */
bool Workbench::hasMaterial(int type)
{
    if (this->type == 8 || this->type == 9)
    {
        return false;
    }
    return ((1 << type) & materialStatus) != 0;
}

/** 查看是否规划中已经占用该工作台原料格 */
bool Workbench::hasPlanMaterial(int type)
{
    if (this->type == 8 || this->type == 9)
    {
        return false;
    }
    return ((1 << type) & planMaterialStatus) != 0;
}

/**
 * 更新原料格状态
 *
 * @param type: 原材料类型
 * @param sell: type类规划原材料格置0
 */
void Workbench::updatePlanMaterialStatus(int type, bool Sell)
{
    if (Sell)
    {
        this->planMaterialStatus = this->planMaterialStatus & (~(1 << type));
    }
    else
    {
        this->planMaterialStatus = this->planMaterialStatus | (1 << type);
    }
}

/** 是否空闲，rest==-1 */
bool Workbench::isFree()
{
    return (rest == -1) && (productStatus == 0);
}

/** 是否已经有产物, productStatus==1 */
bool Workbench::isReady()
{
    return productStatus == 1;
}

/** 获取工作台类型 */
int Workbench::getType()
{
    return type;
}

/** 获取工作台位置 */
Vec *Workbench::getPos()
{
    return &pos;
}

void Workbench::newDij(int **accessMap)
{
    this->dijkstra = new Dijkstra(accessMap);
}

/** 获取工作台剩余工作时间 */
int Workbench::getRest()
{
    return rest;
}

/** 获取工作台index */
int Workbench::getId()
{
    return id;
}

/** 当前工作台是否拥塞 */
bool Workbench::isBlocked()
{
    bool ret = false;
    switch (type)
    {
    case 4:
        if (planMaterialStatus == 0b110)
        {
            ret = true;
        }
        break;
    case 5:
        if (planMaterialStatus == 0b1010)
        {
            ret = true;
        }
        break;
    case 6:
        if (planMaterialStatus == 0b1100)
        {
            ret = true;
        }
        break;
    case 7:
        if (planMaterialStatus == 0b1110000)
        {
            ret = true;
        }
        break;
    default:
        ret = false;
        break;
    }
    return ret;
}

int Workbench::getWorkbenchIdx()
{
    return id;
}

/* 获取工作台在dijkstra算法获得的图中的坐标 */
int Workbench::getMapRow()
{
    return ((int)((49.75 - pos.getY()) / 0.5)) * 2 + 1;
}
int Workbench::getMapCol()
{
    return ((int)((pos.getX() - 0.25) / 0.5)) * 2 + 1;
}

Dijkstra *Workbench::getDij()
{
    return this->dijkstra;
}

bool Workbench::operator==(const Workbench &o) const
{
    return id;
}

size_t Workbench::hash() const
{
    return id;
}