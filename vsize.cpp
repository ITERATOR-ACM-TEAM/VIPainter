#include "vsize.h"

VSize::VSize():width(0), height(0){}

VSize::VSize(double x,double y):width(x), height(y){}

const VSize& VSize::operator=(const VSize &size){
    this->width = size.width;
    this->height = size.height;
    return *this;
}

const VSize& VSize::operator=(const QJsonObject &jsonObject)
{
    this->width=jsonObject.value("width").toDouble();
    this->height=jsonObject.value("height").toDouble();
    return *this;
}


VSize VSize::operator *(const VMagnification &magnification)const
{
    return VSize(width*magnification.horizontal,height*magnification.vertical);
}

VSize VSize::operator /(const VMagnification &magnification)const
{
    return VSize(width/magnification.horizontal,height/magnification.vertical);
}

VSize VSize::operator +(const VSize & Right)const
{
    return VSize(width+Right.width, height+Right.height);
}

VSize VSize::operator -(const VSize & Right)const
{
    return VSize(width-Right.width, height-Right.height);
}

VSize::operator QJsonValue()const
{
    QJsonObject jsonObject;
    jsonObject.insert("width",width);
    jsonObject.insert("height",height);
    return jsonObject;
}

QJsonObject VSize::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("width",width);
    jsonObject.insert("height",height);
    return jsonObject;
}

VSize::VSize(const QJsonObject &jsonObject)
{
    *this=jsonObject;
}
