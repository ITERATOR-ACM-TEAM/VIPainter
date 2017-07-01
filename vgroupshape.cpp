#include <QPainter>
#include <QPointF>
#include <QJsonArray>
#include <QDebug>
#include <cmath>
#include <algorithm>
#include "vgroupshape.h"
#include "vtype.h"


VGroupShape::VGroupShape():cr(0,0)
{
}

VGroupShape::~VGroupShape()
{
    for(auto &it : this->shapes)
    {
        delete it;
    }
}

VGroupShape::VGroupShape(const VGroupShape &shape):VShape(shape),cr(0,0)
{
    for(auto & it : shape.shapes)
    {
        VShape *sp=it->clone();
        sp->setParent(this);
        this->shapes.push_back(sp);
    }
    getCircumscribedRectangle();
}


VGroupShape::VGroupShape(const QJsonArray &jsonArray)
{
    VShape * tmp ;
    for(const auto &it: jsonArray)
    {
        tmp = VShape::fromJsonObject(it.toObject());
        if(tmp != nullptr)
            shapes.push_back(tmp);
    }
    getCircumscribedRectangle();
}

QJsonArray VGroupShape::toJsonArray()const
{
    QJsonArray jsonArray;
    for(const auto &it: shapes)
    {
        jsonArray.push_back(*it);
    }
    return jsonArray;
}

const VGroupShape & VGroupShape:: operator=(const VGroupShape &shape)
{
    if(this==&shape)return *this;
    VShape::operator =(shape);
    this->clear();
    for(auto & it : shape.shapes)
    {
        VShape *sp=it->clone();
        sp->setParent(this);
        this->shapes.push_back(sp);
    }
    getCircumscribedRectangle();
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
        {
            tmp->setParent(this);
            shapes.push_back(tmp);
        }
    }
    getCircumscribedRectangle();
    return *this;
}

const VGroupShape & VGroupShape:: operator=(const QJsonArray &jsonArray)
{
    setLocation(VPoint(0,0));
    setAngle(0);
    setMagnification(VMagnification(1,1));
    this->clear();
    VShape * tmp ;
    for(const auto &it: jsonArray)
    {
        tmp = VShape::fromJsonObject(it.toObject());
        if(tmp != nullptr)
        {
            tmp->setParent(this);
            shapes.push_back(tmp);
        }
    }
    getCircumscribedRectangle();
    return *this;
}

int VGroupShape::insertShape(VShape * other)
{
    return insertShape(other,shapes.size());
}

int VGroupShape::insertShape(VShape * other, int pos)
{
    if(pos<0 || pos>this->shapes.size() || other == nullptr) return -1;
    other->setParent(this);
    this->shapes.insert(this->shapes.begin()+pos, other);
    VPoint orign = other->getLocation();
    other->setLocation(VPoint(orign.x-getLocation().x, orign.y-getLocation().y));
    getCircumscribedRectangle();
    return pos;
}

int VGroupShape::insertShape(const QVector<VShape *> & other)
{
    VPoint  orign;
    for(auto &it:other)
    {
        it->setParent(this);
        this->shapes.push_back(it);
        orign = it->getLocation();
        it->setLocation(VPoint(orign.x-getLocation().x, orign.y-getLocation().y));
    }

    getCircumscribedRectangle();
    return this->getVectorSize()-1;
}

int VGroupShape::insertShape(const QVector<VShape *> & other, int pos)
{
    if(pos<0 || pos>=this->shapes.size()) return -1;
    VPoint orign;
    for(auto &it:other)
    {
        it->setParent(this);
        this->shapes.insert(this->shapes.begin()+(pos++), it);
        orign = it->getLocation();
        it->setLocation(VPoint(orign.x-getLocation().x, orign.y-getLocation().y));
    }

    getCircumscribedRectangle();
    return pos-1;
}

bool VGroupShape::moveShape(int i,  VPoint  location)
{
    if(i<0 || i>=shapes.size()) return false;

    this->shapes[i]->setLocation(location);


    getCircumscribedRectangle();
    return true;
}

QVector<VShape *> VGroupShape::getShapeVector()
{
    return shapes;
}

VShape* VGroupShape::clone()
{
    return new VGroupShape(*this);
}


void VGroupShape::draw(QPainter *painter,const VMagnification &magnification)
{
//    qDebug()<<"size"<<getSize();
//    qDebug()<<"logic"<<getSize();
//    qDebug()<<"trans"<<trans;
    double angle;
    VPoint loc;

    for(auto &it: shapes)
    {
        painter->save();
        angle = it->getAngle();
        loc = it->getLocation();
        painter->translate(loc.x*magnification.horizontal, loc.y*magnification.vertical);
        painter->rotate(angle);
        it->draw(painter,magnification*(it->getMagnification()));
        //qDebug() << *it;
        painter->restore();
    }

    //painter->drawRect(0,0,1,1);
}
//获得外接矩形的左上点、右下点
void VGroupShape::getCircumscribedRectangle(){

    if(shapes.empty())
    {
        cr.width=cr.height=0;
        return;
    }

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
        point = VPoint(loc.x + der[i][0]*siz.width/2, loc.y + der[i][1]*siz.height/2);
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
            point = VPoint(loc.x + der[i][0]*siz.width/2, loc.y + der[i][1]*siz.height/2);
            point=point.rotate( loc, a);
//            qDebug()<<"point "<<point;
            maxX = std::max(maxX, point.x);
            maxY = std::max(maxY, point.y);
            minX = std::min(minX, point.x);
            minY = std::min(minY, point.y);
        }
    }
//    qDebug()<<maxX<<minX<<maxY<<minY;
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
    VPoint location=getLocation()+VSize(midx,midy);
    location=(location*getMagnification()).rotate(location,getAngle());
    setLocation(location);
    VGroupShape *groupShape=dynamic_cast<VGroupShape*>(getParent());
    if(groupShape!=nullptr)groupShape->getCircumscribedRectangle();

    cr=VSize(maxX-minX,maxY-minY);
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
    VPoint subPoint, subLocation;
    double subAngle;
    VMagnification subMag;
    for(VShape * it:this->shapes)
    {
        subLocation = it->getLocation();
        subAngle = it->getAngle();
        subMag = it->getMagnification();
        subPoint = VPoint(point.x - subLocation.x, point.y - subLocation.y).rotate(VPoint(0,0),-subAngle)/subMag;
        if(it->contains(subPoint))
        {
            return true;
        }
    }
    return false;
}

QJsonObject VGroupShape::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
    if(getParent()==nullptr)
    {
        jsonObject.erase(jsonObject.find("location"));
        jsonObject.erase(jsonObject.find("magnification"));
        jsonObject.erase(jsonObject.find("angle"));
    }
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
    cr=VSize(1,1);
    QJsonArray jsonArray = jsonObject.value("shapes").toArray();
    VShape * tmp ;
    for(const auto &it: jsonArray)
    {
        tmp = VShape::fromJsonObject(it.toObject());
        if(tmp != nullptr)
            shapes.push_back(tmp);
    }
    getCircumscribedRectangle();
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

VSize VGroupShape::getSize()
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
