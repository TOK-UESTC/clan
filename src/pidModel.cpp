#include "pidModel.h"

PIDModel::PIDModel()
{
}
void PIDModel::set(const PIDModel &s)
{
    this->KpAngle = s.KpAngle;
    this->KiAngle = s.KiAngle;
    this->KdAngle = s.KdAngle;
    this->lastErrAngle = s.lastErrAngle;
    this->intAngle = s.intAngle;
    this->intMaxAngle = s.intMaxAngle;
    this->KpDist = s.KpDist;
    this->KiDist = s.KiDist;
    this->KdDist = s.KdDist;
    this->KpDistLoad = s.KpDistLoad;
    this->KiDistLoad = s.KiDistLoad;
    this->KdDistLoad = s.KdDistLoad;
    this->lastErrDist = s.lastErrDist;
    this->intDist = s.intDist;
    this->intMaxDist = s.intMaxDist;
    // this->rb = model.rb;
}

void PIDModel::set(double KpDist, double KiDist, double KdDist, double KpDistLoad, double KiDistLoad,
                   double KdDistLoad)
{
    this->KpDist = KpDist;
    this->KiDist = KiDist;
    this->KdDist = KdDist;
    this->KpDistLoad = KpDistLoad;
    this->KiDistLoad = KiDistLoad;
    this->KdDistLoad = KdDistLoad;
}
