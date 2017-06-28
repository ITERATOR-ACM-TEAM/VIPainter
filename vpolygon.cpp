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

VPolygon::VPolygon(const QJsonObject &jsonObject):VPolyline(jsonObject){

}

const VPolygon& VPolygon::operator=(const VPolygon &vpolygon){
    VPolyline::operator=(vpolygon);
    return *this;
}

const VPolygon& VPolygon::operator=(const QJsonObject &jsonObject){
    VPolyline::operator=(jsonObject);
    return *this;
}

void VPolygon::draw(QPainter *painter){
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    painter->setBrush(defaultBrush);
    QPolygonF qpf;
    for(auto &i : vertex){
        qpf << i.toQPointF();
    }
    painter->drawPolygon(qpf);
}
//QImage VPolygon::toImage(){
//    int width = cr2.y-cr1.y, height = cr2.x-cr1.x;
//    QImage image(width, height, QImage::Format_ARGB32);
//    QPainter painter(&image);
//    QPolygonF qpf;
//    for(auto &i : vertex){
//        qpf << i.toQPointF();
//    }
//    painter.drawPolygon(qpf);

//    return image;
//}

QString VPolygon::type() const{
    return VType::Polygon;
}
