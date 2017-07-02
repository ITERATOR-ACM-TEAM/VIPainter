#include "vellipse.h"
#include "cmath"
#include <QPainter>
#include <QDebug>
#include <vtype.h>

VEllipse::VEllipse(const QString &name, const VPoint &location, const VMagnification &magnification, double angle):VShape(name,location,magnification,angle)
{
}

VEllipse::~VEllipse()
{
}

VEllipse::VEllipse(const QJsonObject &jsonObject):VShape(jsonObject)
{
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

void VEllipse::draw(QPainter *painter, const VMagnification &magnification)
{
    painter->setPen(defaultPen);
    painter->setBrush(defaultBrush);
    painter->drawEllipse(-magnification.horizontal/2.0,-magnification.vertical/2.0,magnification.horizontal,magnification.vertical);
}


QJsonObject VEllipse::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
    return jsonObject;
}

VShape* VEllipse::clone()
{
    return new VEllipse(*this);
}

bool VEllipse::contains(VPoint point)
{
    return point.x*point.x*4+point.y*point.y*4 <= 1;
}

QString VEllipse::type()const
{
    return VType::Ellipse;
}

VSize VEllipse::getSize()
{
    return VSize(1,1);
}
