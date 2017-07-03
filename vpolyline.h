#ifndef VPOLYLINE_H
#define VPOLYLINE_H

#include "vpointgroupshape.h"
#include "QVector"
#include "vpoint.h"

class VPolyline:public VPointGroupShape
{
public:
    VPolyline();
    VPolyline(const VPolyline &polyline);
    VPolyline(const QJsonObject &jsonObject);
    VPolyline(QVector<VPoint> vec);
    QJsonObject toJsonObject()const;
    const VPolyline& operator=(const VPolyline &polyline);
    const VPolyline& operator=(const QJsonObject &jsonObject);
    bool contains(VPoint point) override;
    QString type()const;
    //QImage toImage() override;
    void draw(QPainter *painter,const VTransform &transform)override;
    VShape* clone()override;
};

#endif // VPOLYLINE_H
