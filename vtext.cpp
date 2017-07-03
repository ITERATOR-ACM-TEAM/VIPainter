#include "vtext.h"
#include "vtype.h"
#include <QJsonObject>

VText::VText()
{
}

VText::VText(QString str):text(str){

}

VText::VText(const QJsonObject jsonObject):VShape(jsonObject){
    this->text = jsonObject.value("text").toString();
}


const VText& VText::operator=(const VText &vText){
    if(this==&vText)return *this;
    VShape::operator=(vText);
    this->text = vText.getText();
    return *this;
}

const VText& VText::operator=(const QJsonObject &jsonObject){
    VShape::operator=(jsonObject);
    this->text = jsonObject.value("text").toString();
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

QJsonObject VText::toJsonObject()const{
    QJsonObject jsonObject(VShape::toJsonObject());
    jsonObject.insert("text", this->text);
    return jsonObject;
}

void VText::draw(QPainter *painter,const VMagnification &magnification){
    painter->setPen(defaultPen);
    painter->setBrush(defaultBrush);
    QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::WordWrap);
    VPoint point = VShape::getLocation();
    painter->drawText(point.x, point.y, this->text);
}

bool VText::contains(VPoint point){
    return false;
}

QString VText::type()const{
    return VType::Text;
}

VSize VText::getSize(){
    return VSize(1,1);
}
