#include "vpolygon.h"

VPolygon::VPolygon():n(0){
}

VPolygon::VPolygon(const VPolygon &shape):n(shape.n){
    for(auto &i : shape.vertex){
        this->vertex.append(i);
    }
}

QList<VPoint> VPolygon::toImage(int w,int h,int alpha){

}

