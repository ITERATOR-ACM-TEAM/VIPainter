#ifndef VSIZE_H
#define VSIZE_H

#include <QJsonObject>
#include <QJsonValue>
#include <vmagnification.h>

class VSize
{
public:
    VSize();
    double width;
    double height;
    VSize(double width,double height);
    VSize(const QJsonObject &jsonObject);

    const VSize& operator=(const VSize &size);
    const VSize& operator=(const QJsonObject &jsonObject);
    VSize operator +(const VSize & Right)const;
    VSize operator -(const VSize & Right)const;
    VSize operator *(const VMagnification &magnification)const;
    VSize operator /(const VMagnification &magnification)const;
    operator QJsonValue()const;
    QJsonObject toJsonObject()const;
};

#endif // VSIZE_H
