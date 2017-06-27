#ifndef VSIZE_H
#define VSIZE_H

#include <QJsonObject>
#include <QJsonValue>

class VSize
{
public:
    VSize();
    double x;
    double y;
    VSize(double x,double y);
    VSize(const QJsonObject &jsonObject);

    const VSize& operator=(const VSize &size);
    const VSize& operator=(const QJsonObject &jsonObject);
    VSize operator +(const VSize & Right);
    VSize operator -(const VSize & Right);
    operator QJsonValue()const;
    QJsonObject toJsonObject()const;
};

#endif // VSIZE_H
