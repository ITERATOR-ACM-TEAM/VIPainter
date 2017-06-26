#ifndef VGROUPSHAPE_H
#define VGROUPSHAPE_H

#include "vshape.h"

class VGroupShape : public VShape
{
public:
    VGroupShape();
    VGroupShape(const VGroupShape &shape);
    ~VGroupShape()override;
    const VGroupShape& operator=(const VGroupShape &shape);
};

#endif // VGROUPSHAPE_H
