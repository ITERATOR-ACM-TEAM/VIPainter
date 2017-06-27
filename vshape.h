#ifndef VSHAPE_H
#define VSHAPE_H

#include <QImage>
#include <QJsonObject>
#include <QString>
#include "vpoint.h"

class VShape{
public:
    static VShape* fromJsonObject(const QJsonObject &jsonObject);
    virtual QJsonObject toJsonObject()=0;
    VShape* clone();
    virtual QImage toImage()=0;
    virtual VPoint size()=0;
    virtual void resize(int weight,int height)=0;
    virtual bool contains(const VPoint &point)=0;
    virtual QString type()=0;
    virtual ~VShape();
};

#endif //#ifndef VSHAPE_H
