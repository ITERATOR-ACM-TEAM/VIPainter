#include "vpolygon.h"
#include "vpoint.h"
#include "QDebug"
#include <QPainter>
#include "vtype.h"

VPolygon::VPolygon():VPolyline(){
}

VPolygon::~VPolygon(){
}

VPolygon::VPolygon(const VPolygon &shape):VPolyline(shape){
}

QImage VPolygon::toImage(){
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

QString VPolygon::type() const{
    return VType::Polygon;
}
