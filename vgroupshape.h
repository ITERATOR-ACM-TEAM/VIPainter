#ifndef VGROUPSHAPE_H
#define VGROUPSHAPE_H

#include "vshape.h"
#include "vpoint.h"
#include <QList>
#include <QVector>
#include <QPair>

class VGroupShape : public VShape
{
private:
    QVector<VShape*> shapes;
    void caculateSubShapeAndSize();
    VSize cr;

public:

    static QVector<VShape *> breakUp (VGroupShape * group);

    VGroupShape();
    VGroupShape(const VGroupShape &shape);
    VGroupShape(const QJsonObject &jsonObject);
    ~VGroupShape()override;
    QJsonObject toJsonObject()const;

    // insert shape into group,  return its index
    int insertShape(VShape * other);
    int insertShape(VShape * other, int pos);
    // insert shapeVector into group,  return the last shape's index
    int insertShape(const QVector<VShape *> & other);
    int insertShape(const QVector<VShape *> & other, int pos);

    bool eraseShape(int i);
    bool moveShape(int i, VPoint location);
    bool contains(VPoint point);
    void clear();

    QVector<VShape *> getShapeVector();

    int getVectorSize()const;
    VSize getSize()override;

    void draw(QPainter *painter,const VMagnification &magnification)override;
    QString type()const override;

    const VGroupShape& operator=(const VGroupShape &shape);
    const VGroupShape& operator=(const QJsonObject &jsonObject);
    virtual void getCircumscribedRectangle();//获得外接矩形的左上点、右下点
    VShape* clone()override;
};

#endif // VGROUPSHAPE_H

