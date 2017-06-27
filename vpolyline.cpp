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

void VPolyline::move(int i,VPoint p){//move the ith point to position p(x,y)
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
}
