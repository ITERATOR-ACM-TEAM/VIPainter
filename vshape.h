#ifndef VSHAPE_H
#define VSHAPE_H

#include <QList>
#include <pair>
#include "vpoint.h"

class VShape{
public:
    virtual QList<VPoint> toImage(int w,int h,int alpha)=0;
    VShape();
    VShape(const VShape &shape);
    VShape(VShape &&shape);
    virtual ~VShape()=0;

};

#endif //#ifndef VSHAPE_H
