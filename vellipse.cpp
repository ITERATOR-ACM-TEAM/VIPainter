#include "vellipse.h"
#include "cmath"

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
