#ifndef VTEXT_H
#define VTEXT_H

#include <QString>
#include <cstring>
#include <QJsonObject>
#include "vshape.h"

class VText : public VShape
{
private:
    QString text;
public:
    VText();
    VText(QString str);
    VText(const QJsonObject jsonObject);
    const VText& operator=(const VText &vText);
    const VText& operator=(const QJsonObject &jsonObject);
    ~VText();

    QString getText() const;
    void setText(QString str);
    QJsonObject toJsonObject()const;

    void draw(QPainter *painter,const VMagnification &magnification);
    bool contains(VPoint point);
    QString type()const;
    VSize getSize();
};

#endif // VTEXT_H
