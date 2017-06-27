#include "vshape.h"

VShape::~VShape()
{
}

VShape* VShape::clone()
{
    return fromJsonObject(toJsonObject());
}

VShape* VShape::fromJsonObject(const QJsonObject &jsonObject)
{
}

void VShape::setLocation(const VPoint &location)
{
    this->location=location;
}

VPoint VShape::getLocation()
{
    return location;
}

void VShape::setAngle(double angle)
{
    this->angle=angle;
}

double VShape::getAngle()
{
    return angle;
}
