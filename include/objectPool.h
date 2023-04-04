#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>
#include <functional>

template <typename T>
class ObjectPool
{
private:
    std::vector<T *> pool;

public:
    ObjectPool(int initCount)
    {
        // 先开辟池子，避免冷启动过慢
        for (int i = 0; i < initCount; i++)
        {
            pool.push_back(new T());
        }
    }

    ~ObjectPool()
    {
        for (auto t : pool)
        {
            delete t;
        }
    }

    T *acquire()
    {
        T *t;
        if (pool.empty())
        {
            t = new T();
        }
        else
        {
            // 从池中取出，放到使用池中
            t = pool.back();
            pool.pop_back();
        }
        return t;
    }

    void release(T *t)
    {
        pool.push_back(t);
    }

    int availableSize()
    {
        return pool.size();
    }
};

#endif