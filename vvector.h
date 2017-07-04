#ifndef VVECTOR_H
#define VVECTOR_H

#include "vpoint.h"
#include "vmagnification.h"
#include "vsize.h"

class VVector
{
public:

    double x;
    double y;
    VVector(double _x = 0,double _y = 0);
    VVector(const VPoint & from, const VPoint & to);
    VVector(const VPoint & p);

    static double includedAngle(const VVector & from, const VVector & to);
    static double rotationAngle(const VVector & from, const VVector & to);

    const VVector& operator=(const VVector &Right);
    VVector operator +(const VVector & Right)const;
    VPoint operator +(const VPoint & Right)const;
    VVector operator -(const VVector & Right)const;
    VVector operator -()const;
    double operator *(const VVector & Right)const;
    VVector operator *(double Right)const;
    VVector operator *(const VMagnification & Right)const;
    VVector operator /(double Right)const;
    VVector operator /(const VMagnification & Right)const;

    VVector rotate(double angle)const;

    double norm()const;
};

#endif // VVECTOR_H
