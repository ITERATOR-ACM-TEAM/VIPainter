#ifndef VSHAPE_H
#define VSHAPE_H

#include <QImage>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QList>
#include "vpoint.h"
#include "vsize.h"
#include "vvector.h"
#include "vmagnification.h"

class VShape{
private:
    QString name;
    VPoint location;
    VMagnification magnification;
    double angle;
    VShape *parent=nullptr;

protected:
    static const QPen defaultPen;
    static const QBrush defaultBrush;
    QPen pen = defaultPen;
    QBrush brush = defaultBrush;
    double crDis = 3;

public:
    static const double PI;
    static VShape* fromJsonObject(const QJsonObject &jsonObject);
    VShape(const QString &name="",const VPoint &location=VPoint(0,0),const VMagnification &magnification=VMagnification(1,1),double angle=0,VShape *parent=nullptr);
    VShape(const QJsonObject jsonObject);
    virtual QJsonObject toJsonObject()const;
    operator QJsonValue()const;
    const VShape& operator=(const QJsonObject &jsonObject);
    virtual VShape* clone();

    virtual void draw(QPainter *painter,const VMagnification &magnification)=0;
    virtual bool contains(VPoint point)=0;
    virtual QString type()const=0;
    virtual ~VShape();

    void moveLoc(const VPoint & point);

    virtual void drawCR(QPainter * painter,const VMagnification &magnification);
    void changeMag(int pos, const VVector &vec);
    int atCrPoints(const VPoint & point);


/*****************setter and getter*********************/
    void zoomin(VMagnification magnification);
    void setMagnification(const VMagnification &magnification);
    VMagnification getMagnification()const;

    void setLocation(const VPoint &location);
    VPoint getLocation()const;

    void setAngle(double angle);
    double getAngle()const;

    void setName(const QString &name);
    QString getName()const;

    void setParent(VShape *parent);
    VShape * getParent()const;
    QVector<VPoint> getRect();

    virtual VSize getSize()=0;
    VPoint translate(const VPoint & point);
    VPoint reverseTranslate(const VPoint &point);

};

#endif //#ifndef VSHAPE_H
