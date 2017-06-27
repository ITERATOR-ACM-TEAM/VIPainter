#include "vgroupshape.h"
#include <QPainter>
#include <QPointF>
#include <math.h>
#include <algorithm>
#include "vtype.h"

VGroupShape::VGroupShape()
{
    //TODO:
}

VGroupShape::~VGroupShape()
{
    for(auto &it : this->ShapeVector)
    {
        delete it;
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
    return *this;
}

void VGroupShape::addShape(VShape * other)
{
    this->ShapeVector.push_back(other);
}

bool VGroupShape::moveShape(int i, const VPoint & location)
{
    if(i>=ShapeVector.size()) return false;
    this->ShapeVector[i]->setLocation(location);
    return true;
}

QVector<VShape *> VGroupShape::getShapeVector()
{
    return ShapeVector;
}

VSize VGroupShape::getSize()const
{
    double minX, minY;
    double maxX, maxY;
    for(auto & it : this->ShapeVector)
    {
        VPoint loc = it->getLocation();
        maxX = std::max(maxX, loc.x);
        maxY = std::max(maxY, loc.y);
        minX = std::min(minX, loc.x);
        minY = std::min(minY, loc.y);
    }
    return VSize(maxX-minX, maxY-minY);
}

void VGroupShape::setSize(const VSize &size)
{
    VSize siz = this->getSize();
    double fractionX = size.x/siz.x , fractionY = size.y / siz.y;
    for(auto & it : this->ShapeVector)
    {
        it->setLocation(VPoint(it->getLocation().x * fractionX, it->getLocation().y * fractionY));
        VSize subSiz = it->getSize();
        subSiz.x *= fractionX;
        subSiz.y *= fractionY;
        it->setSize(subSiz);
    }
}

QImage VGroupShape::toImage()
{
    VSize sz = this->getSize();
    int width = sz.y, height = sz.x;
    QImage image(width, height, QImage::Format_ARGB32);
    QPainter painter(&image);
    for(auto & it : this->ShapeVector)
    {
        QImage subImage = it->toImage();
        QPainter subPainter(&subImage);
        subPainter.rotate(it->getAngle());
        painter.drawImage(QPointF(it->getLocation().x - this->location.x, it->getLocation().y - this->location.y), subImage);
    }
    return image;
}

bool VGroupShape::eraseShape(int i)
{
    if(i>=ShapeVector.size()) return false;
    ShapeVector.erase(ShapeVector.begin()+i);
    return true;
}

QString VGroupShape::type()const
{
    return VType::GroupShap;
}

bool VGroupShape::contains(const VPoint &point)
{
    for(auto & it: ShapeVector)
    {
        if(it->contains(point)) return false;
    }
    return true;
}

QJsonObject VGroupShape::toJsonObject()const
{

}
