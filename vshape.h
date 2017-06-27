#ifndef VSHAPE_H
#define VSHAPE_H

#include <QImage>
#include "vpoint.h"

class VShape{
public:
    virtual QImage toImage();
    virtual void resize(int weight,int height);
    virtual void rotate(double alpha);
    virtual ~VShape();
};

#endif //#ifndef VSHAPE_H
