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

QImage VEllipse::toImage()
{
    QImage image(size.x+2,size.y+2,QImage::Format_ARGB32);
    image.fill(0);
    QPainter painter(&image);
    painter.translate(1,1);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawEllipse(0,0,size.x,size.y);
    //painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::yellow,1,Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
    //painter.drawRect(0,0,size.x,size.y);
    return image;
}


QJsonObject VEllipse::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
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
