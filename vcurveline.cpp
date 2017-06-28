#include "vcurveline.h"
#include "interpolation.h"
#include <QJsonArray>
#include <QDebug>
#include "vtype.h"

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
    this->n = jsonObject.value("n").toInt();
    QJsonArray jsonArray = jsonObject.value("vertex").toArray();
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->vertex.push_back(p);
    }
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
    this->n = jsonObject.value("n").toInt();
    QJsonArray jsonArray = jsonObject.value("vertex").toArray();
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->vertex.push_back(p);
    }
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
    QJsonObject jsonObject(VShape::toJsonObject());
    jsonObject.insert("n", n);
    QJsonArray qja;
    for(auto & i : vertex){
        qja.push_back(i.toJsonObject());
    }
    jsonObject.insert("vertex",qja);
    return jsonObject;
}

void VCurveline::draw(QPainter *painter)
{
    Newton newton;
    for(auto &i : this->vertex){
        newton.addPoint(i.x, i.y);
    }
    QVector<VPoint> vec = newton.getFunc();
    QPolygonF qpf;
    for(auto &i : vec){
        qpf << i.toQPointF();
        qDebug()<<i.x<<" "<<i.y<<endl;
    }
//    QPen pen;
//    pen.setWidth(1);
//    painter->setPen(pen);
    painter->drawPoints(qpf);
}
