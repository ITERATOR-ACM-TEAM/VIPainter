#include "vpointgroupshape.h"
#include "vgroupshape.h"
#include "vtype.h"
#include <QPainter>
#include <QJsonArray>
#include <cmath>

VPointGroupShape::VPointGroupShape():cr2(0,0)
{
}

VPointGroupShape::VPointGroupShape(const VPointGroupShape &shape):VShape(shape),cr2(0,0){
    points=shape.getPointList();
    getCircumscribedRectangle();
}

VPointGroupShape::VPointGroupShape(const QJsonObject &jsonObject):VShape(jsonObject),cr2(0,0){
    QJsonArray jsonArray = jsonObject.value("points").toArray();
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->points.push_back(p);
    }
    getCircumscribedRectangle();
}

VPointGroupShape::VPointGroupShape(QVector<VPoint> vec):cr2(0,0){
    for(auto &i : vec){
        this->points.push_back(i);
    }
    getCircumscribedRectangle();
}


void VPointGroupShape::movePoint(int i,VPoint point){//move the ith point to position p(x,y)
    if(i >= points.size()-1)i = points.size()-1;
    if(i < 0)i = 0;
    this->points[i].x = point.x;
    this->points[i].y = point.y;
    getCircumscribedRectangle();
}

void VPointGroupShape::erasePoint(int i){
    if(i >=0 && i < points.size()){
        auto it = this->points.begin();
        it += i;
        this->points.erase(it);
    }
    getCircumscribedRectangle();
}

QVector<VPoint> VPointGroupShape::getPointList()const{
    return this->points;
}

void VPointGroupShape::addPoint(const VPoint &p){
    this->points.push_back(p);
    getCircumscribedRectangle();
}

const VPointGroupShape& VPointGroupShape::operator=(const VPointGroupShape &shape){
    if(this==&shape)return *this;
    VShape::operator=(shape);
    points=shape.getPointList();
    cr1=shape.cr1;
    cr2=shape.cr2;
    return *this;
}

const VPointGroupShape& VPointGroupShape::operator=(const QJsonObject &jsonObject){
    VShape::operator=(jsonObject);
    QJsonArray jsonArray = jsonObject.value("points").toArray();
    points.clear();
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->points.push_back(p);
    }
    getCircumscribedRectangle();
    return *this;
}

//获得外接矩形的左上点、右下点
void VPointGroupShape::getCircumscribedRectangle(){

    if(points.empty())
    {
        cr2.x=cr2.y=1;
        return;
    }

    double x1 = points[0].x, y1 = points[0].y;
    double x2 = x1, y2 = y1;
    for(int i = 1; i < points.size(); i++){
        x1 = x1 < points[i].x ? x1 : points[i].x;
        y1 = y1 < points[i].y ? y1 : points[i].y;
        x2 = x2 > points[i].x ? x2 : points[i].x;
        y2 = y2 > points[i].y ? y2 : points[i].y;
    }
    cr1.x = x1;
    cr1.y = y1;
    cr2.x = x2;
    cr2.y = y2;

    double midx=(x1+x2)/2;
    double midy=(y1+y2)/2;
    //标准化，使外接矩形的左上点移到坐标原点
    for(int i = 0; i < points.size(); i++){
        points[i].x -= midx;
        points[i].y -= midy;
    }

    VPoint location=getLocation()+VSize(midx,midy);
    location=(location*getMagnification()).rotate(location,getAngle());
    setLocation(location);
    VGroupShape *groupShape=dynamic_cast<VGroupShape*>(getParent());
    if(groupShape!=nullptr)groupShape->getCircumscribedRectangle();

    cr1.x = 0;
    cr1.y = 0;
    cr2.x -= x1;
    cr2.y -= y1;
}

VSize VPointGroupShape::getSize()
{
//    if(abs(cr2.x)<=0.0000001)setSize(VSize(0,getSize().y));
//    if(abs(cr2.y)<=0.0000001)setSize(VSize(getSize().x,0));
    return VSize(cr2.x,cr2.y);
}

QJsonObject VPointGroupShape::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
    QJsonArray qja;
    for(auto & i : points){
        qja.push_back(i.toJsonObject());
    }
    jsonObject.insert("points",qja);
    return jsonObject;
}
