#ifndef VCURVE_H
#define VCURVE_H

#include "vshape.h"

class VCurve : public VShape
{
public:
    VCurve();
    VCurve(const VCurve &shape);
    ~VCurve()override;
    const VCurve& operator=(const VCurve &shape);
    static VCurve* fromJsonObject(const QJsonObject &jsonObject);
};

#endif // VCURVE_H
