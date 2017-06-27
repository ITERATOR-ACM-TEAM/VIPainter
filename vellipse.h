#ifndef VELLIPSE_H
#define VELLIPSE_H

#include "vshape.h"
#include "vpoint.h"

class VEllipse : public VShape{
private:
    double angle;

public:
    VEllipse();
    VEllipse(const VEllipse &shape);
    const VEllipse& operator=(const VEllipse &shape);
    static VEllipse* fromJsonObject(const QJsonObject &jsonObject);

    QImage toImage()override;
    VSize getSize()const override;
    void setSize(const VSize &point)override;
    ~VEllipse()override;
};

#endif // VELLIPSE_H
