#include "vshape.h"
#include "vpolygon.h"
#include "vpolyline.h"
#include "vellipse.h"
#include "vcurveline.h"
#include "vgroupshape.h"
#include "vbezlercurve.h"
#include <vtype.h>
#include <vpoint.h>
#include <cmath>
#include <QDebug>

const double VShape::PI=atan(1)*4.0;
const QPen VShape::defaultPen(QBrush(Qt::black),1);
const QBrush VShape::defaultBrush(Qt::transparent);

VShape::~VShape()
{
}

VShape::VShape(const QString &name, const VPoint &location, const VMagnification &magnification, double angle,VShape *parent):
    name(name),location(location),magnification(magnification),angle(angle),parent(parent)
{
}

VShape* VShape::clone()
{
    return fromJsonObject(toJsonObject());
}

VShape* VShape::fromJsonObject(const QJsonObject &jsonObject)
{
    QString type=jsonObject.value("type").toString();
    if(type==VType::Ellipse)return new VEllipse(jsonObject);
    else if(type==VType::Polyline)return new VPolyline(jsonObject);
    else if(type==VType::Polygon)return new VPolygon(jsonObject);
    else if(type==VType::GroupShape)return new VGroupShape(jsonObject);
    else if(type==VType::Curveline)return new VCurveline(jsonObject);
    else if(type==VType::BezlerCurve)return new VBezlerCurve(jsonObject);
    else return nullptr;
}


void VShape::zoomin(VMagnification magnification)
{
    this->magnification=this->magnification*magnification;
}

void VShape::setLocation(const VPoint &location)
{
    this->location=location;
}


VPoint VShape::getLocation()const
{
    return location;
}

void VShape::setAngle(double angle)
{
    this->angle=angle;
}

void VShape::setName(const QString &name)
{
    this->name=name;
}

QString VShape::getName()const
{
    return name;
}

VShape::operator QJsonValue()const
{
    return toJsonObject();
}

VShape::VShape(const QJsonObject jsonObject)
{
    name=jsonObject.value("name").toString();
    magnification=jsonObject.value("magnification").toObject();
    angle=jsonObject.value("angle").toDouble();
    location=jsonObject.value("location").toObject();
}


const VShape& VShape::operator=(const QJsonObject &jsonObject)
{
    name=jsonObject.value("name").toString();
    magnification=jsonObject.value("magnification").toObject();
    angle=jsonObject.value("angle").toDouble();
    location=jsonObject.value("location").toObject();
    return *this;
}

QJsonObject VShape::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("type",this->type());
    jsonObject.insert("name",this->getName());
    jsonObject.insert("magnification",this->getMagnification());
    jsonObject.insert("angle",this->getAngle());
    jsonObject.insert("location",this->getLocation());
    return jsonObject;
}

double VShape::getAngle()const
{
    return angle;
}

void VShape::setMagnification(const VMagnification &magnification)
{
    this->magnification=magnification;
}

VMagnification VShape::getMagnification()const
{
    return magnification;
}

void VShape::setParent(VShape *parent)
{
    this->parent=parent;
}

VShape * VShape::getParent()const
{
    return parent;
}

QVector<VPoint> VShape::getRect()
{
    QVector<VPoint> points;
    VSize size=getSize()/VMagnification(2);
    VPoint center(0,0);
    VMagnification mag = getMagnification();
    points.append(VPoint(size.width,size.height)*mag+VSize(crDis,crDis));
    points.append(VPoint(0, size.height)*mag+VSize(0,crDis));
    points.append(VPoint(-size.width,size.height)*mag+VSize(-crDis,crDis));
    points.append(VPoint(-size.width,0)*mag+VSize(-crDis,0));
    points.append(VPoint(-size.width,-size.height)*mag+VSize(-crDis,-crDis));
    points.append(VPoint(0,-size.height)*mag+VSize(0,-crDis));
    points.append(VPoint(size.width,-size.height)*mag+VSize(crDis,-crDis));
    points.append(VPoint(size.width,0)*mag+VSize(crDis,0));
    return points;
}

VPoint VShape::translate(const VPoint & point)
{
    VPoint subLocation(getLocation());
    double subAngle = getAngle();
    VMagnification subMag = getMagnification();
    return VPoint(point.x - subLocation.x, point.y - subLocation.y).rotate(VPoint(0,0),-subAngle)/subMag;
}

VPoint VShape::reverseTransform(const VPoint &point)
{
    VPoint subLocation(getLocation());
    double subAngle = getAngle();
    VMagnification subMag = getMagnification();
    return (point*subMag).rotate(VPoint(0, 0), subAngle)+subLocation;
}

void VShape::moveLoc(const VPoint & point)
{
    setLocation(point);
    if(parent == nullptr) return;
    VGroupShape *groupShape=dynamic_cast<VGroupShape*>(getParent());
    if(groupShape!=nullptr)groupShape->getCircumscribedRectangle();
}

void VShape::drawCR(QPainter * painter)
{

    QVector<VPoint> points = this->getRect();
    QList<QPointF> qpoints;
    QPolygonF qpf;
    for(auto &i : points){
        QPointF qpoint = i.toQPointF();
        qpoints.append(qpoint);
        qpf << qpoint;
    }

    QBrush bru;
    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);
    painter->setBrush(bru);
    painter->drawPolygon(qpf);

    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    bru.setStyle(Qt::SolidPattern);
    bru.setColor(Qt::white);
    painter->setPen(pen);
    painter->setBrush(bru);
    for(auto &i : qpoints){
        painter->drawEllipse(i, 2, 2);
    }

}

int VShape::atCrPoints(const VPoint & point)
{
    QVector<VPoint> points = this->getRect();
    VSize siz = VSize(crDis*2,crDis*2);
    VPoint pos;
    VPoint p = point * magnification;
    int cnt = 0;
    for(auto it: points)
    {
        pos = VPoint(p.x-it.x, p.y-it.y);
        qDebug() << pow(pos.x * siz.height, 2)+pow(pos.y*siz.width, 2) << pow(siz.width*siz.height, 2);
        if(pow(pos.x * siz.height, 2)+pow(pos.y*siz.width, 2) <= pow(siz.width*siz.height, 2))
            return cnt;
        cnt++;
    }
    return -1;
}

void VShape::changeMag(int i, const VVector & vec)
{
    static const VMagnification mark[4] =
    {
      {1, 1}, {-1, 1},{-1,-1},{1,-1}
    };
    if(i<0 || i>=8) return;
    QVector<VPoint> points = this->getRect();
    VPoint crp = points[i];
    VVector pc = vec;
    VVector co(VPoint(0,0), crp);
    VVector mov;
    VMagnification mag = this->getMagnification();
    if(i % 2)
    {
        double dis = (pc*co) / co.norm();
        if(i % 4 == 1)
        {
            mov = VVector(0, dis);
        }
        else
        {
            mov = VVector(dis, 0);
        }
    }
    else
    {
        mov = vec * mark[i/2];
    }
    mov = mov*2;
    //this->setLocation(this->reverseTransform((mov+VPoint(0,0))/mag));
    this->setMagnification(VMagnification(std::max(mag.horizontal+mov.x/(this->getSize().width), 1e-9), std::max(mag.vertical+mov.y/(this->getSize().height), 1e-9)));
}
