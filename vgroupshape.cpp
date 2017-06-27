#include "vgroupshape.h"
#include <QPainter>
#include <QPointF>
#include <math.h>

VGroupShape::VGroupShape()
{
    //TODO:
}

VGroupShape::~VGroupShape()
{
    for(auto &it : shape.ShapeVector)
    {
        if(it != nullptr) delete it;
    }
}

VGroupShape::VGroupShape(const VGroupShape &shape)
{
    for(auto & it : shape.ShapeVector)
    {
        this->ShapeVector.push_back(it->clone());
    }

}

const VGroupShape & VGroupShape:: operator=(const VGroupShape &shape)
{
    this->ShapeVector.clear();
    for(auto & it : shape.ShapeVector)
    {
        this->ShapeVector.push_back(it->clone());
    }
    return this;
}

void VGroupShape::addShape(VShape * other)
{
    this->ShapeVector.push_back(other);
}

void VGroupShape::moveShape(int i, const VPoint & location)
{
    this->ShapeVector[i]->setLocation(location);
}

QVector<VShape *> VGroupShape::getShapeVector()
{
    return ShapeVector;
}

VSize VGroupShape::getSize()
{
    double minX, minY;
    double maxX, maxY;
    for(auto & it : this->ShapeVector)
    {
        VPoint loc = it->first.size()+it->second;
        maxX = max(maxX, loc.x);
        maxY = max(maxY, loc.y);
        minX = min(minX, loc.x);
        minY = min(minY, loc.y);
    }
    for(auto & it : this->ShapeVector)
    {
        it->location.x -= minX;
        it->location.y -= minY;
    }
    return VSize(maxX-minX, maxY-minY);
}

void VGroupShape::setSize(const VSize &size)
{
    VSize siz = this->getSize();
    double fractionX = size.x/siz.x , factionY = size.y / siz.y;
    for(auto & it : this->ShapeVector)
    {
        it->location.x *= fractionX;
        it->location.y *= fractionY;
        VSize subSiz = it->getSize();
        subSiz.x *= fractionX;
        subSiz.y *= fractionY;
        it->setSize(subSiz);
    }
}

QImage VGroupShape::toImage()
{
    VPoint sz = this->size();
    int width = sz.y, height = sz.x;
    QImage image(width, height, QImage::Format_ARGB32);
    QPainter painter(&image);
    for(auto & it : this->ShapeVector)
    {
        painter.drawImage(QPointF(it->location.x - this->location.x, QPointF(it->location.y - this->location.y), it->toImage());
    }
    return image;
}
