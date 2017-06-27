#include "vpolygon.h"
#include "vpoint.h"
#include "QDebug"

VPolygon::VPolygon():VPolyline(){
}

VPolygon::~VPolygon(){
}

VPolygon::VPolygon(const VPolygon &shape):VPolyline(){
    n = shape.getN();
    vertex.clear();
    for(auto &i : shape.vertex){
        this->vertex.append(i);
    }
    this->getCircumscribedRectangle();
}

QImage VShape::toImage(){
    int width = VPolyline::cr2.y-VPolyline::cr1.y, height = VPolyline::cr2.x-VPolyline::cr1.x;
    QImage image(width, height, QImage::Format_ARGB32);
    QPainter painter(&image);
    QPolygonF qpf;
    for(auto &i : vertex){
        qpf << i.toQPointF();
    }
    painter.drawPolygon(qpf);

    return image;
}
