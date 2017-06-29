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
private:
    QString name;
    VPoint location;
    VSize size;
    double angle;
protected:
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
    VSize getSize()const;
    void setSize(const VSize &size);

    void setLocation(const VPoint &location);
    VPoint getLocation()const;

    void setAngle(double angle);
    double getAngle()const;

    void setName(QString name);
    QString getName()const;

};

#endif //#ifndef VSHAPE_H
