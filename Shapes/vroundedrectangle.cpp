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
    text = new VText(*(roundedRectangle.text));
    this->text->setSize(getSize());
}

VRoundedRectangle::VRoundedRectangle(const QJsonObject &jsonObject):VPolygon(jsonObject){
    //qDebug()<<"ok gz"<<endl;
    text = new VText(jsonObject.value("text").toObject());
    this->text->setSize(getSize());
}

VRoundedRectangle::VRoundedRectangle(QVector<VPoint> vec){
    Q_UNUSED(vec);
    text = new VText("");
    this->text->setSize(getSize());
}

const VRoundedRectangle& VRoundedRectangle::operator=(const VRoundedRectangle &roundedRectangle){
    if(this==&roundedRectangle)return *this;
    VPointGroupShape::operator=(roundedRectangle);
    delete text;
    text = new VText(*(roundedRectangle.text));
    return *this;
}

const VRoundedRectangle& VRoundedRectangle::operator=(const QJsonObject &jsonObject){
    VPointGroupShape::operator=(jsonObject);
    delete text;
    text = new VText(jsonObject.value("text").toObject());
    return *this;
}

void VRoundedRectangle::draw(QPainter *painter,const VTransform &transform)
{
    painter->setPen(QPen(pen.brush(),1,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    painter->setBrush(brush);
    painter->save();
    VPoint loc=transform.map(VPoint(0,0));
    VPoint p1 = points[1];
    VPoint p2 = points[3];
    double W = p2.x-p1.x;
    double H = p2.y-p1.y;
    double width=transform.map(VPoint(1,0))-loc;
    double height=transform.map(VPoint(0,1))-loc;
    QTransform ptrans=QTransform::fromScale(1/width,1/height)*transform*painter->worldTransform();
    painter->setTransform(ptrans);
    width*=W;
    height*=H;
    QRectF rec(-width/2, -height/2, width, height);
    painter->drawRoundRect(rec);
    painter->restore();
    text->draw(painter, transform);
}

void VRoundedRectangle::drawCR(QPainter *painter,const VTransform &transform, double scale){
    QBrush bru;
    QPen pen;
    bru.setColor(Qt::black);
    pen.setColor(Qt::black);
    pen.setWidth(0);
    bru.setStyle(Qt::SolidPattern);
    pen.setStyle(Qt::SolidLine);

    painter->setPen(pen);
    painter->setBrush(bru);


    VShape::drawCR(painter,transform,  scale);
}

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

VShape* VRoundedRectangle::clone()const
{
    return new VRoundedRectangle(*this);
}

QJsonObject VRoundedRectangle::toJsonObject()const
{
    QJsonObject jsonObject(VPointGroupShape::toJsonObject());
    jsonObject.insert("text",*text);
    return jsonObject;
}
