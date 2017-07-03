#ifndef VMAGNIFICATION_H
#define VMAGNIFICATION_H

#include <QJsonObject>

class VMagnification
{
public:
    VMagnification();
    double horizontal=1;
    double vertical=1;
    VMagnification(double magnification);
    VMagnification(double horizontal,double vertical );
    VMagnification(const QJsonObject &jsonObject);

    const VMagnification& operator=(const VMagnification &magnification);
    const VMagnification& operator=(const QJsonObject &jsonObject);
    VMagnification operator *(const VMagnification &magnification)const;
    VMagnification operator /(const VMagnification &magnification)const;
    VMagnification operator +(const VMagnification &magnification)const;
    VMagnification operator -(const VMagnification &magnification)const;
    VMagnification rotate(double a);
    operator QJsonValue()const;
    QJsonObject toJsonObject()const;
};

#endif // VMAGNIFICATION_H
