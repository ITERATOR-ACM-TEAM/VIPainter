#ifndef VDOCKTITLEBAR_H
#define VDOCKTITLEBAR_H

#include <QSize>
#include <QDockWidget>
#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include "qdebug.h"
#include <QStyle>

class VDockTitleBar:public QWidget
{
    Q_OBJECT
public:
    VDockTitleBar(QWidget *parent = 0);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

public slots:
    void updateMask();

private:
    QPixmap maxPix, closePix, minPix;

};

#endif // VDOCKTITLEBAR_H
