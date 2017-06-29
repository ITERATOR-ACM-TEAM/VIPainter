#ifndef VELLIPSE_H
#define VELLIPSE_H

#include "vshape.h"
#include "vpoint.h"
#include "vsize.h"

class VEllipse : public VShape{
public:
    VEllipse(const QString &name="", const VPoint &location=VPoint(0,0), const VMagnification &magnification=VMagnification(1,1), double angle=0);
    VEllipse(const QJsonObject &jsonObject);
    const VEllipse& operator=(const VEllipse &shape);
    const VEllipse& operator=(const QJsonObject &jsonObject);
    QJsonObject toJsonObject()const override;

    void draw(QPainter *painter,const VMagnification & magnification)override;
    VSize getSize()override;
    virtual bool contains(VPoint point)override;
    virtual QString type()const override;
    VShape* clone()override;
    ~VEllipse()override;
};

#endif // VELLIPSE_H
