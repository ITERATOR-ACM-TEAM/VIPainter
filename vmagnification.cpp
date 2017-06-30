#include "vmagnification.h"

VMagnification::VMagnification():horizontal(1), vertical (1){}

VMagnification::VMagnification(double magnification):horizontal(magnification), vertical(magnification){}

VMagnification::VMagnification(double x,double y):horizontal(x), vertical (y){}

const VMagnification& VMagnification::operator=(const VMagnification &magnification){
    this->horizontal = magnification.horizontal;
    this->vertical  = magnification.vertical ;
    return *this;
}

const VMagnification& VMagnification::operator=(const QJsonObject &jsonObject)
{
    if(jsonObject.isEmpty())
    {
        this->horizontal=this->vertical=1;
    }
    else
    {
        this->horizontal=jsonObject.value("horizontal").toDouble();
        this->vertical =jsonObject.value("vertical").toDouble();
    }
    return *this;
}
VMagnification::operator QJsonValue()const
{
    QJsonObject jsonObject;
    jsonObject.insert("horizontal",horizontal);
    jsonObject.insert("vertical",vertical );
    return jsonObject;
}


VMagnification VMagnification::operator *(const VMagnification &magnification) const
{
    return VMagnification(horizontal*magnification.horizontal,vertical*magnification.vertical);
}

VMagnification VMagnification::operator /(const VMagnification &magnification)const
{
    return VMagnification(horizontal/magnification.horizontal,vertical/magnification.vertical);
}

VMagnification VMagnification::operator +(const VMagnification &magnification)const
{
    return VMagnification(horizontal+magnification.horizontal,vertical+magnification.vertical);
}

VMagnification VMagnification::operator -(const VMagnification &magnification)const
{
    return VMagnification(horizontal-magnification.horizontal,vertical-magnification.vertical);
}

QJsonObject VMagnification::toJsonObject()const
{
    QJsonObject jsonObject;
    jsonObject.insert("horizontal",horizontal);
    jsonObject.insert("vertical",vertical );
    return jsonObject;
}

VMagnification::VMagnification(const QJsonObject &jsonObject)
{
    *this=jsonObject;
}
