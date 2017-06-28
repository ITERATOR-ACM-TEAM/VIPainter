#include "vshape.h"
#include "vpolygon.h"
#include "vpolyline.h"
#include "vellipse.h"
#include "vcurveline.h"
#include "vgroupshape.h"
#include <vtype.h>
#include <cmath>

const double VShape::PI=atan(1)*4.0;

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
//    else if(type==VType::Polyline)return new VPolyline(jsonObject);
//    else if(type==VType::Polygon)return new VPolygon(jsonObject);
    else if(type==VType::GroupShape)return new VGroupShape(jsonObject);
    else return nullptr;
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
    name=jsonObject.value("name").toString();
    size=jsonObject.value("size").toObject();
    angle=jsonObject.value("angle").toDouble();
    location=jsonObject.value("location").toObject();
}


const VShape& VShape::operator=(const QJsonObject &jsonObject)
{
    name=jsonObject.value("name").toString();
    size=jsonObject.value("size").toObject();
    angle=jsonObject.value("angle").toDouble();
    location=jsonObject.value("location").toObject();
    return *this;
}

QJsonObject VShape::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("type",this->type());
    jsonObject.insert("name",this->name);
    jsonObject.insert("size",this->size);
    jsonObject.insert("angle",this->angle);
    jsonObject.insert("location",this->location);
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
}
