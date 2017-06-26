#ifndef VELLIPSE_H
#define VELLIPSE_H

#include "vshape.h"

class VEllipse : public VShape{
public:
    VEllipse();
    VEllipse(const VEllipse &shape);
    ~VEllipse();
    const VEllipse& operator=(const VEllipse &shape);
};

#endif // VELLIPSE_H
