#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"
#include "vpoint.h"
#include <QPolygon>

class VPolygon : public VShape{
public:
    QPolygon toQPolygon();
    VPolygon();
    VPolygon(const VPolygon &shape);
    ~VPolygon()override;
    const VPolygon& operator=(const VPolygon &shape);
};

#endif //#ifndef VPOLYGON
