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
    QVector<VShape*> ShapeVector;

public:
    static void rotate(VPoint & point, const VPoint & center, double a);

    VGroupShape();
    VGroupShape(const VGroupShape &shape);
    VGroupShape(const QJsonObject &jsonObject);
    ~VGroupShape()override;
    QJsonObject toJsonObject()const;

    void addShape(VShape * other);
    bool eraseShape(int i);
    bool moveShape(int i, const VPoint & location);
    bool contains(const VPoint &point);

    QVector<VShape *> getShapeVector();

    void setSize(const VSize & size)override;
    VSize getSize()const override;

    QImage toImage()override;
    QString type()const override;

    const VGroupShape& operator=(const VGroupShape &shape);
    const VGroupShape& operator=(const QJsonObject &jsonObject);
};

#endif // VGROUPSHAPE_H

