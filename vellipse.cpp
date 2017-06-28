#include "vellipse.h"
#include "cmath"
#include <QPainter>
#include <vtype.h>

VEllipse::VEllipse()
{
}

VEllipse::~VEllipse()
{
}

VEllipse::VEllipse(const QJsonObject &jsonObject)
{
    *this=jsonObject;
}


const VEllipse& VEllipse::operator=(const QJsonObject &jsonObject)
{
    VShape::operator=(jsonObject);
    return *this;
}

const VEllipse& VEllipse::operator=(const VEllipse &shape)
{
    VShape::operator=(shape);
    return *this;
}

void VEllipse::draw(QPainter *painter)
{
    painter->drawEllipse(-size.x/2.0,-size.y/2.0,size.x,size.y);
}


QJsonObject VEllipse::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
    return jsonObject;
}


bool VEllipse::contains(const VPoint &point)
{
    double x=point.x;
    double y=point.y;
    double a=size.x/2;
    double b=size.y/2;
    return (x*x)/(a*a)+(y*y)/(b*b)<=1;
}

QString VEllipse::type()const
{
    return VType::Ellipse;
}
