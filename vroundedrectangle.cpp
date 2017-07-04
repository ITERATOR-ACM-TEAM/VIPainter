#include "vroundedrectangle.h"
#include "vtype.h"
#include "vvector.h"
#include <QPainter>
#include <QJsonArray>
#include <QDebug>
#include <cmath>

VRoundedRectangle::VRoundedRectangle()
{
    text = new VText("");
    this->text->setSize(getSize());
}

VRoundedRectangle::VRoundedRectangle(const VRoundedRectangle &roundedRectangle):VPolygon(roundedRectangle){
    text = new VText("");
    this->text->setSize(getSize());
}

VRoundedRectangle::VRoundedRectangle(const QJsonObject &jsonObject):VPolygon(jsonObject){
    //qDebug()<<"ok gz"<<endl;
    text = new VText("");
    this->text->setSize(getSize());
}

VRoundedRectangle::VRoundedRectangle(QVector<VPoint> vec){
    text = new VText("");
    this->text->setSize(getSize());
}

const VRoundedRectangle& VRoundedRectangle::operator=(const VRoundedRectangle &roundedRectangle){
    if(this==&roundedRectangle)return *this;
    VPointGroupShape::operator=(roundedRectangle);
    return *this;
}

const VRoundedRectangle& VRoundedRectangle::operator=(const QJsonObject &jsonObject){
    VPointGroupShape::operator=(jsonObject);
    return *this;
}

void VRoundedRectangle::draw(QPainter *painter,const VMagnification &magnification)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    painter->setBrush(defaultBrush);
    VPoint p1 = points[1]*magnification;
    VPoint p2 = points[3]*magnification;
    double W = p2.x-p1.x;
    double H = p2.y-p1.y;
    QRectF rec(p1.x, p1.y, W, H);
//    painter->drawRect(p1.x, p1.y, p2.x, p2.y);
//    qDebug()<<"ok"<<endl;
//    QPolygonF qpf;
//    for(auto &i : this->points){
//        qpf << (i*magnification).toQPointF();
//    }
//    painter->drawPolyline(qpf);
    painter->drawRoundRect(rec, 25, 25);
    text->draw(painter, magnification);
//    painter->drawRoundRect(20,20,210,160,50,50);
}
//QImage VRoundedRectangle::toImage(){
//    int width = cr2.y-cr1.y, height = cr2.x-cr1.x;
//    QImage image(width, height, QImage::Format_ARGB32);
//    QPainter painter(&image);
//    QPolygonF qpf;
//    for(auto &i : vertex){
//        qpf << i.toQPointF();
//    }
//    painter.drawroundedRectangle(qpf);

//    return image;
//}

QString VRoundedRectangle::type() const{
    return VType::RoundedRectangle;
}

bool VRoundedRectangle::contains(VPoint point){
    qreal x=point.x,y=point.y;
        int left=0;
        int right=0;
        int j=points.size()-1;
        for(int i=0;i<points.size();i++){
            if((points[i].y<y&&points[j].y>=y)||(points[j].y<y&&points[i].y>=y)){
                if((y-points[i].y)*(points[i].x-points[j].x)/(points[i].y-points[j].y)+points[i].x<=x){
                    left++;
                }
                else right++;
            }
            j=i;
        }
        return left&right;
}

VShape* VRoundedRectangle::clone()
{
    return new VRoundedRectangle(*this);
}

//VRoundedRectangle* VRoundedRectangle::fromJsonObject(const QJsonObject &jsonObject)
//{
//    VRoundedRectangle *VRoundedRectangle=new VRoundedRectangle();
//    VRoundedRectangle->n = jsonObject.value("n").toInt();
//    VRoundedRectangle->vertex.clear();
//    QJsonArray jsonVertex = jsonObject.value("vertex").toArray();
//    for(const auto &i : jsonVertex){
//        VRoundedRectangle->vertex.push_back(VPoint::fromJsonObject(i.toObject()));
//    }
//    return VRoundedRectangle;
//}

QJsonObject VRoundedRectangle::toJsonObject()const
{
    QJsonObject jsonObject(VPointGroupShape::toJsonObject());
    return jsonObject;
}
