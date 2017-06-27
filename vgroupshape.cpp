#include "vgroupshape.h"

VGroupShape::VGroupShape()
{
    //TODO:
}

VGroupShape::~VGroupShape()
{
    //TODO:
}

VGroupShape::VGroupShape(const VGroupShape &shape)
{
    for(auto & it : shape.ShapeVector)
    {
        this->ShapeVector.push_back(it);
    }

}

const VGroupShape & VGroupShape:: operator=(const VGroupShape &shape)
{
    this->ShapeVector.clear();
    for(auto & it : shape.ShapeVector)
    {
        this->ShapeVector.push_back(it);
    }
    return this;
}

void VGroupShape::addShape(VShape * other, const VPoint & location)
{
    this->ShapeVector.push_back(make_pair(other, location));
}

void VGroupShape::moveShape(int i, const VPoint & location)
{
    this->ShapeVector[i].second = location;
}

QVector<VShape *> VGroupShape::getShapeVector()
{
    QVector<VShape *> result;
    for(auto & it : this->ShapeVector)
    {
        result.push_back(it.first);
    }
    return result;
}

void VGroupShape::rotate(const VPoint &center, double alpha)
{
    for(auto & it : this->ShapeVector)
    {
        it.first.rotate(center, alpha);
    }
}

