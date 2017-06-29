#include "vellipse.h"
#include "cmath"
#include <QPainter>
#include <QDebug>
#include <vtype.h>

VEllipse::VEllipse(const QString &name, const VPoint &location, const VSize &size, double angle):VShape(name,location,size,angle)
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
    if(this==&shape)return *this;
    VShape::operator=(shape);
    return *this;
}

void VEllipse::draw(QPainter *painter)
{
    painter->setPen(defaultPen);
    painter->setBrush(defaultBrush);
    painter->drawEllipse(-getSize().x/2.0,-getSize().y/2.0,getSize().x,getSize().y);
}


QJsonObject VEllipse::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
    return jsonObject;
}


bool VEllipse::contains(VPoint point)
{
    double x=point.x;
    double y=point.y;
    double a=getSize().x/2;
    double b=getSize().y/2;
    return (x*x)/(a*a)+(y*y)/(b*b)<=1;
}

QString VEllipse::type()const
{
    return VType::Ellipse;
}
