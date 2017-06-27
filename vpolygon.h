#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"
#include "vpoint.h"
#include "vpolyline.h"

class VPolygon : public VPolyline{
public:
    VPolygon();
    VPolygon(const VPolygon &shape);
    ~VPolygon()override;
    const VPolygon& operator=(const VPolygon &shape);
    static VPolygon* fromJsonObject(const QJsonObject &jsonObject);
    QImage toImage()override;
    QString type()const override;
};

#endif //#ifndef VPOLYGON
