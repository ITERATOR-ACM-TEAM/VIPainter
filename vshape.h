#ifndef VSHAPE_H
#define VSHAPE_H

#include <QImage>
#include "vpoint.h"

class VShape{
public:
    virtual QImage toImage()=0;
    virtual void resize(int weight,int height)=0;
    virtual void rotate(const VPoint &center,double alpha)=0;
    virtual ~VShape();
};

#endif //#ifndef VSHAPE_H
