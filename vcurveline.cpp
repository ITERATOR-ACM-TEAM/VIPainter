#include "vcurveline.h"
#include "interpolation.h"
#include <QJsonArray>
#include <QDebug>
#include "vtype.h"
#include <QDebug>
#include <QPainterPath>

VCurveline::VCurveline(){
}

VCurveline::VCurveline(const VCurveline &vcurveline):VPointGroupShape(vcurveline){
}

VCurveline::VCurveline(const QJsonObject &jsonObject):VPointGroupShape(jsonObject){
}


bool VCurveline::contains(const VPoint &point)
{
    Q_UNUSED(point);
    return false;
    //TODO:
}

const VCurveline& VCurveline::operator=(const VCurveline &vcurveline){
    if(this==&vcurveline)return *this;
    VPointGroupShape::operator=(vcurveline);
    return *this;
}

const VCurveline& VCurveline::operator=(const QJsonObject &jsonObject){
    VPointGroupShape::operator=(jsonObject);
    return *this;
}

VCurveline::~VCurveline()
{
}

QString VCurveline::type() const{
    return VType::Curveline;
}

QJsonObject VCurveline::toJsonObject()const
{
    QJsonObject jsonObject(VPointGroupShape::toJsonObject());
    return jsonObject;
}

void VCurveline::draw(QPainter *painter)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter->setBrush(defaultBrush);
    double x[20], y[20];
    for(int i = 0; i < points.size(); i++){
        x[i] = this->points[i].x;
        y[i] = this->points[i].y;
        //qDebug()<<">>> "<<x[i]<<" "<<y[i]<<endl;
    }
    Newton newton(points.size()-1, x, y);
    QVector<VPoint> vec = newton.getFunc();
    QPolygonF qpf;
    for(auto &i : vec){
        qpf << i.toQPointF();
        qDebug()<<i.x<<" "<<i.y<<endl;
    }
    painter->drawPolyline(qpf);
//    QPainterPath path;
//    auto nextit=vec.begin();
//    auto nowit=nextit++;
//    while(nextit!=vec.end())
//    {
//        path.quadTo((nextit->x+nowit->x)/2,(nextit->y+nowit->x)/2,nextit->x,nextit->y);
//        nowit=nextit++;
//    }
//    painter->drawPath(path);
}
