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


bool VCurveline::contains(VPoint point)
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
    VSize trans(getTranslate());

    double x[20], y[20];
    for(int i = 0; i < points.size(); i++){
        x[i] = this->points[i].x;
        y[i] = this->points[i].y;
        //qDebug()<<">>> "<<x[i]<<" "<<y[i]<<endl;
    }
    int seg = (points.size()-1)/2;
    //for(int sg = 0; sg < seg; sg++){
        Newton newton(points.size()-1, x, y);
        QPolygonF qpf;
        double h = 1*(newton.R-newton.L)/getSize().x;
        int len = (int)(getSize().x);
        for(int i = 0; i <= len; i++){
            VPoint point(newton.L+i*h, newton.calNewDiffer(newton.L+i*h));
            qpf << point.translate(trans).toQPointF();
        }
        qpf << points.back().translate(trans).toQPointF();
        painter->drawPolyline(qpf);
    //}
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
