#ifndef VSIZE_H
#define VSIZE_H

#include <QJsonObject>

class VSize
{
public:
    VSize();
    double x;
    double y;
    VSize(double x,double y);

    const VSize& operator=(const VSize &size);
    VSize operator +(const VSize & Right);
    VSize operator -(const VSize & Right);
    QJsonObject toJsonObject();
    static VSize fromJsonObject(const QJsonObject &jsonObject);
};

#endif // VSIZE_H
