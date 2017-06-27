#ifndef VELLIPSE_H
#define VELLIPSE_H

#include "vshape.h"
#include "vpoint.h"

class VEllipse : public VShape{
private:
    VPoint center;
    double angle;

public:
    VEllipse();
    VEllipse(const VEllipse &shape);
    ~VEllipse();
    const VEllipse& operator=(const VEllipse &shape);

    QImage toImage()override;
    virtual VPoint size()override;
    void resize(const VPoint &point)override;
    void rotate(double angle)override;
    ~VEllipse()override;
};

#endif // VELLIPSE_H
