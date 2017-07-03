#ifndef VPOINT_H
#define VPOINT_H

#include "vsize.h"
#include <QPointF>
#include <QJsonObject>
#include <vmagnification.h>

class VPoint{   
public:
    double x;
    double y;
    VPoint();
    VPoint(double x,double y);
    VPoint(const QJsonObject &jsonObject);
    VPoint(const QPointF &pointf);
    QPointF toQPointF();

    const VPoint& operator=(const VPoint &point);
    const VPoint& operator=(const QJsonObject &jsonObject);

    operator QJsonValue()const;
    QJsonObject toJsonObject()const;
    VPoint operator*(const VMagnification &magnification)const;
    VPoint operator/(const VMagnification &magnification)const;
    double operator-(const VPoint &point)const;
    VPoint operator-()const;
    VPoint operator+(const VSize &size)const;
    VPoint operator+(const VPoint &size)const;
    VPoint operator-(const VSize &size)const;

    VPoint rotate(const VPoint & center, double a);
    VPoint centralTransformation();//中心变换
};

#endif //#ifndef VPOINT_H
