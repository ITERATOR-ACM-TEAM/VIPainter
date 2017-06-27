#include "vgroupshape.h"
#include <QPainter>
#include <QPointF>

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
        it.first.rotate(center-it.second, alpha);
    }
}

VPoint VGroupShape::size()
{
    double minX, minY;
    double maxX, maxY;
    for(auto & it : this->ShapeVector)
    {
        VPoint loc = it.first.size()+it.second;
        maxX = max(maxX, loc.x);
        maxY = max(maxY, loc.y);
        minX = min(minX, loc.x);
        minY = min(minY, loc.y);
    }
    for(auto & it : this->ShapeVector)
    {
        it.second.x -= minX;
        it.second.y -= minY;
    }
    return VPoint(maxX-minX, maxY-minY);
}

void VGroupShape::resize(const VPoint &point)
{
    VPoint siz = this->size();
    double fractionX = point.x/siz.x , factionY = point.y / siz.y;
    for(auto & it : this->ShapeVector)
    {
        it.second.x *= fractionX;
        it.second.y *= fractionY;
        VPoint subSiz = it.first.size();
        subSiz.x *= fractionX;
        subSiz.y *= fractionY;
        it.first.resize(subSiz);
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
        painter.drawImage(QPointF(it.second.x, it.second.y), it.first.toImage());
    }
    return image;
}
