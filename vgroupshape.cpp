#include <QPainter>
#include <QPointF>
#include <QJsonArray>
#include <QDebug>
#include <cmath>
#include <algorithm>
#include "vgroupshape.h"
#include "vtype.h"


VGroupShape::VGroupShape(bool isRoot):isRoot(isRoot),cr(1,1)
{
}

VGroupShape::~VGroupShape()
{
    for(auto &it : this->shapes)
    {
        delete it;
    }
}

VGroupShape::VGroupShape(const VGroupShape &shape):VShape(shape),cr(1,1)
{
    for(auto & it : shape.shapes)
    {
        this->shapes.push_back(it->clone());
    }
    VSize size=getSize();
    getCircumscribedRectangle();
    VShape::setSize(size);
}

const VGroupShape & VGroupShape:: operator=(const VGroupShape &shape)
{
    if(this==&shape)return *this;
    VShape::operator =(shape);
    this->clear();
    cr=VSize(0,0);
    for(auto & it : shape.shapes)
    {
        this->shapes.push_back(it->clone());
    }
    VSize size=getSize();
    getCircumscribedRectangle();
    VShape::setSize(size);
    return *this;
}

const VGroupShape & VGroupShape:: operator=(const QJsonObject &jsonObject)
{
    VShape::operator =(jsonObject);
    this->clear();
    cr=VSize(0,0);
    QJsonArray jsonArray = jsonObject.value("shapes").toArray();
    VShape * tmp ;
    for(const auto &it: jsonArray)
    {
        tmp = VShape::fromJsonObject(it.toObject());
        if(tmp != nullptr)
            shapes.push_back(tmp);
    }
    VSize size=getSize();
    getCircumscribedRectangle();
    VShape::setSize(size);
    return *this;
}

int VGroupShape::insertShape(VShape * other)
{
    return insertShape(other,shapes.size());
}

int VGroupShape::insertShape(VShape * other, int pos)
{
    VSize trans=getTranslate();
    if(pos<0 || pos>this->shapes.size() || other == nullptr) return -1;
    this->shapes.insert(this->shapes.begin()+pos, other);
    VPoint orign = other->getLocation();
    other->setLocation(VPoint(orign.x-getLocation().x, orign.y-getLocation().y).translate(trans));


    getCircumscribedRectangle();
    return pos;
}

int VGroupShape::insertShape(const QVector<VShape *> & other)
{
    VSize trans=getTranslate();
    VPoint  orign;
    for(auto &it:other)
    {
        this->shapes.push_back(it);
        orign = it->getLocation();
        it->setLocation(VPoint(orign.x-getLocation().x, orign.y-getLocation().y).translate(trans));
    }

    getCircumscribedRectangle();
    return this->getVectorSize()-1;
}

int VGroupShape::insertShape(const QVector<VShape *> & other, int pos)
{
    VSize trans=getTranslate();
    if(pos<0 || pos>=this->shapes.size()) return -1;
    VPoint orign;
    for(auto &it:other)
    {
        this->shapes.insert(this->shapes.begin()+(pos++), it);
        orign = it->getLocation();
        it->setLocation(VPoint(orign.x-getLocation().x, orign.y-getLocation().y).translate(trans));
    }

    getCircumscribedRectangle();
    return pos-1;
}

bool VGroupShape::moveShape(int i,  VPoint  location)
{
    location=location.retranslate(getTranslate());
    if(i<0 || i>=shapes.size()) return false;

    this->shapes[i]->setLocation(location);


    getCircumscribedRectangle();
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
    //painter->drawRect(0,0,1,1);
}
//获得外接矩形的左上点、右下点
void VGroupShape::getCircumscribedRectangle(){

    VSize cr1,cr2;
    if(shapes.empty())
    {
        cr.x=cr.y=1;
        VShape::setSize(VSize(1,1));
        return;
    }
    if(shapes.size()==1)
    {
        cr.x=cr.y=1;
        VShape::setSize(VSize(1,1));
        shapes[0]->setLocation(VPoint(0,0));
        return;
    }
    VSize olds=getLogicalSize();

//    double x1 = shapes[0]->getLocation().x, y1 = shapes[0]->getLocation().y;
//    double x2 = x1, y2 = y1;
//    for(int i = 1; i < shapes.size(); i++){
//        x1 = x1 < shapes[i]->getLocation().x ? x1 : shapes[i]->getLocation().x;
//        y1 = y1 < shapes[i]->getLocation().y ? y1 : shapes[i]->getLocation().y;
//        x2 = x2 > shapes[i]->getLocation().x ? x2 : shapes[i]->getLocation().x;
//        y2 = y2 > shapes[i]->getLocation().y ? y2 : shapes[i]->getLocation().y;
//    }
//    cr1.x = x1;
//    cr1.y = y1;
//    cr2.x = x2;
//    cr2.y = y2;

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

    for(int i=0; i<4; i++)
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
        for(int i=0; i<4; i++)
        {
            point = VPoint(loc.x + der[i][0]*siz.x, loc.y + der[i][1]*siz.y);
            point=point.rotate( loc, a);
            maxX = std::max(maxX, point.x);
            maxY = std::max(maxY, point.y);
            minX = std::min(minX, point.x);
            minY = std::min(minY, point.y);
        }
    }
//    VPoint mid((minX+maxX)/2,(minY+maxY)/2);
//    for(const auto&i:shapes)
//    {
//        VPoint loc=i->getLocation();
//        i->setLocation(VPoint(loc.x-mid.x,loc.y-mid.y));
//    }

    double midx=(minX+maxX)/2;
    double midy=(minY+maxY)/2;
    //标准化，使外接矩形的左上点移到坐标原点
    for(int i = 0; i < shapes.size(); i++){
        shapes[i]->setLocation(VPoint(shapes[i]->getLocation().x-midx,shapes[i]->getLocation().y-midy));
    }
    qDebug()<<midx<<""<<midy<<endl;

    VShape::setSize(VSize(getSize().x*maxX/olds.x,getSize().y*maxY/olds.y));
    cr=VSize(maxX,maxY);
}

bool VGroupShape::eraseShape(int i)
{
    if(i<0 || i>=shapes.size()) return false;
    VShape * tmp = shapes[i];
    shapes.erase(shapes.begin()+i);
    delete tmp;
    getCircumscribedRectangle();
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
    VSize size=getSize();
    getCircumscribedRectangle();
    VShape::setSize(size);
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
    return cr;
//    if(shapes.empty())return VSize(1,1);
//    double minX, minY;
//    double maxX, maxY;
//    int der[4][2] = {1,1,-1,1,-1,-1,1,-1};

//    // if no subShape, return (0,0)

//    //init max&min
//    VPoint point;
//    VShape * first = shapes[0];
//    VPoint loc = first->getLocation();
//    VSize siz = first->getSize();
//    double a = first->getAngle();

//    for(int i=0; i<4; i++)
//    {
//        point = VPoint(loc.x + der[i][0]*siz.x, loc.y + der[i][1]*siz.y);
//        point=point.rotate(loc, a);
//        maxX = point.x;
//        maxY = point.y;
//        minX = point.x;
//        minY = point.y;
//    }

//    // loop
//    for(auto & it : this->shapes)
//    {
//        loc = it->getLocation();
//        siz = it->getSize();
//        a = it->getAngle();
//        for(int i=0; i<4; i++)
//        {
//            point = VPoint(loc.x + der[i][0]*siz.x, loc.y + der[i][1]*siz.y);
//            point=point.rotate( loc, a);
//            maxX = std::max(maxX, point.x);
//            maxY = std::max(maxY, point.y);
//            minX = std::min(minX, point.x);
//            minY = std::min(minY, point.y);
//        }
//    }
//    VPoint mid((minX+maxX)/2,(minY+maxY)/2);
//    for(const auto&i:shapes)
//    {
//        VPoint loc=i->getLocation();
//        i->setLocation(VPoint(loc.x-mid.x,loc.y-mid.y));
//    }
//    return VSize(maxX-minX, maxY-minY);
}
