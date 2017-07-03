#include "vtext.h"
#include "vtype.h"
#include <QJsonObject>

VText::VText()
{
    center.x = 0;
    center.y = 0;
}

VText::VText(QString str):text(str){
    calSize();
    center.x = size.width/2;
    center.y = size.height/2;
}

VText::VText(const QJsonObject jsonObject):VShape(jsonObject){
    this->text = jsonObject.value("text").toString();
    calSize();
    center.x = size.width/2;
    center.y = size.height/2;
}


const VText& VText::operator=(const VText &vText){
    if(this==&vText)return *this;
    VShape::operator=(vText);
    this->text = vText.getText();
    this->center = vText.getCenter();
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

VSize VText::getSize(){
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
    return jsonObject;
}

void VText::draw(QPainter *painter,const VMagnification &magnification){
    painter->setPen(defaultPen);
    painter->setBrush(defaultBrush);

    QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::WordWrap);
//    QFontMetrics fm = painter->fontMetrics();
//    this->size.width = fm.width(this->text);
//    this->size.height = fm.ascent()+fm.descent();

//    QFontMetrics fm = painter->fontMetrics();
//    QString strElidedText = fm.elidedText(this->text, Qt::ElideRight, this->size.width, Qt::TextShowMnemonic);

    painter->drawText(center.x, center.y, this->text);
}

bool VText::contains(VPoint point){
    if((point.x-center.x)<=size.width && (point.x-center.x)>=0 && (point.y-center.y)<=size.height && (point.y-center.y)>=0)
        return true;
    else
        return false;
}

QString VText::type()const{
    return VType::Text;
}

void VText::calSize(){
    size.width = text.length();
    size.height = 2;
}
