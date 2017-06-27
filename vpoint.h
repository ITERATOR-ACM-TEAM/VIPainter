#ifndef VPOINT_H
#define VPOINT_H

#include <QPointF>

class VPoint{   
public:
    double x;
    double y;
    VPoint();
    VPoint(double x,double y);

    const VPoint& operator=(const VPoint &point);
};

#endif //#ifndef VPOINT_H
