#include "testwidget.h"
#include "vpolygon.h"
#include "vellipse.h"
#include "vpoint.h"
#include "vsize.h"
#include "vcurveline.h"
#include "vmagnification.h"
#include <QPainter>
#include <QSize>
#include <QPen>
#include <QBrush>
#include <QtMath>
#include <QStatusBar>
#include <QDebug>
#include <QVector>

TestWidget::TestWidget(QMainWindow *parent) :
    QWidget(parent),canvasLocation(0,0),canvasSize(400,300),cursorType(VCursorType::CHOOSE)
{
    mainwindow=parent;
    setMouseTracking(true);
    focusShape = nullptr;
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
        if(scale>10)scale=10;
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
    VPoint point(pressPoint.x(), pressPoint.y());
    if(cursorType == VCursorType::MOVE)
        this->setCursor(Qt::ClosedHandCursor);
    else if(cursorType == VCursorType::CHOOSE)
    {
        focusShape = getShape(point);
    }
}

void TestWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(cursorType == VCursorType::MOVE)
        this->setCursor(Qt::OpenHandCursor);
}

void TestWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    if(event->buttons()&Qt::LeftButton)
    {
        if(cursorType == VCursorType::MOVE)
        {
            canvasLocation.x+=qpoint.x()-pressPoint.x();
            canvasLocation.y+=qpoint.y()-pressPoint.y();
            pressPoint=qpoint;
            //qDebug()<<"canvasLocation: ("<<canvasLocation.x<<","<<canvasLocation.y<<")"<<endl;
            update();
        }
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

    painter.scale(scale,scale);
    painter.save();
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::lightGray,1));
    painter.drawRect(-canvasSize.width/2, -canvasSize.height/2, canvasSize.width, canvasSize.height);
    painter.restore();

    groupShape.draw(&painter,groupShape.getMagnification());

}

void TestWidget::changeCursor(int type)
{
    this->cursorType = type;
    switch(type)
    {
    case VCursorType::CHOOSE:
    {
        this->setCursor(Qt::ArrowCursor);
    }break;
    case VCursorType::MOVE:
    {
        this->setCursor(Qt::OpenHandCursor);
    }break;
    }
}

VShape * TestWidget::getShape(const VPoint &point)
{
    VPoint subPoint, subLocation;
    double subAngle;
    VPoint loc = getLoc(point);
    qDebug() << loc;
    VMagnification subMag;
    for(VShape * it:this->groupShape.getShapeVector())
    {
        subLocation = it->getLocation();
        subAngle = it->getAngle();
        subMag = it->getMagnification();
        subPoint = VPoint(loc.x - subLocation.x, loc.y - subLocation.y).rotate(VPoint(0,0),-subAngle)*subMag;
        if(it->contains(subPoint))
        {
            qDebug() << it->type();
            return it;
        }
    }
    return nullptr;
}

VPoint TestWidget::getLoc(const VPoint & point)
{

    return VPoint((point.x-(this->width()/2+canvasLocation.x))/scale,(point.y-(this->height()/2+canvasLocation.y))/scale);
}
