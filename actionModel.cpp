#include "include/includeAll.h"

void ActionModel::generateMoveActions()
{
    // 首先判断是否有任务
    if (rb->getTask() == nullptr)
    {
        return;
    }

    // for (int i = 0; i < 10; i++)
    // {
    //     x.push_back(path[i][0]);
    //     y.push_back(path[i][1]);
    //     t.push_back(i);
    // }
    // std::vector<double> a1, a2;
    // spline(t, x, a1);
    // spline(t, y, a2);

    // // 获取插值结果曲线
    // for (double ti = 0; ti < 10; ti += 0.1)
    // {
    //     double xi = eval_spline(t, x, a1, ti);
    //     double yi = eval_spline(t, y, a2, ti);
    // }

    // // 如果目标点为空，则返回
    // if (paths.empty())
    // {
    //     return;
    // }

    // // 获取state
    MotionState *state = statePool->acquire();
    state->update(rb);

    // 获取下一个目标点
    Task *task = rb->getTask();
    std::list<Vec *> *road = task->getRoad();
    if (road->empty())
        return;
    Vec *nextPos = road->front();
    // 比较当前state与目标target的距离，如果距离小于一定值，则认为到达目标点
    if (computeDist(state->getPos(), nextPos) < 0.4)
    {
        road->pop_front();
        if (road->empty())
            return;
        nextPos = road->front();
    }

    // // 比较当前state与目标target的距离，如果距离小于一定值，则认为到达目标点
    // if (computeDist(state->getPos(), paths.front().get()) < 0.4)
    // {
    //     paths.pop_front();
    //     if (paths.empty())
    //         return;
    // }
    // Vec *nextPos = paths.front().get();

    // // TODO: 需要每帧都预测么？
    // Coordinate *next = rb.predict();
    // nextPos.setValue(next);
    // coordPool.release(next);

    // // Calculate the control factors for the robot's movement
    // double *controlFactor = rb.control(state, nextPos);
    double v = 0, w = 0;

    rb->control(state, nextPos, v, w);

    // // Release the acquired state
    statePool->release(state);
    // double controlFactor[2] = {1.0, 1.0};
    // 产生转向动作
    rb->addAction(this->rotateAction.update(ActionType::ROTATE, w));
    // 产生前进动作
    rb->addAction(this->forwardAction.update(ActionType::FORWARD, v));
}

void ActionModel::generateShopActions()
{
    if (rb->getTask() == nullptr)
    {
        return;
    }

    // 购买
    if (rb->getProductType() == 0)
    {
        Workbench wb = rb->getTask()->getFrom();
        Workbench to = rb->getTask()->getTo();
        // 判断是否在目标工作台附近，并且当前已经调转，开始朝向下一个工作台
        if (rb->getWorkbenchIdx() == wb.getWorkbenchIdx()) //&& computeDist(wb->getPos(), to->getPos()) / MAX_FORWARD_FRAME * 1.2 < Context::leftFrame)
        {
            // 购买行为
            rb->addAction(this->buyAction.update(ActionType::BUY));
        }
    }
    else
    {
        // 去售出
        Workbench wb = rb->getTask()->getTo();
        if (rb->getWorkbenchIdx() == wb.getWorkbenchIdx())
        {
            // 售出行为
            rb->addAction(this->sellAction.update(ActionType::SELL));
        }
    }
}
// 三次样条插值，x为自变量，y为因变量，a为三次样条的系数，a[0]为常数项，a[1]为一次项，a[2]为二次项，a[3]为三次项
void ActionModel::spline(const std::vector<double> &x, const std::vector<double> &y, std::vector<double> &a)
{
    int n = x.size();
    std::vector<double> h(n - 1);
    std::vector<double> b(n - 1);
    std::vector<double> u(n - 1);
    std::vector<double> v(n - 1);
    std::vector<double> z(n);

    for (int i = 0; i < n - 1; i++)
    {
        h[i] = x[i + 1] - x[i];
        b[i] = (y[i + 1] - y[i]) / h[i];
    }

    u[1] = 2.0 * (h[0] + h[1]);
    v[1] = 6.0 * (b[1] - b[0]);

    for (int i = 2; i < n - 1; i++)
    {
        u[i] = 2.0 * (h[i - 1] + h[i]) - h[i - 1] * h[i - 1] / u[i - 1];
        v[i] = 6. * (b[i] - b[i - 1]) - h[i - 1] * v[i - 1] / u[i - 1];
    }

    z[n - 1] = 0;
    for (int i = n - 2; i >= 1; i--)
    {
        z[i] = (v[i] - h[i] * z[i + 1]) / u[i];
    }
    z[0] = 0;

    a.resize(n - 1);
    for (int i = 0; i < n - 1; i++)
    {
        a[i] = (z[i + 1] - z[i]) / (6.0 * h[i]);
    }
}
// 获取三次样条插值的值，x为自变量，y为因变量，a为三次样条的系数，a[0]为常数项，a[1]为一次项，a[2]为二次项，a[3]为三次项，xi为插值点
double ActionModel::eval_spline(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &a, double xi)
{
    int n = x.size();

    int i = 0;
    if (xi >= x[n - 1])
    {
        i = n - 2;
    }
    else
    {
        while (xi > x[i + 1])
        {
            i++;
        }
    }

    double dx = x[i + 1] - x[i];
    double dy = y[i + 1] - y[i];
    double A = a[i] * dx - dy;
    double B = -a[i + 1] * dx + dy;
    double t = (xi - x[i]) / dx;
    double q = (1.0 - t) * y[i] + t * y[i + 1] + t * (1.0 - t) * (A * (1.0 - t) + B * t);
    return q;
}