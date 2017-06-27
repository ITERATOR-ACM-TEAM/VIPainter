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
    QImage image(size.x,size.y,QImage::Format_ARGB32);
    image.fill(0);
    QPainter painter(&image);
    painter.setPen(QPen(Qt::red,2));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(0,0,size.x,size.y);
    //painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::yellow,2,Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
    painter.drawRect(0,0,size.x,size.y);
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
