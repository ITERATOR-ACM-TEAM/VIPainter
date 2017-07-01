#include "vpolyline.h"
#include "vtype.h"
#include <QPainter>
#include <QJsonArray>
#include <QDebug>
#include <cmath>

VPolyline::VPolyline()
{
}

VPolyline::VPolyline(const VPolyline &polyline):VPointGroupShape(polyline){
}

VPolyline::VPolyline(const QJsonObject &jsonObject):VPointGroupShape(jsonObject){
}

VPolyline::VPolyline(QVector<VPoint> vec):VPointGroupShape(vec){
}

const VPolyline& VPolyline::operator=(const VPolyline &polyline){
    if(this==&polyline)return *this;
    VPointGroupShape::operator=(polyline);
    return *this;
}

const VPolyline& VPolyline::operator=(const QJsonObject &jsonObject){
    VPointGroupShape::operator=(jsonObject);
    return *this;
}

void VPolyline::draw(QPainter *painter,const VMagnification &magnification)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    painter->setBrush(defaultBrush);
    QPolygonF qpf;
    for(auto &i : this->points){
        qpf << (i*magnification).toQPointF();
    }
    painter->drawPolyline(qpf);
}
//QImage VPolyline::toImage(){
//    int width = cr2.y-cr1.y, height = cr2.x-cr1.x;
//    QImage image(width, height, QImage::Format_ARGB32);
//    QPainter painter(&image);
//    QPolygonF qpf;
//    for(auto &i : vertex){
//        qpf << i.toQPointF();
//    }
//    painter.drawPolyline(qpf);

//    return image;
//}

QString VPolyline::type() const{
    return VType::Polyline;
}

bool VPolyline::contains(VPoint point){
    double width = VShape::defaultPen.width()*20;
    double x = point.x;
    double y = point.y;
    for(int i = 1; i < points.size(); i++){
        double A = points[i].y-points[i-1].y;
        double B = points[i].x-points[i-1].x;
        if(std::abs(B*(y-points[i-1].y)-A*(x-points[i-1].x))<=width)
            return true;
    }
    return false;
//    if(x >= cr1.x && x <= cr2.x && y >= cr1.y && y <= cr2.y)
//        return true;
//    else return false;
}

VShape* VPolyline::clone()
{
    return new VPolyline(*this);
}

//VPolyline* VPolyline::fromJsonObject(const QJsonObject &jsonObject)
//{
//    VPolyline *vPolyline=new VPolyline();
//    vPolyline->n = jsonObject.value("n").toInt();
//    vPolyline->vertex.clear();
//    QJsonArray jsonVertex = jsonObject.value("vertex").toArray();
//    for(const auto &i : jsonVertex){
//        vPolyline->vertex.push_back(VPoint::fromJsonObject(i.toObject()));
//    }
//    return vPolyline;
//}

QJsonObject VPolyline::toJsonObject()const
{
    QJsonObject jsonObject(VPointGroupShape::toJsonObject());
    return jsonObject;
}
