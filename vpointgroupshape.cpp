#include "vpointgroupshape.h"
#include "vtype.h"
#include <QPainter>
#include <QJsonArray>
#include <cmath>

VPointGroupShape::VPointGroupShape():cr2(1,1)
{
}

VPointGroupShape::VPointGroupShape(const VPointGroupShape &shape):VShape(shape),cr2(1,1){
    points=shape.getPointList();
    VSize size=getSize();
    getCircumscribedRectangle();
    VShape::setSize(size);
}

VPointGroupShape::VPointGroupShape(const QJsonObject &jsonObject):VShape(jsonObject),cr2(1,1){
    QJsonArray jsonArray = jsonObject.value("points").toArray();
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->points.push_back(p);
    }
    VSize size=getSize();
    getCircumscribedRectangle();
    VShape::setSize(size);
}

VPointGroupShape::VPointGroupShape(QVector<VPoint> vec):cr2(1,1){
    for(auto &i : vec){
        this->points.push_back(i);
    }
    getCircumscribedRectangle();
}


int VPointGroupShape::getN()const{
    return points.size();
}

void VPointGroupShape::movePoint(int i,VPoint point){//move the ith point to position p(x,y)
    point=point.retranslate(getTranslate());
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

void VPointGroupShape::addPoint(VPoint p){
    p=p.retranslate(getTranslate());
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
    points.clear();cr2=VPoint(0,0);
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->points.push_back(p);
    }
    VSize size=getSize();
    getCircumscribedRectangle();
    VShape::setSize(size);
    return *this;
}

//获得外接矩形的左上点、右下点
void VPointGroupShape::getCircumscribedRectangle(){

    if(points.empty())
    {
        cr2.x=cr2.y=1;
        setSize(VSize(1,1));
        return;
    }
    if(points.size()==1)
    {
        cr2.x=cr2.y=1;
        setSize(VSize(1,1));
        points[0].x=points[0].y=0;
        return;
    }
    VSize olds=getLogicalSize();

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
    cr2.x = x2+1;
    cr2.y = y2+1;

    double midx=(x1+x2)/2;
    double midy=(y1+y2)/2;
    //标准化，使外接矩形的左上点移到坐标原点
    for(int i = 0; i < points.size(); i++){
        points[i].x -= midx;
        points[i].y -= midy;
    }

    cr1.x = 0;
    cr1.y = 0;
    cr2.x -= x1;
    cr2.y -= y1;

    VShape::setSize(VSize(getSize().x*cr2.x/olds.x,getSize().y*cr2.y/olds.y));
}

VSize VPointGroupShape::getLogicalSize()
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
