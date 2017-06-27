#ifndef VPOLYLINE_H
#define VPOLYLINE_H

#include "vshape.h"
#include "QVector"
#include "vpoint.h"

class VPolyline:public VShape
{
private:
    int n;//折线端点数目
    QVector<VPoint> vertex;//折线的各个端点
public:
    VPolyline();
    VPolyline(const VPolyline &polyline);
    int getN() const;
    virtual void addPoint(VPoint p);
    virtual void move(int i,VPoint p);//move the ith point to position p(x,y)
    virtual QVector<VPoint> getPointList() const;
    const VPolyline& operator=(const VPolyline &polyline);
};

#endif // VPOLYLINE_H
