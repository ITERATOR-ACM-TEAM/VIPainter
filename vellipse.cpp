#include "vellipse.h"
#include "cmath"
#include <QPainter>
#include <QDebug>
#include <vtype.h>
#include <QTransform>

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

void VEllipse::draw(QPainter *painter, const VTransform &transform)
{
    painter->setPen(defaultPen);
    painter->setBrush(defaultBrush);
    QTransform qtrans(1,transform.m12(),transform.m13()
                         ,transform.m21(),1,transform.m23()
                         ,transform.m31(),transform.m32(),transform.m33());
    painter->setTransform(qtrans);
    painter->drawEllipse(-transform.m11()/2.0,-transform.m22()/2.0,transform.m11(),transform.m22());
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
