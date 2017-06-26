#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"

class VPolygon : public VShape{
public:
    std::vector<VPoint> toImage(int w,int h,int alpha);
    VPolygon();
    VPolygon(const VPolygon &shape);
    virtual ~VShape();
    const VPolygon& operator=(const VPolygon &shape);
};

#endif //#ifndef VPOLYGON
