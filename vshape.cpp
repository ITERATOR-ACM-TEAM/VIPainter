#include "vshape.h"
#include "vpolygon.h"
#include "vpolyline.h"
#include "vellipse.h"
#include "vcurveline.h"
#include "vgroupshape.h"
#include <vtype.h>
#include <cmath>
#include <QDebug>

const double VShape::PI=atan(1)*4.0;
const QPen VShape::defaultPen(QBrush(Qt::black),1);
const QBrush VShape::defaultBrush(Qt::transparent);

VShape::~VShape()
{
}

VShape::VShape(const QString &name, const VPoint &location, const VSize &size, double angle):
    name(name),location(location),size(size),angle(angle)
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

void VShape::setLocation(const VPoint &location)
{
    this->location=location;
}


VSize VShape::getLogicalSize()
{
    return this->size;
}

VPoint VShape::getLocation()const
{
    return location;
}

void VShape::setAngle(double angle)
{
    this->angle=angle;
}

VSize VShape::getTranslate()
{
    VSize logicalSize=getLogicalSize();
    return VSize(size.x/logicalSize.x,size.y/logicalSize.y);
}

void VShape::setName(QString name)
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
    setName(jsonObject.value("name").toString());
    setSize(jsonObject.value("size").toObject());
    setAngle(jsonObject.value("angle").toDouble());
    setLocation(jsonObject.value("location").toObject());
}


const VShape& VShape::operator=(const QJsonObject &jsonObject)
{
    setName(jsonObject.value("name").toString());
    setSize(jsonObject.value("size").toObject());
    setAngle(jsonObject.value("angle").toDouble());
    setLocation(jsonObject.value("location").toObject());
    return *this;
}

QJsonObject VShape::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("type",this->type());
    jsonObject.insert("name",this->getName());
    jsonObject.insert("size",this->getSize());
    jsonObject.insert("angle",this->getAngle());
    jsonObject.insert("location",this->getLocation());
    return jsonObject;
}

double VShape::getAngle()const
{
    return angle;
}

VSize VShape::getSize()const
{
    return size;
}

void VShape::setSize(const VSize &size)
{
    this->size=size;
    // qDebug()<<size<<endl;
}
