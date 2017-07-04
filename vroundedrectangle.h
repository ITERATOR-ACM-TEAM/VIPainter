#ifndef VROUNDEDRECTANGLE_H
#define VROUNDEDRECTANGLE_H

#include "vpolygon.h"
#include "QVector"
#include "vpoint.h"
#include "vtext.h"

class VRoundedRectangle : public VPolygon
{
public:
    VRoundedRectangle();
    VRoundedRectangle(const VRoundedRectangle &roundedRectangle);
    VRoundedRectangle(const QJsonObject &jsonObject);
    VRoundedRectangle(QVector<VPoint> vec);
    QJsonObject toJsonObject()const;
    const VRoundedRectangle& operator=(const VRoundedRectangle &roundedRectangle);
    const VRoundedRectangle& operator=(const QJsonObject &jsonObject);
    bool contains(VPoint point) override;
    QString type()const;
    //QImage toImage() override;
    void draw(QPainter *painter,const VMagnification & magnification)override;
    VShape* clone()override;
};

#endif // VROUNDEDRECTANGLE_H
