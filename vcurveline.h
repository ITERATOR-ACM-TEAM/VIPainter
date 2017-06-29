#ifndef VCURVELINE_H
#define VCURVELINE_H

#include "vpointgroupshape.h"
#include "vpoint.h"
#include <QVector>

class VCurveline : public VPointGroupShape
{
public:
    VCurveline();
    VCurveline(const VCurveline &shape);
    VCurveline(const QJsonObject &jsonObject);
    ~VCurveline()override;
    const VCurveline& operator=(const VCurveline &shape);
    const VCurveline& operator=(const QJsonObject &jsonObject);

    QJsonObject toJsonObject()const override;
    void draw(QPainter *painter,const VMagnification &magnification)override;
    QString type()const override;
    bool contains(VPoint point)override;
    VShape* clone()override;
};

#endif // VCURVELINE_H
