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
    VPoint cr1, cr2;//CircumscribedRectangle,外接矩形的左上点、右下点
public:
    VPolyline();
    VPolyline(const VPolyline &polyline);
    VPolyline(const QJsonObject &jsonObject);
    VPolyline(int n, QVector<VPoint> vec);
    QJsonObject toJsonObject()const;
    virtual void movePoint(int i,const VPoint &point);//move the ith point to (x,y)
    virtual void erasePoint(int i);
    int getN() const;
    virtual void addPoint(VPoint p);
    virtual QVector<VPoint> getPointList() const;
    const VPolyline& operator=(const VPolyline &polyline);
    const VPolyline& operator=(const QJsonObject &jsonObject);
    virtual void getCircumscribedRectangle();//获得外接矩形的左上点、右下点
    bool contains(const VPoint &point) override;
    QString type()const;
    //QImage toImage() override;
    void draw(QPainter *painter)override;
    VSize getSize()const override;
    void setSize(const VSize &size) override;
};

#endif // VPOLYLINE_H
