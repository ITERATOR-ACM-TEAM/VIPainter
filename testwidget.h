#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QPaintEvent>
#include <QMouseEvent>
#include <QWidget>
#include <QMainWindow>
#include <QRect>
#include "vgroupshape.h"
#include "vsize.h"

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = 0);
    double scale=1;
    ~TestWidget();

protected:
    void paintEvent(QPaintEvent *)override;
    void mouseMoveEvent(QMouseEvent *event)override;

private:
    VGroupShape groupShape;
    VSize canvasSize;
};

#endif // TESTWIDGET_H
