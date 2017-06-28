#include "vpolyline.h"
#include "vtype.h"
#include <QPainter>
#include <QJsonArray>
#include <cmath>

VPolyline::VPolyline():n(0)
{
}

VPolyline::VPolyline(const VPolyline &polyline):VShape(polyline){
    this->n = polyline.getN();
    this->vertex.clear();
    for(auto &i: polyline.getPointList()){
        this->vertex.append(i);
    }
    getCircumscribedRectangle();
}

VPolyline::VPolyline(const QJsonObject &jsonObject):VShape(jsonObject){
    QJsonArray jsonArray = jsonObject.value("vertex").toArray();
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->vertex.push_back(p);
    }
    n=jsonArray.size();
    getCircumscribedRectangle();
}

VPolyline::VPolyline(int n, QVector<VPoint> vec){
    this->n = n;
    for(auto &i : vec){
        this->vertex.push_back(i);
    }
}


int VPolyline::getN()const{
    return n;
}

void VPolyline::movePoint(int i,const VPoint &point){//move the ith point to position p(x,y)
    if(i >= n-1)i = n-1;
    if(i < 0)i = 0;
    this->vertex[i].x = point.x;
    this->vertex[i].y = point.y;
}

void VPolyline::erasePoint(int i){
    if(i >=0 && i < n){
        auto it = this->vertex.begin();
        it += i;
        this->vertex.erase(it);
    }
}

QVector<VPoint> VPolyline::getPointList()const{
    return this->vertex;
}

void VPolyline::addPoint(VPoint p){
    this->vertex.push_back(p);
    n++;
}

const VPolyline& VPolyline::operator=(const VPolyline &polyline){
    VShape::operator=(polyline);
    QVector<VPoint> vec = polyline.getPointList();
    for(int i = 0; i < n; i++){
        this->vertex[i] = vec[i];
    }
    return *this;
}

const VPolyline& VPolyline::operator=(const QJsonObject &jsonObject){
    VShape::operator=(jsonObject);
    QJsonArray jsonArray = jsonObject.value("vertex").toArray();
    for(const auto &it: jsonArray)
    {
        VPoint p(it.toObject());
        this->vertex.push_back(p);
    }
    n=jsonArray.size();
    getCircumscribedRectangle();
    return *this;
}

//获得外接矩形的左上点、右下点
void VPolyline::getCircumscribedRectangle(){
    double x1 = vertex[0].x, y1 = vertex[0].y;
    double x2 = x1, y2 = y1;
    for(int i = 1; i < n; i++){
        x1 = x1 < vertex[i].x ? x1 : vertex[i].x;
        y1 = y1 < vertex[i].y ? y1 : vertex[i].y;
        x2 = x2 > vertex[i].x ? x2 : vertex[i].x;
        y2 = y2 > vertex[i].y ? y2 : vertex[i].y;
    }
    cr1.x = x1;
    cr1.y = y1;
    cr2.x = x2;
    cr2.y = y2;

    //标准化，使外接矩形的左上点移到坐标原点
    for(int i = 0; i < n; i++){
        vertex[i].x -= x1;
        vertex[i].y -= y1;
    }

    cr1.x = 0;
    cr1.y = 0;
    cr2.x -= x1;
    cr2.y -= y1;
}

//返回外接矩形右下角的位置
VSize VPolyline::getSize()const{
    VSize vs(cr2.x, cr2.y);
    return vs;
}

//把外接举行的右下角移动到point
void VPolyline::setSize(const VSize &point){
    double nx = (point.x-cr1.x)/(cr2.x-cr1.x);
    double ny = (point.y-cr1.y)/(cr2.y-cr1.y);
    for(int i = 0; i < n; i++){
        vertex[i].x *= nx;
        vertex[i].y *= ny;
    }
    cr2.x = point.x;
    cr2.y = point.y;
}

void VPolyline::draw(QPainter *painter)
{
    //Q_UNUSED(painter);
    //TODO: delete Q_UNUSED(painter); and this line
    QPolygonF qpf;
    for(auto &i : this->vertex){
        qpf << i.toQPointF();
    }
    painter->drawPolyline(qpf);
}
//QImage VPolyline::toImage(){
//    int width = cr2.y-cr1.y, height = cr2.x-cr1.x;
//    QImage image(width, height, QImage::Format_ARGB32);
//    QPainter painter(&image);
//    QPolygonF qpf;
//    for(auto &i : vertex){
//        qpf << i.toQPointF();
//    }
//    painter.drawPolyline(qpf);

//    return image;
//}

QString VPolyline::type() const{
    return VType::Polyline;
}

bool VPolyline::contains(const VPoint &point){
    double width = 5.0;
    double x = point.x-location.x;
    double y = point.y-location.y;
    for(int i = 1; i < n; i++){
        double A = vertex[i].y-vertex[i-1].y;
        double B = vertex[i].x-vertex[i-1].x;
        double C = vertex[i-1].x*(vertex[i].y-vertex[i-1].y)+vertex[i-1].y*(vertex[i].x-vertex[i-1].x);
        if(std::abs(A*x+B*y+C)/(A*A+B*B) <= width*width)
            return true;
    }
    return false;
//    if(x >= cr1.x && x <= cr2.x && y >= cr1.y && y <= cr2.y)
//        return true;
//    else return false;
}


//VPolyline* VPolyline::fromJsonObject(const QJsonObject &jsonObject)
//{
//    VPolyline *vPolyline=new VPolyline();
//    vPolyline->n = jsonObject.value("n").toInt();
//    vPolyline->vertex.clear();
//    QJsonArray jsonVertex = jsonObject.value("vertex").toArray();
//    for(const auto &i : jsonVertex){
//        vPolyline->vertex.push_back(VPoint::fromJsonObject(i.toObject()));
//    }
//    return vPolyline;
//}

QJsonObject VPolyline::toJsonObject()const
{
    QJsonObject jsonObject(VShape::toJsonObject());
    QJsonArray qja;
    for(auto & i : vertex){
        qja.push_back(i.toJsonObject());
    }
    jsonObject.insert("vertex",qja);
    return jsonObject;
}
