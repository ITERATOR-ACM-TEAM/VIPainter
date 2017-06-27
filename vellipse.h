#ifndef VELLIPSE_H
#define VELLIPSE_H

#include "vshape.h"
#include "vpoint.h"

class VEllipse : public VShape{
private:
    VPoint center;
    double alpha;

public:
    VEllipse();
    VEllipse(const VEllipse &shape);
    ~VEllipse();
    const VEllipse& operator=(const VEllipse &shape);

    QImage toImage()override;
    void resize(int weight,int height)override;
    void rotate(const VPoint &center,double alpha)override;
    ~VEllipse()override;
};

#endif // VELLIPSE_H
