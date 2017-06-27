#ifndef VSHAPE_H
#define VSHAPE_H

#include <QImage>
#include "vpoint.h"

class VShape{
public:
    virtual QImage toImage()=0;
    virtual VPoint size()=0;//返回右下角的位置
    virtual void resize(const VPoint &point)=0;//把外接举行的右下角移动到point
    virtual void rotate(const VPoint &center,double alpha)=0;
    virtual ~VShape();
};

#endif //#ifndef VSHAPE_H
