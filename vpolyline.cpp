#include "vpolyline.h"

VPolyline::VPolyline():n(0)
{
}

VPolyline::VPolyline(const VPolyline &polyline){
    this->n = polyline.getN();
    vertex.clear();
    for(auto &i: polyline.getPointList()){
        this->vertex.append(i);
    }
}

int VPolyline::getN()const{
    return n;
}

void VPolyline::movePoint(int i,VPoint p){//move the ith point to position p(x,y)
    if(i >= n-1)i = n-1;
    if(i < 0)i = 0;
    this->vertex[i].x = p.x;
    this->vertex[i].y = p.y;
}

QList<VPoint> VPolyline::getPointList()const{
    return this->vertex;
}

void VPolyline::addPoint(VPoint p){
    this->vertex.push_back(p);
}

const VPolyline& VPolyline::operator=(const VPolyline &polyline){
    QVector<VPoint> vec = polyline.getPointList();
    for(int i = 0; i < n; i++){
        this->vertex[i] = vec[i];
    }
}

void VPolyline::getCircumscribedRectangle(){//获得外接矩形的左上点、右下点
    double x1 = vertex[0].x, y1 = vertex[0].y;
    double x2 = x1, y2 = y1;
    for(int i = 1; i < n; i++){
        x1 = x1 < vertex[i].x ? x1 : vertex[i].x;
        y1 = y1 < vertex[i].y ? y1 : vertex[i].y;
        x2 = x2 > vertex[i].x ? x2 : vertex[i].x;
        y2 = y2 > vertex[i].y ? y2 : vertex[i].y;
    }
    cr1.x = x1;
    cr1.y = y1;
    cr2.x = x2;
    cr2.y = y2;
    /*
    for(int i = 0; i < n; i++){//标准化，使外接矩形的左上点移到坐标原点
        vertex[i].x -= x1;
        vertex[i].y -= y1;
    }
    */
}

VPoint size(){//返回外接矩形右下角的位置
    return cr2;
}
void resize(const VPoint &point){//把外接举行的右下角移动到point
    double nx = (point.x-cr1.x)/(cr2.x-cr1.x);
    double ny = (point.y-cr1.y)/(cr2.y-cr1.y);
    for(int i = 0; i < n; i++){
        vertex[i].x *= nx;
        vertex[i].y *= ny;
    }
    cr2 = point;
}
void rotate(const VPoint &center,double alpha){
    for(int i = 0; i < n; i++){
        vertex[i].x=(vertex[i].x- center.x)*cos(alpha) - (vertex[i].y- center.y)*sin(alpha) + center.x ;
        vertex[i].y=(vertex[i].x- center.x)*sin(alpha) + (vertex[i].y- center.y)*cos(alpha) + center.y ;
    }
}

QImage toImage(){

}
