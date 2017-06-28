#include "vcurveline.h"
#include "interpolation.h"
#include <QJsonArray>
#include <QDebug>
#include "vtype.h"
#include <QDebug>
#include <QPainterPath>

VCurveline::VCurveline():n(0){
}

VCurveline::VCurveline(const VCurveline &vcurveline):VShape(vcurveline){
    this->n = vcurveline.getN();
    QVector<VPoint> vec = vcurveline.getVertex();
    for(auto &i : vec){
        this->vertex.push_back(i);
    }
}

VCurveline::VCurveline(const QJsonObject &jsonObject):VShape(jsonObject){
    QJsonArray jsonArray = jsonObject.value("vertex").toArray();
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->vertex.push_back(p);
    }
    n=jsonArray.size();
}


bool VCurveline::contains(const VPoint &point)
{
    return false;
}

int VCurveline::getN()const{
    return this->n;
}

QVector<VPoint> VCurveline::getVertex()const{
    return this->vertex;
}

const VCurveline& VCurveline::operator=(const VCurveline &vcurveline){
    VShape::operator=(vcurveline);
    QVector<VPoint> vec = vcurveline.getVertex();
    for(int i = 0; i < n; i++){
        this->vertex[i] = vec[i];
    }
    return *this;
}

const VCurveline& VCurveline::operator=(const QJsonObject &jsonObject){
    VShape::operator=(jsonObject);
    QJsonArray jsonArray = jsonObject.value("vertex").toArray();
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->vertex.push_back(p);
    }
    n=jsonArray.size();
    return *this;
}

void VCurveline::addPoint(VPoint p){
    this->vertex.push_back(p);
    n++;
}

VCurveline::~VCurveline()
{
}

QString VCurveline::type() const{
    return VType::Curveline;
}

QJsonObject VCurveline::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
    QJsonArray qja;
    for(auto & i : vertex){
        qja.push_back(i.toJsonObject());
    }
    jsonObject.insert("vertex",qja);
    return jsonObject;
}

void VCurveline::draw(QPainter *painter)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter->setBrush(defaultBrush);
    double x[20], y[20];
    for(int i = 0; i < n; i++){
        x[i] = this->vertex[i].x;
        y[i] = this->vertex[i].y;
        //qDebug()<<">>> "<<x[i]<<" "<<y[i]<<endl;
    }
    n = vertex.size();
    Newton newton(n-1, x, y);
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
