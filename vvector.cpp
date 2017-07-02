#include "vvector.h"
#include <cmath>

VVector::VVector(double _x,double _y):x(_x), y(_y)
{
}

VVector::VVector(const VPoint & from, const VPoint & to)
{
    x=to.x - from.x;
    y=to.y - from.y;
}

const VVector& VVector::operator=(const VVector &Right)
{
    x = Right.x;
    y = Right.y;
    return *this;
}

VVector VVector::operator +(const VVector & Right)const
{
    return VVector(x+Right.x, y+Right.y);
}

VVector VVector::operator -(const VVector & Right)const
{
    return VVector(x-Right.x, y-Right.y);
}

double VVector::operator *(const VVector & Right)const
{
    return x*Right.x + y*Right.y;
}

VVector VVector::operator *(double Right)const
{
    return VVector(Right*x, Right*y);
}

double VVector::norm()const
{
    return sqrt(x*x+ y*y);
}

VPoint VVector::operator +(const VPoint & Right)const
{
    return VPoint(Right.x+x, Right.y+y);
}

VVector VVector::operator /(double Right)const
{
    return VVector(x/Right , y/Right);
}

VVector VVector::operator /(const VMagnification & Right)const
{
    return VVector(x/Right.horizontal, y/Right.vertical);
}

VVector VVector::operator *(const VMagnification & Right)const
{
    return VVector(x*Right.horizontal, y*Right.vertical);
}

VVector VVector::operator -()const
{
    return (*this)*(-1);
}

VVector VVector::rotate(double angle)const
{
    return VVector(VPoint(0,0), ((*this)+VPoint(0,0)).rotate(VPoint(0,0),angle));
}
