#include "include/motionModel.h"
/**
 * 根据输入进行预测
 *
 * @param state:                 上一帧得到的预测状态
 * @param targetVelocity:        目标速度
 * @param targetAngularVelocity: 目标角度
 */
MotionState *MotionModel::predict(MotionState &state, double targetVelocity, double targetAngularVelocity)
{
    // 计算合速度v0
    double v0 = state.vMod();

    // 根据当前的负载情况确定加速度值
    double tempAngularAcc = 0;
    double tempLinearAcc = 0;
    if (!state.isLoaded())
    {
        // tempAngularAcc = targetAngularVelocity > state.getW() ? ANGULAR_ACC :
        // -ANGULAR_ACC;
        tempAngularAcc = getAngularAcc(false, targetAngularVelocity, state.getW(), 0.02);
        tempLinearAcc = targetVelocity > v0 ? LINEAR_ACC : -LINEAR_ACC;
    }
    else
    {
        // tempAngularAcc = targetAngularVelocity > state.getW() ? LOADED_ANGULAR_ACC :
        // -LOADED_ANGULAR_ACC;
        tempAngularAcc = getAngularAcc(true, targetAngularVelocity, state.getW(), 0.02);
        tempLinearAcc = targetVelocity > v0 ? LOADED_LINEAR_ACC : -LOADED_LINEAR_ACC;
    }

    // 根据目标角速度与当前角速度以及加速度定值计算加速时间
    double tAngle = (targetAngularVelocity - state.getW()) / tempAngularAcc;
    // 根据加速时间计算线速度加速时间
    double tV = (targetVelocity - v0) / tempLinearAcc;

    std::list<MotionFrag *> frags;
    MotionFrag *frag = fragPool.acquire();
    if (tV >= FRAME_TIME && tAngle >= FRAME_TIME)
    {
        frag->update(FRAME_TIME, tempLinearAcc, tempAngularAcc);

        // 均处于加速状态,分为四种，加速加速，加速减速，减速加速，减速减速
        frags.push_back(frag);
    }
    else if (tV < FRAME_TIME && tAngle < FRAME_TIME)
    {
        // 两个均处于匀速状态
        if (tV < MIN_ERROR && tAngle < MIN_ERROR)
        {
            frag->update(FRAME_TIME, 0, 0);
            // 两个均处于匀速状态
            frags.push_back(frag);
        }
        else if (tV >= tAngle)
        {
            // 两个均处于变匀速状态,判断分段情况
            // 说明角速度加速时间大于线速度加速时间，分为三段
            // 第一段，角速度加速，线速度加速
            frag->update(tAngle, tempLinearAcc, tempAngularAcc);
            frags.push_back(frag);
            // 第二段，角速度匀速，线速度加速
            frag = fragPool.acquire();
            frag->update(tV - tAngle, tempLinearAcc, 0);
            frags.push_back(frag);
            // 第三段，角速度匀速，线速度匀速
            frag = fragPool.acquire();
            frag->update(FRAME_TIME - tV, 0, 0);
            frags.push_back(frag);
        }
        else
        {
            // 说明线速度加速时间大于角速度加速时间，分为三段
            // 第一段，线速度加速，角速度加速
            frag->update(tV, tempLinearAcc, tempAngularAcc);
            frags.push_back(frag);
            // 第二段，线速度匀速，角速度加速
            frag = fragPool.acquire();
            frag->update(tAngle - tV, 0, tempAngularAcc);
            frags.push_back(frag);
            // 第三段，线速度匀速，角速度匀速
            frag = fragPool.acquire();
            frag->update(FRAME_TIME - tAngle, 0, 0);
            frags.push_back(frag);
        }
    }
    else
    {
        // 一个处于加速状态，一个处于变匀速状态
        if (tV >= FRAME_TIME)
        {
            // 线速度处于加速状态,角速度变匀速
            frag->update(tAngle, tempLinearAcc, tempAngularAcc);
            frags.push_back(frag);

            frag = fragPool.acquire();
            frag->update(FRAME_TIME - tAngle, tempLinearAcc, 0);
            frags.push_back(frag);
        }
        else if (tAngle >= FRAME_TIME)
        {
            // 角速度处于加速状态,线速度变匀速
            frag->update(tV, tempLinearAcc, tempAngularAcc);
            frags.push_back(frag);

            frag = fragPool.acquire();
            frag->update(FRAME_TIME - tV, 0, tempAngularAcc);
            frags.push_back(frag);
        }
    }

    MotionState *result = statePool.acquire();
    result->update(state);
    for (MotionFrag *item : frags)
    {
        predictFrag(result, item);
        // 将使用过的frag释放出来
        fragPool.release(item);
    }

    // result同时也要记录所需的线速度与角速度指令
    result->setTargetVelocity(targetVelocity);
    result->setTargetAngularVelocity(targetAngularVelocity);
    return result;
}

/**
 * 根据输入进行预测
 *
 * @param state: 机器人当前状态
 * @param frag:  机器人下一个切片
 */

void MotionModel::predictFrag(MotionState *state, MotionFrag *frag)
{
    double heading = state->getHeading();
    double x = state->getPos()->getX();
    double y = state->getPos()->getY();
    double stateV = state->vMod();
    double fragAcc = frag->getAngularAcc();
    double fragT = frag->getT();
    double fragLineAcc = frag->getLinearAcc();
    double w = state->getW();

    a = (w * w) / (2 * fragAcc) - heading;
    b = (fragAcc * fragT + w) / (sqrt(fragAcc) * sqrtPI);
    bc = FresnelC(b);
    bs = FresnelS(b);

    b0 = w / (sqrt(fragAcc) * sqrtPI);
    b0c = FresnelC(b0);
    b0s = FresnelS(b0);

    c = w * sqrtPI;

    // 加速度不为零
    if (fragAcc >= MIN_ERROR)
    {
        x += getIntegralXFront(stateV, fragAcc, heading, w, fragT);
        x -= getIntegralXFront(stateV, fragAcc, heading, w, 0);
        x += getIntegralXBack(fragLineAcc, fragAcc, heading, w, fragT);
        x -= getIntegralXBack(fragLineAcc, fragAcc, heading, w, 0);
        y += getIntegralYFront(stateV, fragAcc, heading, w, fragT);
        y -= getIntegralYFront(stateV, fragAcc, heading, w, 0);
        y += getIntegralYBack(fragLineAcc, fragAcc, heading, w, fragT);
        y -= getIntegralYBack(fragLineAcc, fragAcc, heading, w, 0);
    }
    else if (fragAcc < MIN_ERROR && w >= MIN_ERROR)
    {
        // 加速度为0，且角速度不为0
        x += getIntegralXAngular0(stateV, fragLineAcc, heading, w, fragT);
        x -= getIntegralXAngular0(stateV, fragLineAcc, heading, w, 0);
        y += getIntegralYAngular0(stateV, fragLineAcc, heading, w, fragT);
        y -= getIntegralYAngular0(stateV, fragLineAcc, heading, w, 0);
    }
    else if (fragAcc < MIN_ERROR && w < MIN_ERROR)
    {
        // 加速度为0，且角速度为0
        x += (stateV * fragT + fragLineAcc * (fragT * fragT)) * cos(heading);
        y += (stateV * fragT + fragLineAcc * (fragT * fragT)) * sin(heading);
    }

    heading += w * fragT + 0.5 * fragAcc * fragT * fragT;
    // 恢复到-pai~pai
    if (heading > PI)
    {
        heading -= 2 * PI;
    }
    else if (heading < -PI)
    {
        heading += 2 * PI;
    }

    // 更新state并返回
    state->setPos(x, y);
    state->setHeading(heading);
    double newVx = (stateV + fragLineAcc * fragT) * cos(heading);
    double newVy = (stateV + fragLineAcc * fragT) * sin(heading);
    state->setVelocity(newVx, newVy);
    state->setW(w + fragAcc * fragT);
}

/**
 * @param isloaded: true 负载
 * @param sign:     true 加速, false 减速
 * @param w:        当前角速度 w>0 正向加速
 * @param t:        当前时间
 * @return 加速度大小
 */
double MotionModel::getAngularAcc(bool isloaded, double targetAngularVelocity, double w, double t)
{
    bool sign = abs(targetAngularVelocity) > abs(w);
    if (sign)
    {
        double A = isloaded ? 20.150170343276994 : 38.773244625;
        double eta = 3.86566;
        // 初始加速度
        double a;
        if (w > 0)
        {
            a = sqrt(A * A - 2 * eta * w);
        }
        else
        {
            a = -sqrt(A * A + 2 * eta * w);
        }

        return a - t * eta / 2;
    }
    else
    {

        double A = isloaded ? 20.313264124202313 : 39.087072;
        double eta = 3.89725;
        // 初始加速度
        double a;
        double MAX_W = 3.141420364;
        if (w > 0)
        {
            a = -sqrt(A * A - 2 * eta * (MAX_W - w));
        }
        else
        {
            a = sqrt(A * A - 2 * eta * (MAX_W + w));
        }

        return a - t * eta / 2;
    }
}

// 获取x轴积分结果，加速度为0，角速度不为零
double MotionModel::getIntegralXAngular0(double v0, double linearAcc, double theta0, double omega0, double t)
{
    double item1 = omega0 * (v0 + linearAcc * t) * sin(theta0 + omega0 * t);
    double item2 = linearAcc * cos(theta0 + omega0 * t);
    return (item1 + item2) / (omega0 * omega0);
}

// 获取y轴积分结果，加速度为0，角速度不为零
double MotionModel::getIntegralYAngular0(double v0, double linearAcc, double theta0, double omega0, double t)
{
    double item1 = -omega0 * (v0 + linearAcc * t) * cos(theta0 + omega0 * t);
    double item2 = linearAcc * sin(theta0 + omega0 * t);
    return (item1 + item2) / (omega0 * omega0);
}

/**
 * 获取x轴积分后项结果,也就是对axcos(theta0+omega0t+alpha*t^2/2)求积分
 *
 */
double MotionModel::getIntegralXBack(double linearAcc, double angularAcc, double theta0, double omega0, double t)
{
    // 根据公式计算
    // (1/(ANGULAR_ACC^(3/2)))*v0
    double result = (1 / pow(angularAcc, 1.5)) * linearAcc;

    // -omega0*sqrtPI*cos(((omgea0^2)/(2*ANGULAR_ACC))-theta0)*FresnelC((ANGULAR_ACC*FRAME_TIME+omega0)/(Math.sqrt(ANGULAR_ACC)*sqrtPI))
    double item1 = -c * cos(a) * (t == 0 ? b0c : bc);
    // -omega0*sqrtPI*sin(((omgea0^2)/(2*ANGULAR_ACC))-theta0)*FresnelS((ANGULAR_ACC*FRAME_TIME+omega0)/(Math.sqrt(ANGULAR_ACC)*sqrtPI))
    double item2 = -c * sin(a) * (t == 0 ? b0s : bs);
    // sqrt(ANGULAR_ACC)*sin((ANGULAR_ACC*pow(FRAME_TIME,2)/2)+theta0+omegea0*FRAME_TIME)
    double item3 = sqrt(angularAcc) * sin((angularAcc * (t * t) / 2) + theta0 + omega0 * t);
    return result * (item1 + item2 + item3);
}

// 获取x轴积分前项结果,也就是对v0cos(theta0+omega0t+alpha*t^2/2)求积分
double MotionModel::getIntegralXFront(double v0, double angularAcc, double theta0, double omega0, double t)
{
    // 根据公式计算
    // sqrtPI*v0/sqrt(ANGULAR_ACC)
    double result = sqrtPI * v0 / sqrt(angularAcc);

    // cos((pow(ANGULAR_ACC,2)/2*ANGULAR_ACC)-theta0)*FresnelC((ANGULAR_ACC*FRAME_TIME+omega0)/(sqrt(ANGULAR_ACC)*sqrtPI))
    double item1 = cos(a) * (t == 0 ? b0c : bc);
    // sin((pow(ANGULAR_ACC,2)/2*ANGULAR_ACC)-theta0)*FresnelS((ANGULAR_ACC*FRAME_TIME+omega0)/(sqrt(ANGULAR_ACC)*sqrtPI))
    double item2 = sin(a) * (t == 0 ? b0s : bs);
    return result * (item1 + item2);
}

// 获取y轴积分前项结果，也就是对v0sin(theta0+omega0t+alpha*t^2/2)求积分
double MotionModel::getIntegralYFront(double v0, double angularAcc, double theta0, double omega0, double t)
{
    // 根据公式计算
    // sqrtPI*v0/sqrt(ANGULAR_ACC)
    double result = sqrtPI * v0 / sqrt(angularAcc);

    // cos((pow(ANGULAR_ACC,2)/2*ANGULAR_ACC)-theta0)*FresnelS((ANGULAR_ACC*FRAME_TIME+omega0)/(sqrt(ANGULAR_ACC)*sqrtPI))
    double item1 = cos(a) * (t == 0 ? b0s : bs);
    // -sin((pow(ANGULAR_ACC,2)/2*ANGULAR_ACC)-theta0)*FresnelC((ANGULAR_ACC*FRAME_TIME+omega0)/(sqrt(ANGULAR_ACC)*sqrtPI))
    double item2 = -sin(a) * (t == 0 ? b0c : bc);
    return result * (item1 + item2);
}

// 获取y轴积分后项结果，也就是对axsin(theta0+omega0t+alpha*t^2/2)求积分
double MotionModel::getIntegralYBack(double linearAcc, double angularAcc, double theta0, double omega0, double t)
{
    // 根据公式计算
    // -(1/pow(ANGULAR_ACC,1.5))*v0
    double result = -(1 / pow(angularAcc, 1.5)) * linearAcc;

    double item1 = -c * sin(a) * (t == 0 ? b0c : bc);
    double item2 = c * cos(a) * (t == 0 ? b0s : bs);
    double item3 = sqrt(angularAcc) * cos((angularAcc * (t * t) / 2) + theta0 + omega0 * t);
    return result * (item1 + item2 + item3);
}

// 菲涅尔函数C
double MotionModel::FresnelC(double x)
{
    double xp4 = x * x * x * x;
    double PI2xp4 = PI2 * xp4;
    return x * (1. + PI2xp4 * (-1. / 40. + PI2xp4 * (1. / 3456. - PI2xp4 / 599040.)));
}

// 菲涅尔函数S
double MotionModel::FresnelS(double x)
{
    // 根据公式计算
    double xp3 = x * x * x;
    double xp4 = xp3 * x;
    double PI2xp4 = PI2 * xp4;
    return PI * xp3 * (1. / 6. + PI2xp4 * (-1. / 336. + PI2xp4 * (1. / 42240. - PI2xp4 / 9676800.)));
}

double MotionModel::pow(double x, double p)
{
    if (x < 0)
    {
        return -std::pow(-x, p);
    }
    return std::pow(x, p);
}

double MotionModel::sqrt(double x)
{
    if (x < 0)
    {
        return -std::sqrt(-x);
    }
    return std::sqrt(x);
}
