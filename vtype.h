#ifndef VTYPE_H
#define VTYPE_H

#include <QString>
#include <QJsonValue>

class VType
{
public:
    static QString Polygon;
    static QString Polyline;
    static QString Ellipse;
    static QString Curveline;
    static QString GroupShape;
    static QString BezlerCurve;
    VType()=delete;
};

#endif // VTYPE_H
