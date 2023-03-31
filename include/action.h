#ifndef ACTION_H
#define ACTION_H

#include "includeAll.h"

/*
 * @brief 机器人基础动作
 *
 *
 */

class Action
{
private:
    enum ActionType type;
    double value;

public:
    Action(enum ActionType type, double value)
    {
        this->type = type;
        this->value = value;
    }

    /** 给不需要参数的动作使用, e->g-> buy, sell, destroy */
    Action(ActionType type)
    {
        this->type = type;
        this->value = 0;
    }

    void set(ActionType type, double value)
    {
        this->type = type;
        this->value = value;
    }

    void set(ActionType type)
    {
        this->type = type;
        this->value = 0;
    }

    /** 输出为字符串，方便直接调用 */
    std::string toString(int robotId)
    {
        std::stringstream ss;
        switch (type)
        {
        case FORWARD:
            ss << "forward"
               << " " << robotId << " " << value;
            break;
        case ROTATE:
            ss << "rotate"
               << " " + robotId << " " << value;
            break;
        case BUY:
            ss << "buy"
               << " " << robotId;
            break;
        case SELL:
            ss << "sell"
               << " " << robotId;
            break;
        case DESTROY:
            ss << "destroy "
               << " " << robotId;
            break;
        default:
            return "";
        }

        return ss.str();
    }
    Action *update(ActionType type, double value)
    {
        this->type = type;
        this->value = value;
        return this;
    }
    Action *update(ActionType type)
    {
        this->type = type;
        this->value = 0;
        return this;
    }
};

#endif