#include "includeAll.h"

void ActionModel::generateMoveActions()
{
    // 首先判断是否有任务
    if (rb->getTask() == nullptr)
    {
        return;
    }

    // // 获取state
    // MotionState *state = statePool.acquire();
    // state->update(rb);

    // // TODO: 需要每帧都预测么？
    // Coordinate *next = rb.predict();
    // nextPos.setValue(next);
    // coordPool.release(next);

    // // Calculate the control factors for the robot's movement
    // double *controlFactor = rb.control(state, nextPos);

    // // Release the acquired state
    // statePool.release(state);

    // // 产生转向动作
    // rb.addAction(this->rotateAction.update(ActionType::ROTATE, controlFactor[1]));
    // // 产生前进动作
    // rb.addAction(this->forwardAction.update(ActionType::FORWARD, controlFactor[0]));
}

void ActionModel::generateShopActions()
{
    if (rb->getTask() == nullptr)
    {
        return;
    }

    Workbench *wb;

    // 购买
    if (rb->getProductType() == 0)
    {
        wb = rb->getTask()->getFrom();
        Workbench *to = rb->getTask()->getTo();
        // 判断是否在目标工作台附近，并且当前已经调转，开始朝向下一个工作台
        if (rb->getWorkbenchIdx() == wb->getWorkbenchIdx() && computeDist(wb->getPos(), to->getPos()) / MAX_FORWARD_FRAME * 1.2 < Context::leftFrame)
        {
            // 购买行为
            rb->addAction(this->buyAction.update(ActionType::BUY));
        }
    }
    else
    {
        // 去售出
        wb = rb->getTask()->getTo();
        if (rb->getWorkbenchIdx() == wb->getWorkbenchIdx())
        {
            // 售出行为
            rb->addAction(this->sellAction.update(ActionType::SELL));
        }
    }
}