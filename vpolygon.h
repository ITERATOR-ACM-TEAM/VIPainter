#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"
#include "vpoint.h"
#include "vpolyline.h"
#include "vtext.h"

class VPolygon : public VPointGroupShape{
protected:
    VText *text;
public:
    VPolygon();
    VPolygon(const VPolygon &shape);
    VPolygon(const QJsonObject &jsonObject);
    ~VPolygon()override;
    const VPolygon& operator=(const VPolygon &shape);
    const VPolygon& operator=(const QJsonObject &jsonObject);
    bool contains(VPoint point) override;
    //QImage toImage()override;
    void draw(QPainter *painter,const VMagnification & magnification)override;
    VShape* clone()override;
    QString type()const override;
    VText* getText();
};

#endif //#ifndef VPOLYGON
