#include "vsize.h"

VSize::VSize():x(0), y(0){}

VSize::VSize(double x,double y):x(x), y(y){}

const VSize& VSize::operator=(const VSize &size){
    this->x = size.x;
    this->y = size.y;
    return *this;
}

VSize VSize::operator +(const VSize & Right)
{
    return VSize(x+Right.x, y+Right.y);
}

VSize VSize::operator -(const VSize & Right)
{
    return VSize(x-Right.x, y-Right.y);
}

QJsonObject VSize::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("x",x);
    jsonObject.insert("y",y);
    return jsonObject;
}

VSize VSize::fromJsonObject(const QJsonObject &jsonObject)
{
    return VSize(jsonObject.value("x").toDouble(),jsonObject.value("y").toDouble());
}
