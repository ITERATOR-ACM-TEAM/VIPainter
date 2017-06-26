#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"
#include "vpoint.h"

class VPolygon : public VShape{
public:
    QList<VPoint> toImage(int w,int h,double alpha)override;
    VPolygon();
    VPolygon(const VPolygon &shape);
    ~VPolygon()override;
    const VPolygon& operator=(const VPolygon &shape);
};

#endif //#ifndef VPOLYGON
