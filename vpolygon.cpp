#include "vpolygon.h"
#include "vpoint.h"
#include "QDebug"

VPolygon::VPolygon():n(0){
}

VPolygon::~VPolygon(){
}

VPolygon::VPolygon(const VPolygon &shape):n(shape.n){
    for(auto &i : shape.vertex){
        this->vertex.append(i);
    }
}


