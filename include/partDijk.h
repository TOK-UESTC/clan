#ifndef PARTDIJK_H
#define PARTDIJK_H

#include "includeAll.h"

class PartDijk
{
private:
    int **accessMap;     // 访问图
    double **loadedDist; // 负载距离图
    double **unloadDist; // 空载距离图
    int row, col;        // 地图属性
public:
    PartDijk(int **accessMap)
    {
        this->accessMap = accessMap;
        this->row = MAP025;
        this->col = MAP025;

        loadedDist = new double *[row];
        for (int i = 0; i < this->row; i++)
        {
            loadedDist[i] = new double[this->col];
        }

        unloadDist = new double *[row];
        for (int i = 0; i < this->row; i++)
        {
            unloadDist[i] = new double[this->col];
        }
    }

    ~PartDijk()
    {
        int row = MAP025;
        for (int i = 0; i < row; i++)
        {
            delete[] loadedDist[i];
            delete[] unloadDist[i];
        }
        delete[] loadedDist;
        delete[] unloadDist;
    }

    std::vector<std::pair<int, int> *> *search(int r, int c, bool loaded, double range, int collideR, int collideC); // 从起始点进行搜素，填充访问图
    std::list<Vec *> *getKnee(int r, int c, bool isLoad);                                                            // 根据访问图获取拐点，输入为目标位置

    bool validCoord(int r, int c);               // 检查坐标是否合法
    bool checkAccess(int r, int c, bool loaded); // 根据机器人状态判断是否可达

    double **getDistMap(bool isLoad);
    void fillDist(double fill, bool isLoad);

    int **getAccessMap()
    {
        return accessMap;
    }
};

#endif