#ifndef VCURVELINE_H
#define VCURVELINE_H

#include "vshape.h"

class VCurveline : public VShape
{
public:
    VCurveline();
    VCurveline(const VCurveline &shape);
    VCurveline(const QJsonObject &jsonObject);
    ~VCurveline()override;
    const VCurveline& operator=(const VCurveline &shape);
    const VCurveline& operator=(const QJsonObject &jsonObject);
};

#endif // VCURVELINE_H
