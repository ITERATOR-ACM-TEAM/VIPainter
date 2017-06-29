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
    bool isRoot;

public:

    static QVector<VShape *> breakUp (VGroupShape * group);

    VGroupShape(bool isRoot=false);
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
    bool moveShape(int i, const VPoint & location);
    bool contains(const VPoint &point);
    void clear();

    QVector<VShape *> getShapeVector();

    int getVectorSize()const;
    VSize getLogicalSize();

    void draw(QPainter *painter)override;
    QString type()const override;

    const VGroupShape& operator=(const VGroupShape &shape);
    const VGroupShape& operator=(const QJsonObject &jsonObject);
};

#endif // VGROUPSHAPE_H

