#ifndef VELLIPSE_H
#define VELLIPSE_H

#include "vshape.h"
#include "vpoint.h"

class VEllipse : public VShape{
private:
    VPoint center;
    double alpha;

public:
    VEllipse();
    VEllipse(const VEllipse &shape);
    const VEllipse& operator=(const VEllipse &shape);
    static VEllipse* fromJsonObject(const QJsonObject &jsonObject);

    QImage toImage()override;
    void resize(int weight,int height)override;
    ~VEllipse()override;
};

#endif // VELLIPSE_H
