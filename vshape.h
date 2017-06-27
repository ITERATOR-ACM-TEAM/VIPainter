#ifndef VSHAPE_H
#define VSHAPE_H

#include <QImage>
#include "vpoint.h"

class VShape{
public:
    virtual QImage toImage()=0;
    virtual VPoint size()=0;
    virtual void resize(const VPoint &point)=0;
    virtual void rotate(double alpha)=0;
    virtual ~VShape();
};

#endif //#ifndef VSHAPE_H
