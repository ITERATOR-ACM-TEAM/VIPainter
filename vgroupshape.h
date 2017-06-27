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
    QVector<QPair<VShape*, VPoint>> ShapeVector;
public:
    VGroupShape();
    VGroupShape(const VGroupShape &shape);
    static VPolyline* fromJsonObject(const QJsonObject &jsonObject);
    ~VGroupShape()override;
    const VGroupShape& operator=(const VGroupShape &shape);
    void addShape(VShape * other, const VPoint & location);
    void moveShape(int i, const VPoint & location);
    QVector<VShape *> getShapeVector();
};

#endif // VGROUPSHAPE_H
