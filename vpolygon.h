#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"

class VPolygon : public VShape{
private:
    int n;//顶点个数，即为n边形
    QList<VPoint> vertex;//n边形的各个顶点，按顺时针顺序存储
public:
    QList<VPoint> toImage(int w,int h,int alpha);
    VPolygon();
    VPolygon(const VPolygon &shape);
    virtual ~VShape();
    const VPolygon& operator=(const VPolygon &shape);
};

#endif //#ifndef VPOLYGON
