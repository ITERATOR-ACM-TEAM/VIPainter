#ifndef VPOLYLINE_H
#define VPOLYLINE_H

#include "vshape.h"
#include "QVector"
#include "vpoint.h"

class VPolyline:public VShape
{
protected:
    int n;//折线端点数目
    QVector<VPoint> vertex;//折线的各个端点
public:
    VPolyline();
    VPolyline(const VPolyline &polyline);
    static VPolyline* fromJsonObject(const QJsonObject &jsonObject);
    virtual void movePoint(int i,const VPoint &point);//move the ith point to (x,y)
    int getN() const;
    virtual void addPoint(VPoint p);
    virtual QVector<VPoint> getPointList() const;
    const VPolyline& operator=(const VPolyline &polyline);
};

#endif // VPOLYLINE_H
