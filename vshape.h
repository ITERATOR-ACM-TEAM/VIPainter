#ifndef VSHAPE_H
#define VSHAPE_H

#include <QImage>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "vpoint.h"
#include "vsize.h"

class VShape{
protected:
    QString name;
    VPoint location;
    VSize size;
    double angle;
    static const QPen defaultPen;
    static const QBrush defaultBrush;
public:
    static const double PI;
    static VShape* fromJsonObject(const QJsonObject &jsonObject);
    VShape(const QString &name="",const VPoint &location=VPoint(0,0),const VSize &size=VSize(0,0),double angle=0);
    VShape(const QJsonObject jsonObject);
    virtual QJsonObject toJsonObject()const;
    operator QJsonValue()const;
    const VShape& operator=(const QJsonObject &jsonObject);
    virtual VShape* clone();

    virtual void draw(QPainter *painter)=0;
    virtual bool contains(const VPoint &point)=0;
    virtual QString type()const=0;
    virtual ~VShape();

/*****************setter and getter*********************/
    virtual VSize getSize()const;
    virtual void setSize(const VSize &size);

    virtual void setLocation(const VPoint &location);
    virtual VPoint getLocation()const;

    virtual void setAngle(double angle);
    virtual double getAngle()const;

    void setName(QString name);
    QString getName()const;

};

#endif //#ifndef VSHAPE_H
