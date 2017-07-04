#include "vbezlercurve.h"
#include "vpoint.h"
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include "vtype.h"

VBezlerCurve::VBezlerCurve():VPointGroupShape(){
}

VBezlerCurve::~VBezlerCurve(){
}

VBezlerCurve::VBezlerCurve(const VBezlerCurve &shape):VPointGroupShape(shape){
}

VBezlerCurve::VBezlerCurve(const QJsonObject &jsonObject):VPointGroupShape(jsonObject){
}

bool VBezlerCurve::contains(VPoint point)
{
    QPainterPath path;
    auto i=points.begin();
    VPoint p1=*i;
    path.moveTo(p1.toQPointF());
    for(++i;i!=points.end();++i)
    {
        p1=*i;
        if((++i)==points.end())
        {
            path.lineTo((p1).toQPointF());
            break;
        }
        else path.quadTo(p1.toQPointF(),i->toQPointF());
    }
    return path.contains(point.toQPointF());
}

const VBezlerCurve& VBezlerCurve::operator=(const VBezlerCurve &vbezlerCurve){
    if(this==&vbezlerCurve)return *this;
    VPointGroupShape::operator=(vbezlerCurve);
    return *this;
}

const VBezlerCurve& VBezlerCurve::operator=(const QJsonObject &jsonObject){
    VPointGroupShape::operator=(jsonObject);
    return *this;
}

VShape* VBezlerCurve::clone()
{
    return new VBezlerCurve(*this);
}

void VBezlerCurve::draw(QPainter *painter,const VTransform &transform)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter->setBrush(defaultBrush);
    QPainterPath path;
    auto i=points.begin();
    VPoint p1=*i;
    path.moveTo((p1*transform).toQPointF());
    for(++i;i!=points.end();++i)
    {
        p1=*i;
        if((++i)==points.end())
        {
            path.lineTo((p1*transform).toQPointF());
            break;
        }
        else path.quadTo((p1*transform).toQPointF(),((*i)*transform).toQPointF());
    }
    painter->drawPath(path);
}

//QImage VBezlerShape::toImage(){
//    int width = cr2.y-cr1.y, height = cr2.x-cr1.x;
//    QImage image(width, height, QImage::Format_ARGB32);
//    QPainter painter(&image);
//    QBezlerShapeF qpf;
//    for(auto &i : vertex){
//        qpf << i.toQPointF();
//    }
//    painter.drawBezlerShape(qpf);

//    return image;
//}

QString VBezlerCurve::type() const{
    return VType::BezlerCurve;
}

void VBezlerCurve::drawCR(QPainter * painter, const VTransform &transform, double scale)
{
    VPointGroupShape::drawCR(painter,transform,scale);
    QBrush bru;
    QPen pen;
    pen.setColor(Qt::gray);
    pen.setWidth(1);
    pen.setStyle(Qt::DashDotDotLine);
    painter->setPen(pen);
    for(int i=1; i<points.size()-1; i+=2)
    {
        painter->drawLine((points[i-1]*transform).toQPointF(), (points[i]*transform).toQPointF());
        if(i+1<points.size())
            painter->drawLine((points[i+1]*transform).toQPointF(), (points[i]*transform).toQPointF());
    }
}
