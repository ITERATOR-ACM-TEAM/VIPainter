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
