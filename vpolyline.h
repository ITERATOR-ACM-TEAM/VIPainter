#ifndef VPOLYLINE_H
#define VPOLYLINE_H

#include "vshape.h"
#include "QVector"
#include "vpoint.h"

class VPolyline:public VShape
{
protected:
    int n;//折线端点数目
    double alpha;
    QVector<VPoint> vertex;//折线的各个端点
    VPoint cr1, cr2;//CircumscribedRectangle,外接矩形的左上点、右下点
public:
    VPolyline();
    VPolyline(const VPolyline &polyline);
    virtual void movePoint(int i,const VPoint &point);//move the ith point to (x,y)
    int getN() const;
    virtual void addPoint(VPoint p);
    virtual QVector<VPoint> getPointList() const;
    const VPolyline& operator=(const VPolyline &polyline);
    virtual void getCircumscribedRectangle();//获得外接矩形的左上点、右下点
};

#endif // VPOLYLINE_H
