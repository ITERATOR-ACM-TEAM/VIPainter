#ifndef VELLIPSE_H
#define VELLIPSE_H

#include "vshape.h"

class VEllipse : public VShape{
private:

public:
    VEllipse();
    VEllipse(const VEllipse &shape);
    const VEllipse& operator=(const VEllipse &shape);
};

#endif // VELLIPSE_H
