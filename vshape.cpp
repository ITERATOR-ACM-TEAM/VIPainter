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
#include "vshape.h"
#include "vpolygon.h"
#include "vpolyline.h"
#include "vellipse.h"
#include "vcurveline.h"
#include "vgroupshape.h"
#include "vbezlercurve.h"
#include "vtext.h"
#include "vtype.h"
#include "vpoint.h"
#include <cmath>
#include <QDebug>
#include <QTransform>

const double VShape::PI=atan(1)*4.0;
const QPen VShape::defaultPen(QBrush(Qt::black),1);
const QBrush VShape::defaultBrush(Qt::transparent);

VShape::~VShape()
{
}

VShape::VShape(VShape *parent):parent(parent)
{
}

VShape* VShape::clone()
{
    return fromJsonObject(toJsonObject());
}

VShape* VShape::fromJsonObject(const QJsonObject &jsonObject)
{
    QString type=jsonObject.value("type").toString();
    if(type==VType::Ellipse)return new VEllipse(jsonObject);
    else if(type==VType::Polyline)return new VPolyline(jsonObject);
    else if(type==VType::Polygon)return new VPolygon(jsonObject);
    else if(type==VType::GroupShape)return new VGroupShape(jsonObject);
    else if(type==VType::Curveline)return new VCurveline(jsonObject);
    else if(type==VType::BezlerCurve)return new VBezlerCurve(jsonObject);
    else if(type==VType::Text)return new VText(jsonObject);
    else return nullptr;
}

void VShape::setName(const QString &name)
{
    this->name=name;
}

const QString &VShape::getName()const
{
    return name;
}

VShape::operator QJsonValue()const
{
    return toJsonObject();
}


void VShape::setTransform(const VTransform& transform)
{
    this->transform=transform;
}

VTransform &VShape::getTransform()
{
    return transform;
}

void VShape::zoomin(const VMagnification &mag)
{
    VTransform trans;
    trans.scale(mag);
    transform = transform * trans;
}


VPoint VShape::getLocation()const
{
    return transform.map(VPoint(0,0));
}

VShape::VShape(const QJsonObject jsonObject)
    :name(jsonObject.value("name").toString())
    ,transform(jsonObject.value("transform").toArray())
{
}


const VShape& VShape::operator=(const QJsonObject &jsonObject)
{
    name=jsonObject.value("name").toString();
    transform=jsonObject.value("transform").toArray();
    return *this;
}

QJsonObject VShape::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("type",this->type());
    jsonObject.insert("name",this->getName());
    jsonObject.insert("transform",this->transform.toJsonArray());
    return jsonObject;
}

void VShape::setParent(VShape *parent)
{
    this->parent=parent;
}

VShape * VShape::getParent()const
{
    return parent;
}

QVector<VPoint> VShape::getRect()
{
    QVector<VPoint> points;
    VSize size=getSize()/VMagnification(2);
    VTransform trans = getTransform();
    size=VSize(size.width+crDis,size.height+crDis);

    points.append(VPoint(size.width,size.height));
    points.append(VPoint(0, size.height));
    points.append(VPoint(-size.width,size.height));
    points.append(VPoint(-size.width,0));
    points.append(VPoint(-size.width,-size.height));
    points.append(VPoint(0,-size.height));
    points.append(VPoint(size.width,-size.height));
    points.append(VPoint(size.width,0));
    return points;
}

VPoint VShape::transformPoint(const VPoint & point)
{
    return transform.inverted().map(point);
}

VPoint VShape::reverseTransformPoint(const VPoint &point)
{
    return transform.map(point);
}

void VShape::moveLoc(const VPoint & point)
{
    //qDebug() << point;
    transform.translate(point);
    if(parent == nullptr) return;
    VGroupShape *groupShape=dynamic_cast<VGroupShape*>(getParent());
    if(groupShape!=nullptr)groupShape->getCircumscribedRectangle();
}

void VShape::drawCR(QPainter * painter, const VTransform &trans, double scale)
{
    Q_UNUSED(scale);
    QVector<VPoint> points = this->getRect();
    QList<QPointF> qpoints;
    QPolygonF qpf;
    for(auto &i : points){
        i=i*trans;
        QPointF qpoint =i.toQPointF();
        qpoints.append(qpoint);
        qpf << qpoint;
    }

    QBrush bru;
    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);
    painter->setBrush(bru);
    painter->drawPolygon(qpf);

    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    bru.setStyle(Qt::SolidPattern);
    bru.setColor(Qt::white);
    painter->setPen(pen);
    painter->setBrush(bru);
    for(auto &i : qpoints){
        painter->drawEllipse(i, crDis, crDis);
    }

}

int VShape::atCrPoints(const VPoint & point,double scale)
{
    QVector<VPoint> points = this->getRect();
    VSize siz(crDis/scale,crDis/scale);
    int cnt = 0;
    for(auto it: points)
    {
        VPoint pos = VPoint(point.x-it.x, point.y-it.y);
        //qDebug() << pow(pos.x * siz.height, 2)+pow(pos.y*siz.width, 2) << pow(siz.width*siz.height, 2);
        if(pow(pos.x * siz.height, 2)+pow(pos.y*siz.width, 2) <= pow(siz.width*siz.height, 2))
            return cnt;
        cnt++;
    }
    return -1;
}

void VShape::changeMag(int i, const VVector & vec)
{
    static const VMagnification mark[4] =
    {
      {1, 1}, {-1, 1},{-1,-1},{1,-1}
    };
    if(i<0 || i>=8) return;
    QVector<VPoint> points = this->getRect();
    VPoint crp = points[i];
    VVector co(VPoint(0,0), crp);
    VVector mov;
    if(i % 2)
    {
        double dis = (VVector(vec.x-crDis, vec.y-crDis)*co) / co.norm();
        if(i % 4 == 1)
        {
            mov = VVector((this->getSize().width)/2, dis);
        }
        else
        {
            mov = VVector(dis, (this->getSize().height)/2);
        }
    }
    else
    {
        mov = VVector(vec.x, vec.y) * mark[i/2] - VVector(crDis, crDis);
    }
    mov = mov*2;
    //this->setLocation(this->reverseTransform((mov+VPoint(0,0))/mag));
    this->zoomin(VMagnification(std::max(mov.x, 1.0)/(this->getSize().width)
                                        ,std::max(mov.y, 1.0)/(this->getSize().height)
                                        )
                          );
}
