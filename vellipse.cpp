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

const VEllipse& VEllipse::operator=(const VEllipse &shape)
{
    this->size=shape.size;
    return *this;
}

QImage VEllipse::toImage()
{
    QImage image(size.x+1,size.y+1,QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.drawEllipse(0,0,size.x,size.y);
    return image;
}

VEllipse* VEllipse::fromJsonObject(const QJsonObject &jsonObject)
{
    VEllipse *ellipse=new VEllipse();
    ellipse->size=VSize::fromJsonObject(jsonObject.value("size").toObject());
    return ellipse;
}

QJsonObject VEllipse::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
    jsonObject.insert("size",size.toJsonObject());
    return jsonObject;
}


bool VEllipse::contains(const VPoint &point)
{
    double x=point.x-location.x;
    double y=point.y-location.y;
    double a=size.x/2;
    double b=size.y/2;
    return (x*x)/(a*a)+(y*y)/(b*b)<=1;
}

QString VEllipse::type()const
{
    return VType::Ellipse;
}

void VEllipse::setSize(const VSize &size)
{
    this->size=size;
}

VSize VEllipse::getSize() const
{
    return size;
}
