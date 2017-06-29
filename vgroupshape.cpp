#include <QPainter>
#include <QPointF>
#include <QJsonArray>
#include <QDebug>
#include <cmath>
#include <algorithm>
#include "vgroupshape.h"
#include "vtype.h"


VGroupShape::VGroupShape(bool isRoot):isRoot(isRoot)
{
}

VGroupShape::~VGroupShape()
{
    for(auto &it : this->shapes)
    {
        delete it;
    }
}

VGroupShape::VGroupShape(const VGroupShape &shape):VShape(shape)
{
    this->clear();
    for(auto & it : shape.shapes)
    {
        this->shapes.push_back(it->clone());
    }

}

const VGroupShape & VGroupShape:: operator=(const VGroupShape &shape)
{
    if(this==&shape)return *this;
    VShape::operator =(shape);
    this->clear();
    for(auto & it : shape.shapes)
    {
        this->shapes.push_back(it->clone());
    }
    return *this;
}

const VGroupShape & VGroupShape:: operator=(const QJsonObject &jsonObject)
{
    VShape::operator =(jsonObject);
    this->clear();
    QJsonArray jsonArray = jsonObject.value("shapes").toArray();
    VShape * tmp ;
    for(const auto &it: jsonArray)
    {
        tmp = VShape::fromJsonObject(it.toObject());
        if(tmp != nullptr)
            shapes.push_back(tmp);
    }
    return *this;
}

int VGroupShape::insertShape(VShape * other)
{
    if(other == nullptr) return -1;
    this->shapes.push_back(other);
    VPoint orign = other->getLocation();
    other->setLocation(VPoint(orign.x-location.x, orign.y-location.y));
    return this->getVectorSize()-1;
}

int VGroupShape::insertShape(VShape * other, int pos)
{
    if(pos<0 || pos>=this->shapes.size() || other == nullptr) return -1;
    this->shapes.insert(this->shapes.begin()+pos, other);
    VPoint orign = other->getLocation();
    other->setLocation(VPoint(orign.x-location.x, orign.y-location.y));
    return pos;
}

int VGroupShape::insertShape(const QVector<VShape *> & other)
{
    VPoint  orign;
    for(auto &it:other)
    {
        this->shapes.push_back(it);
        orign = it->getLocation();
        it->setLocation(VPoint(orign.x-location.x, orign.y-location.y));
    }
    return this->getVectorSize()-1;
}

int VGroupShape::insertShape(const QVector<VShape *> & other, int pos)
{
    if(pos<0 || pos>=this->shapes.size()) return -1;
    VPoint orign;
    for(auto &it:other)
    {
        this->shapes.insert(this->shapes.begin()+(pos++), it);
        orign = it->getLocation();
        it->setLocation(VPoint(orign.x-location.x, orign.y-location.y));
    }
    return pos-1;
}

bool VGroupShape::moveShape(int i, const VPoint & location)
{
    if(i<0 || i>=shapes.size()) return false;

    this->shapes[i]->setLocation(location);

    return true;
}

QVector<VShape *> VGroupShape::getShapeVector()
{
    return shapes;
}

void VGroupShape::draw(QPainter *painter)
{
    double angle;
    VPoint loc;
    VSize siz = this->getSize();

    for(auto &it: shapes)
    {
        angle = it->getAngle();

        loc = it->getLocation();
        painter->translate(loc.x, loc.y);
        painter->rotate(angle);
        it->draw(painter);
        painter->rotate(-angle);
        painter->translate(-loc.x, -loc.y);
    }
}

bool VGroupShape::eraseShape(int i)
{
    if(i<0 || i>=shapes.size()) return false;
    VShape * tmp = shapes[i];
    shapes.erase(shapes.begin()+i);
    delete tmp;
    return true;
}

QString VGroupShape::type()const
{
    return VType::GroupShape;
}

bool VGroupShape::contains(const VPoint &point)
{
    VPoint subPoint, subLocation;
    for(auto & it: shapes)
    {
        subLocation = it->getLocation();
        subPoint = VPoint(point.x - subLocation.x, point.y - subLocation.y);
        if(it->contains(subPoint.rotate(VPoint(0,0), it->getAngle()))) return false;
    }
    return true;
}

QJsonObject VGroupShape::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
    if(isRoot)
        jsonObject.erase(jsonObject.find("location"));
    QJsonArray jsonArray;
    for(const auto &it: shapes)
    {
        jsonArray.push_back(*it);
    }
    jsonObject.insert("shapes", jsonArray);
    return jsonObject;
}

VGroupShape::VGroupShape(const QJsonObject &jsonObject):VShape(jsonObject)
{
    QJsonArray jsonArray = jsonObject.value("shapes").toArray();
    VShape * tmp ;
    for(const auto &it: jsonArray)
    {
        tmp = VShape::fromJsonObject(it.toObject());
        if(tmp != nullptr)
            shapes.push_back(tmp);
    }
}

QVector<VShape *> VGroupShape::breakUp (VGroupShape * group)
{
    QVector<VShape *> tmp;
    if(group == nullptr) return tmp;
    tmp = group->getShapeVector();
    group->shapes.clear();
    delete group;
    return tmp;
}

int VGroupShape::getVectorSize()const
{
    return this->shapes.size();
}

void VGroupShape::clear()
{
    for(auto &it : this->shapes)
    {
        delete it;
    }
    this->shapes.clear();
}

VSize VGroupShape::getLogicalSize()
{

}
