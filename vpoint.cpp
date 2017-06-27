#include "vpoint.h"

VPoint::VPoint():x(0), y(0){}

VPoint::VPoint(double x,double y):x(x), y(y){}

const VPoint& VPoint::operator=(const VPoint &point){
    this->x = point.x;
    this->y = point.y;
    return *this;
}


QPointF VPoint::toQPointF()
{
    return QPointF(x,y);
}
