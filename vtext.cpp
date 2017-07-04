#include "vtext.h"
#include "vtype.h"
#include <QJsonObject>
#include <QDebug>

VText::VText()
{
    text = "";
    center.x = 0;
    center.y = 0;
}

VText::VText(QString str):text(str){
    calSize();
    center.x = 0;
    center.y = 0;
//    qDebug()<<"gouzao() "<<text<<";"<<endl;
}

VText::VText(const QJsonObject jsonObject):VShape(jsonObject){
    this->text = jsonObject.value("text").toString();
    calSize();
    center.x = 0;
    center.y = 0;
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

    QRectF rec(-size.width*magnification.horizontal/2, -size.height*magnification.vertical/2, size.width*magnification.horizontal, size.height*magnification.vertical);
//    painter->drawRect(rec);
    painter->drawText(rec, Qt::AlignCenter, this->text);
//    qDebug()<<"draw()"<<text<<endl;
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

void VText::calSize(){
    int cnt = 1, wid = 0, tmp = 0;
    for(int i = 0; i < text.length(); i++){
        tmp++;
        if(text[i] == '\n'){
            cnt++;
            wid = wid > tmp ? wid : tmp;
            tmp = 0;
        }
    }
    wid = wid > tmp ? wid : tmp;
    size.width = wid*8;
    size.height = cnt*16;
}
