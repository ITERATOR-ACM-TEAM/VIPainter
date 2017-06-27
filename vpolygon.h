#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"
#include "vpoint.h"
#include "vpolyline.h"

class VPolygon : public VPolyline{
public:
    VPolygon();
    VPolygon(const VPolygon &shape);
    VPolygon(const QJsonObject &jsonObject);
    ~VPolygon()override;
    const VPolygon& operator=(const VPolygon &shape);
    const VPolygon& operator=(const QJsonObject &jsonObject);
    QImage toImage()override;
    QString type()const override;
};

#endif //#ifndef VPOLYGON
