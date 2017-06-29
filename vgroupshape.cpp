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
    setSize(getSize());
    return *this;
}

int VGroupShape::insertShape(VShape * other)
{
    return insertShape(other,shapes.size());
}

int VGroupShape::insertShape(VShape * other, int pos)
{
    VSize olds=getLogicalSize();
    VSize trans=getTranslate();
    if(pos<0 || pos>this->shapes.size() || other == nullptr) return -1;
    this->shapes.insert(this->shapes.begin()+pos, other);
    VPoint orign = other->getLocation();
    other->setLocation(VPoint(orign.x-getLocation().x, orign.y-getLocation().y).translate(trans));

    VSize news=getLogicalSize();
    VShape::setSize(VSize(getSize().x*news.x/olds.x,getSize().y*news.y/olds.y));
    return pos;
}

int VGroupShape::insertShape(const QVector<VShape *> & other)
{
    VSize olds=getLogicalSize();
    VSize trans=getTranslate();
    VPoint  orign;
    for(auto &it:other)
    {
        this->shapes.push_back(it);
        orign = it->getLocation();
        it->setLocation(VPoint(orign.x-getLocation().x, orign.y-getLocation().y).translate(trans));
    }
    VSize news=getLogicalSize();
    VShape::setSize(VSize(getSize().x*news.x/olds.x,getSize().y*news.y/olds.y));
    return this->getVectorSize()-1;
}

int VGroupShape::insertShape(const QVector<VShape *> & other, int pos)
{
    VSize olds=getLogicalSize();
    VSize trans=getTranslate();
    if(pos<0 || pos>=this->shapes.size()) return -1;
    VPoint orign;
    for(auto &it:other)
    {
        this->shapes.insert(this->shapes.begin()+(pos++), it);
        orign = it->getLocation();
        it->setLocation(VPoint(orign.x-getLocation().x, orign.y-getLocation().y).translate(trans));
    }
    VSize news=getLogicalSize();
    VShape::setSize(VSize(getSize().x*news.x/olds.x,getSize().y*news.y/olds.y));
    return pos-1;
}

bool VGroupShape::moveShape(int i,  VPoint  location)
{
    VSize olds=getLogicalSize();
    location=location.retranslate(getTranslate());
    if(i<0 || i>=shapes.size()) return false;

    this->shapes[i]->setLocation(location);

    VSize news=getLogicalSize();
    VShape::setSize(VSize(getSize().x*news.x/olds.x,getSize().y*news.y/olds.y));
    return true;
}

QVector<VShape *> VGroupShape::getShapeVector()
{
    return shapes;
}

void VGroupShape::draw(QPainter *painter)
{
    VSize trans=getTranslate();
    double angle;
    VPoint loc;

    for(auto &it: shapes)
    {
        angle = it->getAngle();

        loc = it->getLocation().translate(trans);
        painter->translate(loc.x, loc.y);
        painter->rotate(angle);
        it->draw(painter);
        painter->rotate(-angle);
        painter->translate(-loc.x, -loc.y);
    }
    painter->drawRect(0,0,1,1);
}

bool VGroupShape::eraseShape(int i)
{
    VSize olds=getLogicalSize();
    if(i<0 || i>=shapes.size()) return false;
    VShape * tmp = shapes[i];
    shapes.erase(shapes.begin()+i);
    delete tmp;
    VSize news=getLogicalSize();
    VShape::setSize(VSize(getSize().x*news.x/olds.x,getSize().y*news.y/olds.y));
    return true;
}

QString VGroupShape::type()const
{
    return VType::GroupShape;
}

bool VGroupShape::contains(VPoint point)
{
    point=point.retranslate(getTranslate());
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
    setSize(getSize());
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


void VGroupShape::setSize(const VSize &size)
{
    VSize gsize=getSize();
    VSize der(size.x-gsize.x,size.y-gsize.y);
    for(const auto &i:shapes)
    {
        VSize isize=i->getSize();
        i->setSize(VSize(isize.x+der.x*isize.x/gsize.x,isize.y+der.y*isize.y/gsize.y));
    }
    VShape::setSize(size);
}

VSize VGroupShape::getLogicalSize()
{
    if(shapes.empty())return VSize(1,1);
    double minX, minY;
    double maxX, maxY;
    int der[4][2] = {1,1,-1,1,-1,-1,1,-1};

    // if no subShape, return (0,0)

    //init max&min
    VPoint point;
    VShape * first = shapes[0];
    VPoint loc = first->getLocation();
    VSize siz = first->getSize();
    double a = first->getAngle();

    for(int i=0; i<4; i++)\
    {
        point = VPoint(loc.x + der[i][0]*siz.x, loc.y + der[i][1]*siz.y);
        point=point.rotate(loc, a);
        maxX = point.x;
        maxY = point.y;
        minX = point.x;
        minY = point.y;
    }

    // loop
    for(auto & it : this->shapes)
    {
        loc = it->getLocation();
        siz = it->getSize();
        a = it->getAngle();
        for(int i=0; i<4; i++)\
        {
            point = VPoint(loc.x + der[i][0]*siz.x, loc.y + der[i][1]*siz.y);
            point=point.rotate( loc, a);
            maxX = std::max(maxX, point.x);
            maxY = std::max(maxY, point.y);
            minX = std::min(minX, point.x);
            minY = std::min(minY, point.y);
        }
    }
    VPoint mid((minX+maxX)/2,(minY+maxY)/2);
    for(const auto&i:shapes)
    {
        i->setLocation(mid);
    }
    return VSize(maxX-minX, maxY-minY);
}
