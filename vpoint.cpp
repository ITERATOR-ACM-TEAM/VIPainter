#include "vpoint.h"

VPoint::VPoint():x(0), y(0){}

VPoint::VPoint(double x,double y):x(x), y(y){}

const VPoint& VPoint::operator=(const VPoint &point){
    this->x = point.x;
    this->y = point.y;
    return *this;
}

const VPoint& VPoint::operator=(const QJsonObject &jsonObject)
{
    this->x=jsonObject.value("x").toDouble();
    this->y=jsonObject.value("y").toDouble();
    return *this;
}

VPoint::VPoint(const QJsonObject &jsonObject)
{
    *this=jsonObject;
}

QPointF VPoint::toQPointF()
{
    return QPointF(x,y);
}

VPoint translate(VSize scale)
{
    return VPoint(x*scale.x,y*scale.y);
}

VPoint::operator QJsonValue()const
{
    QJsonObject jsonObject;
    jsonObject.insert("x",x);
    jsonObject.insert("y",y);
    return jsonObject;
}

QJsonObject VPoint::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("x",x);
    jsonObject.insert("y",y);
    return jsonObject;
}
