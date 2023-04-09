#ifndef POOLS_H
#define POOLS_H

#include "includeAll.h"

class Pools
{
private:
    ObjectPool<Vec> *vecPool;

    Pools &operator=(const Pools &) = delete;

public:
    Pools()
    {
        vecPool = new ObjectPool<Vec>(100);
    };
    ~Pools()
    {
        delete vecPool;
    };

    Vec *getVec()
    {
        return vecPool->acquire();
    };
    void release(Vec *vec)
    {
        vecPool->release(vec);
    };
};

extern Pools pools;

#endif