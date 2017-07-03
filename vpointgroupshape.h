#ifndef VPOINTGROUP_H
#define VPOINTGROUP_H

#include "vshape.h"
#include "QVector"
#include "vpoint.h"
#include <QJsonArray>

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

    QJsonObject toJsonObject()const override;

    virtual void movePoint(int i, VPoint point);//move the ith point to (x,y)
    virtual void erasePoint(int i);
    virtual void addPoint(const VPoint &p);
    virtual QVector<VPoint> getPointList() const;

    const VPointGroupShape& operator=(const VPointGroupShape &polyline);
    const VPointGroupShape& operator=(const QJsonObject &jsonObject);

    virtual void getCircumscribedRectangle();//获得外接矩形的右下点

    VSize getSize()override;
//    void setSize(const VSize &size) override;

    int atPoints(const VPoint & point)const;
    bool changePoint(int i, const VPoint & point);
    void drawCR(QPainter *painter);

};

#endif // VPOINTGROUP_H
