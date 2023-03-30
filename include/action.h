#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <sstream>
#include <iostream>
#include "const.h"

using namespace std;

/*
 * @brief 机器人基础动作
 *
 *
 */

class Action
{
private:
    ActionType type;
    double value;

public:
    Action(ActionType type, double value)
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

    void copy(ActionType type, double value)
    {
        this->type = type;
        this->value = value;
    }

    void copy(ActionType type)
    {
        this->type = type;
        this->value = 0;
    }

    /** 输出为字符串，方便直接调用 */
    string toString(int robotId)
    {
        stringstream ss;
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
};

#endif