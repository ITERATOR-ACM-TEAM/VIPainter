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
    static QString CurveLine;
    static QString GroupShape;
    VType()=delete;
};

#endif // VTYPE_H
