#include "vpolygon.h"

VPolygon::VPolygon():n(0){
}

QList<VPoint> VPolygon::toImage(int w,int h,double alpha){
}

VPolygon::~VPolygon(){
}

VPolygon::VPolygon(const VPolygon &shape):n(shape.n){
    for(auto &i : shape.vertex){
        this->vertex.append(i);
    }
}


