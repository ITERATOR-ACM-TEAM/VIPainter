#include "vtext.h"
#include "vtype.h"
#include <QJsonObject>
#include <cmath>

VText::VText()
{
    center.x = 0;
    center.y = 0;
    size.width = 100;
    size.height = 100;
}

VText::VText(QString str):text(str){
    center.x = 0;
    center.y = 0;
    size.width = 100;
    size.height = 100;
}

VText::VText(const QJsonObject jsonObject):VShape(jsonObject){
    this->text = jsonObject.value("text").toString();
    VPoint p(jsonObject.value("center").toObject());
    this->center = p;
    VSize s(jsonObject.value("size").toObject());
    this->size = s;
}


const VText& VText::operator=(const VText &vText){
    if(this==&vText)return *this;
    VShape::operator=(vText);
    this->text = vText.getText();
    this->center = vText.getCenter();
    this->size = vText.getSize();
    return *this;
}

const VText& VText::operator=(const QJsonObject &jsonObject){
    VShape::operator=(jsonObject);
    this->text = jsonObject.value("text").toString();
    VPoint p(jsonObject.value("center").toObject());
    this->center = p;
    VSize s(jsonObject.value("size").toObject());
    this->size = s;
    return *this;
}

VText::~VText(){

}

QString VText::getText() const{
    return this->text;
}

void VText::setText(QString str){
    this->text = str;
}

VSize VText::getSize() const{
    return this->size;
}

void VText::setSize(VSize s){
    size.width = s.width;
    size.height = s.height;
}

VPoint VText::getCenter() const{
    return this->center;
}

void VText::setCenter(VPoint point){
    center.x = point.x;
    center.y = point.y;
}

QJsonObject VText::toJsonObject()const{
    QJsonObject jsonObject(VShape::toJsonObject());
    jsonObject.insert("text", this->text);
    jsonObject.insert("center", this->center);
    jsonObject.insert("size", this->size);
    return jsonObject;
}

void VText::draw(QPainter *painter, const VTransform &transform){
    painter->setPen(defaultPen);
    painter->setBrush(defaultBrush);

    QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::WordWrap);

//    QFontMetrics fm = painter->fontMetrics();
//    QString strElidedText = fm.elidedText(this->text, Qt::ElideRight, this->size.width, Qt::TextShowMnemonic);

    VPoint ct(center.x-size.width, center.y-size.height);
    ct = ct*transform;
    painter->drawText(ct.x, ct.y, this->text);
}

bool VText::contains(VPoint point){
    if(std::abs(point.x-center.x)*2<=size.width && std::abs(point.y-center.y)*2<=size.height)
        return true;
    else
        return false;
}

QString VText::type()const{
    return VType::Text;
}

VSize VText::getSize(){
    return this->size;
}
