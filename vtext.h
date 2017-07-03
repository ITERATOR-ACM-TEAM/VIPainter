#ifndef VTEXT_H
#define VTEXT_H

#include <QString>
#include <cstring>
#include <QJsonObject>
#include "vshape.h"
#include "vsize.h"

class VText : public VShape
{
private:
    QString text;
    VSize size;
    VPoint center;
public:
    VText();
    VText(QString str);
    VText(const QJsonObject jsonObject);
    const VText& operator=(const VText &vText);
    const VText& operator=(const QJsonObject &jsonObject);
    ~VText();

    QString getText() const;
    void setText(QString str);

    VSize getSize();
    void setSize(VSize s);

    VPoint getCenter() const;
    void setCenter(VPoint point);

    QJsonObject toJsonObject()const;

    void draw(QPainter *painter,const VMagnification &magnification);
    bool contains(VPoint point);
    QString type()const;
    void calSize();
};

#endif // VTEXT_H
