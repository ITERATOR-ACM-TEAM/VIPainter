#include "vshape.h"
#include <vtype.h>

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
//    QString type=jsonObject.value("type").toString();
//    if(type==VType::Polygon)return VPolygon::fromJsonObject(jsonObject);
//    else if(type==VType::Polyline)return VPolyline::fromJsonObject(jsonObject);
//    else if(type==VType::Ellipse)return VEllipse::fromJsonObject(jsonObject);
//    else if(type==VType::GroupShap)return VGroupShap::fromJsonObject(jsonObject);
//    else return nullptr;
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

double VShape::getAngle()const
{
    return angle;
}
