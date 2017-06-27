#include "vellipse.h"
#include "cmath"

static double PI=atan(1)*4.0;

VEllipse::VEllipse()
{
}

VEllipse::~VEllipse()
{
}

QImage VEllipse::toImage()
{
}

void VEllipse::setSize(const VSize &size)
{
    this->size=size;
}

VSize VEllipse::getSize()
{
    return size;
}
