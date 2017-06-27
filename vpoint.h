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
    QPointF toQPointF();

    const VPoint& operator=(const VPoint &point);
    QJsonObject toJsonObject()const;
    static VPoint fromJsonObject(const QJsonObject &jsonObject);
};

#endif //#ifndef VPOINT_H
