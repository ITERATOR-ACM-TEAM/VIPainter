#include "vpolygon.h"
#include "vpoint.h"
#include "QDebug"

VPolygon::VPolygon():n(0){
}

QList<VPoint> VPolygon::toImage(int w,int h,double alpha){
    QList<VPoint> resList;
    if(this->n < 2){
        qDebug()<<"单点无法构成图形"<<endl;
        return resList;
    }

    VPoint a, b;
    QList<VPoint>::iterator it = this->vertex.begin();
    a = *it;
    int cnt = 100;
    for(; it != this->vertex.end(); it++){
        if(it == this->vertex.begin())continue;
        b = *it;
        double dx = (a.x-b.x)/cnt;
        double dy = (a.y-b.y)/cnt;
        for(int i = 0; i < h; i++){
            VPoint c(a.x+i*dx, a.y+i*dy);
            resList.append(c);
        }
    }
    return resList;
}

VPolygon::~VPolygon(){
}

VPolygon::VPolygon(const VPolygon &shape):n(shape.n){
    for(auto &i : shape.vertex){
        this->vertex.append(i);
    }
}


