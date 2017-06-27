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
    double angle=0;
public:
    static double PI;
    static VShape* fromJsonObject(const QJsonObject &jsonObject);
    virtual QJsonObject toJsonObject()const;
    VShape* clone();

    virtual QImage toImage()=0;
    virtual VSize getSize()const=0;
    virtual void setSize(const VSize &size)=0;
    virtual bool contains(const VPoint &point)=0;
    virtual QString type()const=0;
    virtual ~VShape();

    void setLocation(const VPoint &location);
    VPoint getLocation()const;

    void setAngle(double angle);
    double getAngle()const;

};

#endif //#ifndef VSHAPE_H
