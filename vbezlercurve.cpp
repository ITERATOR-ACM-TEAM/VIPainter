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

void VBezlerCurve::draw(QPainter *painter, const VMagnification &magnification)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    painter->setBrush(defaultBrush);
    QPainterPath path;
    auto i=points.begin();
    VPoint p1=*i;
    path.moveTo((p1*magnification).toQPointF());
    for(++i;i!=points.end();++i)
    {
        p1=*i;
        if((++i)==points.end())
        {
            path.lineTo((p1*magnification).toQPointF());
            break;
        }
        else path.quadTo((p1*magnification).toQPointF(),((*i)*magnification).toQPointF());
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
