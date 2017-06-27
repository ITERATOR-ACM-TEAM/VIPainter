#include "vellipse.h"
#include "cmath"

static double PI=atan(1)*4.0;

VEllipse::VEllipse()
{
}

VEllipse::~VEllipse()
{
}

QImage toImage()
{
}

void resize(const VPoint &point)
{
    VPoint old=size();
    size=size/
}

void rotate(double angle)
{
    this->angle+=angle;
    if(this->angle>PI)this->angle-=PI;
    this->angle-=floor(this->angle/PI)*PI;
}

~VEllipse()
{
}
