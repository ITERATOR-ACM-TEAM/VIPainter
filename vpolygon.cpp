#include "vpolygon.h"
#include "vpoint.h"
#include "QDebug"

VPolygon::VPolygon(){
}

VPolygon::~VPolygon(){
}

VPolygon::VPolygon(const VPolygon &shape){
    n = shape.getN();
    vertex.clear();
    for(auto &i : shape.vertex){
        this->vertex.append(i);
    }
}

QImage toImage(){
    int width = cr2.y-cr1.y, height = cr2.x-cr1.x;
    QImage image(width, height, QImage::Format_ARGB32);
    QPainter painter(&image);
    QPolygonF qpf;
    for(auto &i : vertex){
        qpf << i.toQPointF();
    }
    painter.drawPolygon(qpf);

    return image;
}
