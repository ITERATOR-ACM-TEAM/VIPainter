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
    const VEllipse& operator=(const VEllipse &shape);
    static VEllipse* fromJsonObject(const QJsonObject &jsonObject);

    QImage toImage()override;
    virtual VPoint size()override;
    void resize(const VPoint &point)override;
    ~VEllipse()override;
};

#endif // VELLIPSE_H
