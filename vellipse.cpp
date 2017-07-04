/**
 * Copyright (C) 2017 kkkeQAQ
 *               2017 Bcai0797
 *               2017 Penn000
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
    VPoint p1(1,0);
    VPoint p2=transform.map(p1);
    VPoint loc=transform.map(VPoint(0,0));
    p2=VPoint(p2.x-loc.x,p2.y-loc.y);
    double angle=VVector::rotationAngle(VVector(p1),VVector(p2));
    painter->translate(loc.x,loc.y);
    painter->rotate(angle);
    double width=transform.map(VPoint(1,0))-transform.map(VPoint(0,0));
    double height=transform.map(VPoint(0,1))-transform.map(VPoint(0,0));
//    QTransform ptrans=painter->worldTransform();
//    painter->scale(1/ptrans.m11(),1/ptrans.m22());
//    qDebug()<<painter->worldTransform();
//    qDebug()<<ptrans.m11()<<ptrans.m22();
    //qDebug()<<width<<height;
    painter->drawEllipse(-width/2.0,-height/2.0,width,height);
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
