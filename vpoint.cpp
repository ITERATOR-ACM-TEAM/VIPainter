#include "vpoint.h"

VPoint::VPoint():x(0), y(0){}

VPoint::VPoint(double x,double y):x(x), y(y){}

const VPoint& VPoint::operator=(const VPoint &point){
    this->x = point.x;
    this->y = point.y;
    return *this;
}

QPointF VPoint::toQPointF()
{
    return QPointF(x,y);
}

QJsonObject VPoint::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("x",x);
    jsonObject.insert("y",y);
    return jsonObject;
}

VPoint VPoint::fromJsonObject(const QJsonObject &jsonObject)
{
    return VPoint(jsonObject.value("x").toDouble(),jsonObject.value("y").toDouble());
}
