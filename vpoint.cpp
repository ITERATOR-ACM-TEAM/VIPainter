#include "vpoint.h"
#include "vshape.h"
#include <cmath>
#include <cfloat>

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

VPoint VPoint::operator*(const VMagnification &magnification)const
{
    return VPoint(x*magnification.horizontal,y*magnification.vertical);
}


VPoint VPoint::operator/(const VMagnification &magnification)const
{
    bool flag=true;
    VPoint point;
    if(std::abs(magnification.horizontal)<1e-9)
    {
        if(std::abs(x)<1e-9)point.x=0;
        else point.x=DBL_MAX;
        flag=false;
    }
    else point.x=x/magnification.horizontal;
    if(std::abs(magnification.vertical)<1e-9)
    {
        if(std::abs(y)<1e-9)point.y=0;
        else point.y=DBL_MAX;
        flag=false;
    }
    else point.y=y/magnification.vertical;
    return point;
}

double VPoint::operator-(const VPoint &point)const
{
    return sqrt(pow(point.x-x,2)+pow(point.y-y,2));
}


VPoint VPoint::operator-()const
{
    return VPoint(-x,-y);
}

VPoint VPoint::operator+(const VSize &size)const
{
    return VPoint(x+size.width,y+size.height);
}

VPoint VPoint::operator+(const VPoint &point)const
{
    return VPoint(x+point.x,y+point.y);
}

VPoint VPoint::operator-(const VSize &size)const
{
    return VPoint(x-size.width,y-size.height);
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

VPoint VPoint::rotate(const VPoint & center, double a)
{
    VPoint point(*this);
    a = a / 180 * VShape::PI;
    double x = point.x - center.x;
    double y = point.y - center.y;
    point.x = x*cos(a) - y*sin(a) + center.x;
    point.y = x*sin(a) + y*cos(a) + center.y;
    return point;
}

VPoint VPoint::centralTransformation(){//中心变换
    VPoint point(y, x);
    return point;
}
