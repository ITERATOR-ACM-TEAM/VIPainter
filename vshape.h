#ifndef VSHAPE_H
#define VSHAPE_H

#include <QList>
#include "vpoint.h"

class VShape{
public:
    virtual QList<VPoint> toImage(int w,int h,double alpha)=0;
    virtual ~VShape();
};

#endif //#ifndef VSHAPE_H
