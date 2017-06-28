#ifndef VELLIPSE_H
#define VELLIPSE_H

#include "vshape.h"
#include "vpoint.h"

class VEllipse : public VShape{
public:
    VEllipse();
    VEllipse(const QJsonObject &jsonObject);
    const VEllipse& operator=(const VEllipse &shape);
    const VEllipse& operator=(const QJsonObject &jsonObject);
    QJsonObject toJsonObject()const override;

    void draw(QPainter *painter)override;
    virtual bool contains(const VPoint &point)override;
    virtual QString type()const override;
    ~VEllipse()override;
};

#endif // VELLIPSE_H
