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
    VGroupShape();
    VGroupShape(const VGroupShape &shape);
    static VGroupShape* fromJsonObject(const QJsonObject &jsonObject);
    ~VGroupShape()override;
    const VGroupShape& operator=(const VGroupShape &shape);
    void addShape(VShape * other);
    bool eraseShape(int i, const VPoint & location);
    bool moveShape(int i, const VPoint & location);
    QVector<VShape *> getShapeVector();
    VSize getSize()override;
    QImage toImage()override;
    void setSize(const VSize & size)override;
};

#endif // VGROUPSHAPE_H
