#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>
#include <unordered_set>
#include <functional>

template <typename T>
class ObjectPool
{
private:
    std::vector<T *> pool;
    std::unordered_set<T *> used;
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
    ObjectPool(int initCount)
    {
        // 先开辟池子，避免冷启动过慢
        for (int i = 0; i < initCount; i++)
        {
            pool.push_back(new T());
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
        used.insert(t);
        return t;
    }

    void release(T *t)
    {
        if (used.erase(t))
        {
            pool.push_back(t);
        }
    }

    int availableSize()
    {
        return pool.size();
    }

    int usedSize()
    {
        return used.size();
    }
};

#endif