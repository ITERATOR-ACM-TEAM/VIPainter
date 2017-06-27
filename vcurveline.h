#ifndef VCURVELINE_H
#define VCURVELINE_H

#include "vshape.h"

class VCurveline : public VShape
{
public:
    VCurveline();
    VCurveline(const VCurveline &shape);
    ~VCurveline()override;
    const VCurveline& operator=(const VCurveline &shape);
    static VCurveline* fromJsonObject(const QJsonObject &jsonObject);
};

#endif // VCURVELINE_H
