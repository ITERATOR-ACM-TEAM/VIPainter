#ifndef VSHAPE_H
#define VSHAPE_H

#include <QImage>
#include <QJsonObject>
#include <QString>
#include "vpoint.h"
#include "vsize.h"

class VShape{
protected:
    VPoint location;
    VSize size;
    double angle;
public:
    static VShape* fromJsonObject(const QJsonObject &jsonObject);
    virtual QJsonObject toJsonObject()=0;
    VShape* clone();
    virtual QImage toImage()=0;

    virtual VSize getSize()=0;
    virtual void setSize(const VSize &size)=0;

    void setLocation(const VPoint &location);
    VPoint getLocation();

    void setAngle(double angle);
    double getAngle();

    virtual bool contains(const VPoint &point)=0;
    virtual QString type()=0;
    virtual ~VShape();
};

#endif //#ifndef VSHAPE_H
