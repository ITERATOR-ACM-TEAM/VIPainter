/**
 * Copyright (C) 2017 kkkeQAQ
 *               2017 Bcai0797
 *               2017 Penn000
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
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
#include <QByteArray>
#include "vpoint.h"
#include "vsize.h"
#include "vvector.h"
#include "vmagnification.h"
#include "vtransform.h"

class VShape{
private:
    QString name;
    VTransform transform;
    VShape *parent=nullptr;

protected:
    static const QPen defaultPen;
    static const QBrush defaultBrush;
    QPen pen = defaultPen;
    QBrush brush = defaultBrush;
    double crDis = 3;

public:
    static const double PI;
    VShape(VShape *parent=nullptr);
    static VShape* fromJsonObject(const QJsonObject &jsonObject);
    VShape(const QJsonObject jsonObject);
    virtual QJsonObject toJsonObject()const;
    operator QJsonValue()const;
    const VShape& operator=(const QJsonObject &jsonObject);
    virtual VShape* clone()const;

    virtual void draw(QPainter *painter,const VTransform &transform)=0;
    virtual bool contains(VPoint point)=0;
    virtual QString type()const=0;
    virtual ~VShape();

    void moveLoc(const VPoint & point);

    virtual void drawCR(QPainter * painter, const VTransform &trans,double scale);
    void changeMag(int pos, const VVector & vec);
    int atCrPoints(const VPoint & point, double scale=1.0);


/*****************setter and getter*********************/
    void zoomin(const VMagnification &mag);

    void setTransform(const VTransform& transform);
    VTransform& getTransform();

    VPoint getLocation()const;

    void setName(const QString &name);
    const QString& getName()const;

    void setParent(VShape *parent);
    VShape * getParent()const;
    QVector<VPoint> getRect();
    QVector<VPoint> getSizeRect() ;

    virtual VSize getSize()=0;
    VPoint transformPoint(const VPoint & point) const;
    VPoint reverseTransformPoint(const VPoint &point) const;

    virtual void setPen(QPen pen);
    virtual void setBrush(QBrush brush);
    QPen getPen();
    QBrush getBrush();

};

#endif //#ifndef VSHAPE_H
