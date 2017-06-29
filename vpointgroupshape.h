#ifndef VPOINTGROUP_H
#define VPOINTGROUP_H

#include "vshape.h"
#include "QVector"
#include "vpoint.h"

class VPointGroupShape:public VShape
{
protected:
    QVector<VPoint> points;//折线的各个端点
    VPoint cr1,cr2;
public:
    VPointGroupShape();
    VPointGroupShape(const VPointGroupShape &polyline);
    VPointGroupShape(const QJsonObject &jsonObject);
    VPointGroupShape(QVector<VPoint> vec);
    QJsonObject toJsonObject()const;
    virtual void movePoint(int i,const VPoint &point);//move the ith point to (x,y)
    virtual void erasePoint(int i);
    int getN() const;
    virtual void addPoint(VPoint p);
    virtual QVector<VPoint> getPointList() const;
    const VPointGroupShape& operator=(const VPointGroupShape &polyline);
    const VPointGroupShape& operator=(const QJsonObject &jsonObject);
    virtual void getCircumscribedRectangle();//获得外接矩形的左上点、右下点
    VSize getLogicalSize()const override;
//    void setSize(const VSize &size) override;
};

#endif // VPOINTGROUP_H
