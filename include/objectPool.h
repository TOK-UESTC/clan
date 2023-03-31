#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <functional>
#include <vector>

template <typename T>
class ObjectPool
{
private:
    std::vector<T> pool;
    std::function<T()> supplier;

public:
    ObjectPool(int initCount, std::function<T()> supplier) : supplier(supplier)
    {
        // 先开辟池子，避免冷启动过慢
        for (int i = 0; i < initCount; i++)
        {
            pool.push_back(supplier());
        }
    }

    T acquire()
    {
        T t;
        if (pool.empty())
        {
            t = supplier();
        }
        else
        {
            // 从池中取出，放到使用池中
            t = pool.back();
            pool.pop_back();
        }
        return t;
    }

    void release(T t)
    {
        pool.push_back(t);
    }

    int availableSize()
    {
        return pool.size();
    }
};

#endif