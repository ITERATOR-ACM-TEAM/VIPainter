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

TestWidget::TestWidget(QMainWindow *parent) :
    QWidget(parent),canvasLocation(0,0),canvasSize(400,300)
{
    mainwindow=parent;
    setMouseTracking(true);
//    groupShape.setName("main shape");
//    groupShape.setLocation(VPoint(0,0));
//    groupShape.setSize(VSize(10,10));

    VEllipse *ellipse=new VEllipse("test shape",VPoint(0,0),VSize(40,50));

    VGroupShape *polygroup=new VGroupShape;
    VPolyline *polyline=new VPolyline;
    polyline->addPoint(VPoint(0,0));
    polyline->addPoint(VPoint(50,0));
    polyline->addPoint(VPoint(0,50));
    polyline->addPoint(VPoint(50,50));
    polygroup->insertShape(polyline);

    VPolygon *polygon=new VPolygon;
    polygon->addPoint(VPoint(0,0));
    polygon->addPoint(VPoint(50,0));
    polygon->addPoint(VPoint(50,50));
    polygon->addPoint(VPoint(0,50));
    polygroup->insertShape(polygon);
    polygroup->moveShape(1,VPoint(-100,-100));

    VCurveline *vcurve = new VCurveline;
    vcurve->addPoint(VPoint(-50,0.04));
    vcurve->addPoint(VPoint(0,20));
    vcurve->addPoint(VPoint(50,0.04));

    groupShape.insertShape(ellipse);
    groupShape.insertShape(polygroup);
    groupShape.insertShape(vcurve);

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
    }else{
        scale/=1.1;
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
}

void TestWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    if(event->buttons()&Qt::LeftButton)
    {
        canvasLocation.x+=qpoint.x()-pressPoint.x();
        canvasLocation.y+=qpoint.y()-pressPoint.y();
        pressPoint=qpoint;
        //qDebug()<<"canvasLocation: ("<<canvasLocation.x<<","<<canvasLocation.y<<")"<<endl;
        update();
    }
    else
    {
        VPoint point(qpoint.x()-(this->width()/2+canvasLocation.x),qpoint.y()-(this->height()/2+canvasLocation.y));
        mainwindow->statusBar()->showMessage(QString("%1,%2").arg(floor(point.x/scale)).arg(floor(point.y/scale)));
    }
}

void TestWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(this->width()/2+canvasLocation.x,this->height()/2+canvasLocation.y);
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
