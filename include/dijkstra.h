#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "includeAll.h"

class Dijkstra {
    private:
        char **accessMap;
        int **dist;  // 访问图
        int r, c;       // 起始位置，行列
        int row, col; //地图属性
    public:
        Dijkstra(char** accessMap) {
            this->accessMap = accessMap;
            this->row = _msize(accessMap) / 8;
            this->col = _msize(accessMap[0]);
        }

        // 检查当前坐标是否合规
        bool validCoord(int i, int j) {
            if (i < 0 || i  >= row || j < 0 || j >= col) {
                return false;
            }
            return true;
        }

        void search(int r, int c, bool loaded, int id);  // 从起始点进行搜素，填充访问图
        // void getKnee(int r, int c); // 根据访问图获取拐点，输入为目标位置

        bool checkAccess(int r, int c, bool loaded, int id); // 根据机器人状态判断是否可达

        int** getDistMap();
};

#endif
