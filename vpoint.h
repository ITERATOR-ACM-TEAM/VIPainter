#ifndef VPOINT_H
#define VPOINT_H

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
};

#endif //#ifndef VPOINT_H
