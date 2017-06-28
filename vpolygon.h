#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"
#include "vpoint.h"
#include "vpolyline.h"

class VPolygon : public VPointGroupShape{
public:
    VPolygon();
    VPolygon(const VPolygon &shape);
    VPolygon(const QJsonObject &jsonObject);
    ~VPolygon()override;
    const VPolygon& operator=(const VPolygon &shape);
    const VPolygon& operator=(const QJsonObject &jsonObject);
    bool contains(const VPoint &point) override;
    //QImage toImage()override;
    void draw(QPainter *painter)override;
    QString type()const override;
};

#endif //#ifndef VPOLYGON
