#ifndef VTRANSFORM_H
#define VTRANSFORM_H

#include <QTransform>
#include <QJsonArray>
#include "vpoint.h"
#include "vsize.h"
#include "vmagnification.h"

class VTransform : public QTransform
{
public:
    VTransform();
    VTransform(QJsonArray arr);
    VTransform(const VTransform &trans);
    VTransform(const QTransform& trans);
    QJsonArray toJsonArray()const;
    VPoint map(const VPoint &point) const;
    VTransform inverted()const;
    void scale(const VMagnification &mag);
    void translate(const VPoint &point);
    friend VSize operator *(const VSize &size,const VTransform &transform);
    friend VPoint operator *(const VPoint &size,const VTransform &transform);
};

#endif // VTRANSFORM_H
