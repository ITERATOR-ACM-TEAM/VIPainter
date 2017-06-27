#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"
#include "vpoint.h"
#include "vpolyline.h"

class VPolygon : public VPolyline{
private:
    int n;//顶点个数，即为n边形
    QList<VPoint> vertex;//n边形的各个顶点，按顺时针顺序存储
public:
    QPolygon toQPolygon();
    VPolygon();
    VPolygon(const VPolygon &shape);
    ~VPolygon()override;
    const VPolygon& operator=(const VPolygon &shape);
};

#endif //#ifndef VPOLYGON
