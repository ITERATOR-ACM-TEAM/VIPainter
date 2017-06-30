#include "vcurveline.h"
#include "interpolation.h"
#include <QJsonArray>
#include <QDebug>
#include "vtype.h"
#include <QPainterPath>
#include <cmath>

VCurveline::VCurveline(){
}

VCurveline::VCurveline(const VCurveline &vcurveline):VPointGroupShape(vcurveline){
}

VCurveline::VCurveline(const QJsonObject &jsonObject):VPointGroupShape(jsonObject){
}


bool VCurveline::contains(VPoint point)
{
    //Q_UNUSED(point);
    //return false;
    //TODO:
    double distance = 5;
    for(int i = 0; i < points.size(); i+=2){
        if(i+2 > points.size())break;
        if(points[i].x <= point.x && points[i+2].x >= point.x){
            QVector<VPoint> vec;
            for(int j = 0; j < 3; j++){
                vec.push_back(points[j]);
            }
            Lagrange lag;
            lag.Init(vec);
            if(std::abs(lag.calLag(point.x) - point.y) > distance)
                return true;
            else return false;
        }
    }
    return false;
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

VShape* VCurveline::clone()
{
    return new VCurveline(*this);
}

void VCurveline::draw(QPainter *painter,const VMagnification &magnification)
{
    painter->setPen(QPen(QBrush(Qt::black),1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter->setBrush(defaultBrush);

//    double x[20], y[20];
//    for(int i = 0; i < points.size(); i++){
//        x[i] = this->points[i].x;
//        y[i] = this->points[i].y;

//    }

    int seg = (points.size()-1)/2;
    for(int sg = 0; sg < seg; sg++){
        QPolygonF qpf;
        double sx=painter->worldTransform().m11();
        //Newton newton(2, x+sg*2, y+sg*2);
        QVector<VPoint> vec;
        for(int i = 0; i < 3; i++){
            vec.push_back(points[sg*2+i]);
            // qDebug()<<">>> "<<points[sg*2+i].x<<" "<<points[sg*2+i].y<<endl;
        }
        Lagrange lag;
        lag.Init(vec);
        //for(int i = 0; i < 3; i++)
        //    qDebug()<<">>> "<<x[sg*2+i]<<" "<<y[sg*2+i]<<endl;
        double tmp=magnification.horizontal*sx;
        double h = 1.0/tmp;
        int len = (int)((lag.R-lag.L)*tmp);
        for(int i = 0; i <= len; i++){
            VPoint point(lag.L+i*h, lag.calLag(lag.L+i*h));
            if(lag.L+i*h>lag.R)break;
            qpf << (point*magnification).toQPointF();
        }
        qpf << (vec.back()*magnification).toQPointF();
        // qDebug() <<lag.L<<" "<<lag.R<<endl;
        painter->drawPolyline(qpf);
    }
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
