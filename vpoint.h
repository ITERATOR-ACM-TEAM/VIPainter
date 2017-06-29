#ifndef VPOINT_H
#define VPOINT_H

#include "vsize.h"
#include <QPointF>
#include <QJsonObject>

class VPoint{   
public:
    double x;
    double y;
    VPoint();
    VPoint(double x,double y);
    VPoint(const QJsonObject &jsonObject);
    QPointF toQPointF();

    const VPoint& operator=(const VPoint &point);
    const VPoint& operator=(const QJsonObject &jsonObject);

    operator QJsonValue()const;
    QJsonObject toJsonObject()const;
    VPoint translate(VSize scale);
    VPoint retranslate(VSize scale);
    VPoint operator*(VSize scale);
    VPoint operator/(VSize scale);

    VPoint rotate(const VPoint & center, double a);
};

#endif //#ifndef VPOINT_H
