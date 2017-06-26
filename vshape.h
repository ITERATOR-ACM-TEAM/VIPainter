#ifndef VSHAPE_H
#define VSHAPE_H

#include <vector>
#include <pair>
#include "vpoint.h"

class VShape{
public:
    virtual std::vector<VPoint> toImage(int w,int h,int alpha);
    VShape();
    VShape(const VShape &shape);
    VShape(VShape &&shape);
    virtual ~VShape();
    const VShape& operator=(const VShape &shape);
    const VShape& operator=(VShape &&shape);

};

#endif //#ifndef VSHAPE_H
