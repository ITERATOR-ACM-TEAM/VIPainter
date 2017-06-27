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
    QJsonObject toJsonObject()const override;

    QImage toImage()override;
    VSize getSize()const override;
    void setSize(const VSize &point)override;
    virtual bool contains(const VPoint &point)override;
    virtual QString type()const override;
    ~VEllipse()override;
};

#endif // VELLIPSE_H
