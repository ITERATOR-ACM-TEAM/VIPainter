#include "vshape.h"
#include "vpolygon.h"
#include "vpolyline.h"
#include "vellipse.h"
#include "vcurveline.h"
#include "vgroupshape.h"
#include <vtype.h>
#include <cmath>

double VShape::PI=atan(1)*4.0;

VShape::~VShape()
{
}

VShape* VShape::clone()
{
    return fromJsonObject(toJsonObject());
}

VShape* VShape::fromJsonObject(const QJsonObject &jsonObject)
{
    QString type=jsonObject.value("type").toString();
    if(type==VType::Ellipse)return VEllipse::fromJsonObject(jsonObject);
//    else if(type==VType::Polyline)return VPolyline::fromJsonObject(jsonObject);
//    else if(type==VType::Polygon)return VPolygon::fromJsonObject(jsonObject);
//    else if(type==VType::GroupShape)return VGroupShape::fromJsonObject(jsonObject);
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


QJsonObject VShape::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("angle",this->angle);
    return jsonObject;
}

double VShape::getAngle()const
{
    return angle;
}
