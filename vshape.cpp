#include "vshape.h"
#include "vpolygon.h"
#include "vpolyline.h"
#include "vellipse.h"
#include "vcurveline.h"
#include "vgroupshape.h"
#include <vtype.h>
#include <vpoint.h>
#include <cmath>
#include <QDebug>

const double VShape::PI=atan(1)*4.0;
const QPen VShape::defaultPen(QBrush(Qt::black),1);
const QBrush VShape::defaultBrush(Qt::transparent);

VShape::~VShape()
{
}

VShape::VShape(const QString &name, const VPoint &location, const VMagnification &magnification, double angle,VShape *parent):
    name(name),location(location),magnification(magnification),angle(angle),parent(parent)
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
    else return nullptr;
}


void VShape::zoomin(VMagnification magnification)
{
    this->magnification=this->magnification*magnification;
}

void VShape::setLocation(const VPoint &location)
{
    this->location=location;
}


VPoint VShape::getLocation()const
{
    return location;
}

void VShape::setAngle(double angle)
{
    this->angle=angle;
}

void VShape::setName(const QString &name)
{
    this->name=name;
}

QString VShape::getName()const
{
    return name;
}

VShape::operator QJsonValue()const
{
    return toJsonObject();
}

VShape::VShape(const QJsonObject jsonObject)
{
    name=jsonObject.value("name").toString();
    magnification=jsonObject.value("magnification").toObject();
    angle=jsonObject.value("angle").toDouble();
    location=jsonObject.value("location").toObject();
}


const VShape& VShape::operator=(const QJsonObject &jsonObject)
{
    name=jsonObject.value("name").toString();
    magnification=jsonObject.value("magnification").toObject();
    angle=jsonObject.value("angle").toDouble();
    location=jsonObject.value("location").toObject();
    return *this;
}

QJsonObject VShape::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("type",this->type());
    jsonObject.insert("name",this->getName());
    jsonObject.insert("magnification",this->getMagnification());
    jsonObject.insert("angle",this->getAngle());
    jsonObject.insert("location",this->getLocation());
    return jsonObject;
}

double VShape::getAngle()const
{
    return angle;
}

void VShape::setMagnification(const VMagnification &magnification)
{
    this->magnification=magnification;
}

VMagnification VShape::getMagnification()const
{
    return magnification;
}

void VShape::setParent(VShape *parent)
{
    this->parent=parent;
}

VShape * VShape::getParent()const
{
    return parent;
}

QList<VPoint> VShape::getRect()
{
    QList<VPoint> points;
    VSize size=getSize()/VMagnification(1.8);
    VPoint center(0,0);
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

VPoint VShape::translate(const VPoint & point)
{
    VPoint subLocation(getLocation());
    double subAngle = getAngle();
    VMagnification subMag = getMagnification();
    return VPoint(point.x - subLocation.x, point.y - subLocation.y).rotate(VPoint(0,0),-subAngle)/subMag;
}

VPoint VShape::reverseTransform(const VPoint &point)
{
    VPoint subLocation(getLocation());
    double subAngle = getAngle();
    VMagnification subMag = getMagnification();
    return (point*subMag).rotate(VPoint(0, 0), subAngle)+subLocation;
}

void VShape::moveLoc(const VPoint & point)
{
    setLocation(point);
    if(parent == nullptr) return;
    VGroupShape *groupShape=dynamic_cast<VGroupShape*>(getParent());
    if(groupShape!=nullptr)groupShape->getCircumscribedRectangle();
}

void VShape::drawCR(QPainter * painter, const VMagnification &mag)
{

    QList<VPoint> points = this->getRect();
    QList<QPointF> qpoints;
    QPolygonF qpf;
    for(auto &i : points){
        QPointF qpoint = (i*mag).toQPointF();
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
        painter->drawEllipse(i, 2, 2);
    }

}
