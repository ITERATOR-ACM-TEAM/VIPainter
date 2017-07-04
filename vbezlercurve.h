#ifndef VBEZLERSHAPE_H
#define VBEZLERSHAPE_H

#include "vshape.h"
#include "vpoint.h"
#include "vpointgroupshape.h"

class VBezlerCurve : public VPointGroupShape{
public:
    VBezlerCurve();
    VBezlerCurve(const VBezlerCurve &shape);
    VBezlerCurve(const QJsonObject &jsonObject);
    ~VBezlerCurve()override;
    const VBezlerCurve& operator=(const VBezlerCurve &shape);
    const VBezlerCurve& operator=(const QJsonObject &jsonObject);
    bool contains(VPoint point) override;
    //QImage toImage()override;
    void draw(QPainter *painter,const VTransform &transform)override;
    VShape* clone()override;
    QString type()const override;
    void drawCR(QPainter * painter, const VTransform &trans, double scale);
};

#endif // VBEZLERSHAPE_H
