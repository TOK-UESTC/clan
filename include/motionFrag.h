#ifndef MOTIONFRAG_H
#define MOTIONFRAG_H

class MotionFrag
{
private:
    double t;
    double linearAcc;
    double angularAcc;

public:
    MotionFrag()
    {
        this->t = 0;
        this->linearAcc = 0.0;
        this->angularAcc = 0.0;
    }
    MotionFrag(double t, double linearAcc, double angularAcc)
    {
        this->t = t;
        this->linearAcc = linearAcc;
        this->angularAcc = angularAcc;
    }
    double getT()
    {
        return t;
    }
    double getLinearAcc()
    {
        return linearAcc;
    }
    double getAngularAcc()
    {
        return angularAcc;
    }
    void update(double t, double linearAcc, double angularAcc)
    {
        this->t = t;
        this->linearAcc = linearAcc;
        this->angularAcc = angularAcc;
    }
};

#endif
