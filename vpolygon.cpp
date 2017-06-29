#include "vpolygon.h"
#include "vpoint.h"
#include "QDebug"
#include <QPainter>
#include "vtype.h"

VPolygon::VPolygon():VPointGroupShape(){
}

VPolygon::~VPolygon(){
}

VPolygon::VPolygon(const VPolygon &shape):VPointGroupShape(shape){
}

VPolygon::VPolygon(const QJsonObject &jsonObject):VPointGroupShape(jsonObject){
}

bool VPolygon::contains(VPoint point)
{
    Q_UNUSED(point);
    return false;
    //TODO:
}

const VPolygon& VPolygon::operator=(const VPolygon &vpolygon){
    if(this==&vpolygon)return *this;
    VPointGroupShape::operator=(vpolygon);
    return *this;
}

const VPolygon& VPolygon::operator=(const QJsonObject &jsonObject){
    VPointGroupShape::operator=(jsonObject);
    return *this;
}

void VPolygon::draw(QPainter *painter)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    painter->setBrush(defaultBrush);
    VSize trans(getTranslate());
    QPolygonF qpf;
    for(auto &i : points){
        qpf << i.translate(trans).toQPointF();
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
