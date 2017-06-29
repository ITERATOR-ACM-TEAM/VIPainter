#include "testwidget.h"
#include "vpolygon.h"
#include "vellipse.h"
#include "vpoint.h"
#include "vsize.h"
#include "vcurveline.h"
#include <QPainter>
#include <QSize>
#include <QPen>
#include <QBrush>
#include <QtMath>
#include <QStatusBar>
#include <QDebug>
#include <QVector>

TestWidget::TestWidget(MainWindow *parent) :
    QWidget(parent),canvasLocation(0,0),canvasSize(400,300)
{
    mainwindow=parent;
    setMouseTracking(true);
//    groupShape.setName("main shape");
//    groupShape.setLocation(VPoint(0,0));
//    groupShape.setSize(VSize(10,10));

    update();
}

TestWidget::~TestWidget()
{
}

void TestWidget::wheelEvent(QWheelEvent * event)
{
    QPoint qpoint=event->pos();
    VPoint point(qpoint.x()-(this->width()/2+canvasLocation.x),qpoint.y()-(this->height()/2+canvasLocation.y));
    VPoint oldp(point.x/scale,point.y/scale);
    update();
    if(event->delta() > 0){
        scale*=1.1;
        if(scale>20)scale=20;
    }else{
        scale/=1.1;
        if(scale<0.05)scale=0.05;
    }
    VPoint newp(point.x/scale,point.y/scale);
    canvasLocation.x+=(newp.x-oldp.x)*scale;
    canvasLocation.y+=(newp.y-oldp.y)*scale;
    update();
}

void TestWidget::mousePressEvent(QMouseEvent *event)
{
    pressPoint=event->pos();
}

void TestWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void TestWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    if(event->buttons()&Qt::LeftButton)
    {
        if(this->mainwindow->getCursonState() == MainWindow::STATE_MOVE)
        {
            canvasLocation.x+=qpoint.x()-pressPoint.x();
            canvasLocation.y+=qpoint.y()-pressPoint.y();
            pressPoint=qpoint;
            //qDebug()<<"canvasLocation: ("<<canvasLocation.x<<","<<canvasLocation.y<<")"<<endl;
        }

        update();
    }
    else
    {
        VPoint point(qpoint.x()-(this->width()/2+canvasLocation.x),qpoint.y()-(this->height()/2+canvasLocation.y));
        mainwindow->statusBar()->showMessage(QString("%1,%2").arg(floor(point.x/scale+0.5)).arg(floor(point.y/scale+0.5)));
    }
}

void TestWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(this->width()/2+canvasLocation.x,this->height()/2+canvasLocation.y);

    painter.save();
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::lightGray,1));
    painter.scale(scale,scale);
    painter.drawRect(-canvasSize.x/2, -canvasSize.y/2, canvasSize.x, canvasSize.y);
    painter.restore();

    VSize size=groupShape.getSize();
    groupShape.setSize(VSize(size.x*scale,size.y*scale));
    groupShape.draw(&painter);

    groupShape.setSize(size);
}
