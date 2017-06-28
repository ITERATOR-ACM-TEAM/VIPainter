#include "testwidget.h"
#include "vpolygon.h"
#include "vellipse.h"
#include "vpoint.h"
#include "vsize.h"
#include <QPainter>
#include <QSize>
#include <QPen>
#include <QBrush>
#include <QtMath>

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),canvasSize(400,300)
{
    setMouseTracking(true);
    groupShape.setName("main shape");
    groupShape.setLocation(VPoint(0,0));
    groupShape.setSize(VSize(10,10));

    VEllipse *ellipse=new VEllipse("test shape",VPoint(0,0),VSize(40,50));
    groupShape.insertShape(ellipse);
    update();
}

TestWidget::~TestWidget()
{
}


void TestWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->pos();
}

void TestWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(this->width()/2,this->height()/2);
    painter.scale(scale,scale);

    QPen oldPen=painter.pen();
    QBrush oldBrush=painter.brush();

    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::lightGray,1));

    painter.drawRect(-canvasSize.x/2, -canvasSize.y/2, canvasSize.x, canvasSize.y);

    painter.setPen(oldPen);
    painter.setBrush(oldBrush);

    groupShape.draw(&painter);
}
