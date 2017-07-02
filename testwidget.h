#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWidget>
#include <QMainWindow>
#include <QRect>
#include <QDir>
#include <QWheelEvent>
#include "vgroupshape.h"
#include "vsize.h"
#include "vpoint.h"
#include "vcursortype.h"
#include "vshape.h"
#include "vpoint.h"

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QMainWindow *mainwindow,bool antialiasing);
    double scale=1;
    QMainWindow *mainwindow;
    VGroupShape groupShape;
    VShape * focusShape;
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
    VCursorType cursorType;
    VShape * getShape(const VPoint &point);
    VPoint getLoc(const VPoint & point);
    VPoint lastMove;
    int crPos;
    bool antialiasing;

public slots:
    void changeCursor(VCursorType type);
    void setAntialiasing(bool antialiasing);
};

#endif // TESTWIDGET_H
