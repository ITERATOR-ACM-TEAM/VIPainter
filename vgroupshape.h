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
    ~VGroupShape()override;

    static VGroupShape* fromJsonObject(const QJsonObject &jsonObject);

    void addShape(VShape * other);
    bool eraseShape(int i);
    bool moveShape(int i, const VPoint & location);

    QVector<VShape *> getShapeVector();

    void setSize(const VSize & size)override;
    VSize getSize()override;

    QImage toImage()override;

    const VGroupShape& operator=(const VGroupShape &shape);
};

#endif // VGROUPSHAPE_H

