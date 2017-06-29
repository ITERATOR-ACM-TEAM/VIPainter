#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWidget>
#include <QMainWindow>
#include <QRect>
#include <QWheelEvent>
#include "vgroupshape.h"
#include "vsize.h"
#include "vpoint.h"
#include "vcursortype.h"

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QMainWindow *mainwindow);
    double scale=1;
    QMainWindow *mainwindow;
    VGroupShape groupShape;
    VPoint canvasLocation;
    VSize canvasSize;
    QPoint pressPoint;
    bool pressing=false;
    ~TestWidget();

protected:
    void paintEvent(QPaintEvent *)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void wheelEvent(QWheelEvent * event)override;

private:
    int cursorType;

public slots:
    void changeCursor(int type);
};

#endif // TESTWIDGET_H
