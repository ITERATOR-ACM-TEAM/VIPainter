#ifndef VVECTOR_H
#define VVECTOR_H

#include "vpoint.h"

class VVector
{
public:

    double x;
    double y;
    VVector(double _x = 0,double _y = 0);
    VVector(const VPoint & from, const VPoint & to);

    const VVector& operator=(const VVector &Right);
    VVector operator +(const VVector & Right)const;
    VVector operator -(const VVector & Right)const;
    double operator *(const VVector & Right)const;
    VVector operator *(double Right)const;

    double norm()const;
};

#endif // VVECTOR_H
