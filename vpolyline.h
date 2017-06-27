#ifndef VPOLYLINE_H
#define VPOLYLINE_H

#include "vshape.h"
#include "QList"
#include "vpoint.h"

class VPolyline:public VShape
{
public:
    VPolyline();
    VPolyline(const VPolyline &polyline);
    virtual move(int i,int x,int y);//move the ith point to (x,y)
    virtual QList<Vpoint> getPointList();
    const VPolyline& operator=(const VPolyline &polyline);
};

#endif // VPOLYLINE_H
