#include "testwidget.h"
#include "vpolygon.h"
#include "vellipse.h"
#include "vpoint.h"
#include "vsize.h"
#include "vcurveline.h"
#include "vmagnification.h"
#include "vvector.h"
#include "vtext.h"
#include "changetextdialog.h"
#include <QPainter>
#include <QSize>
#include <QPen>
#include <QBrush>
#include <QtMath>
#include <QStatusBar>
#include <QDebug>
#include <QVector>
#include <QAction>

TestWidget::TestWidget(QMainWindow *parent, bool antialiasing) :
    QWidget(parent),canvasLocation(0,0),canvasSize(400,300),cursorType(VCursorType::CHOOSE),crPos(-1),antialiasing(antialiasing)
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


void TestWidget::setAntialiasing(bool antialiasing)
{
    this->antialiasing=antialiasing;
    //qDebug()<<"antialiasing"<<antialiasing;
    update();
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
    QPoint pressPoint=event->pos();
    VPoint point(pressPoint.x(), pressPoint.y());
    if(cursorType == VCursorType::MOVE)
        this->setCursor(Qt::ClosedHandCursor);
    else if(cursorType == VCursorType::CHOOSE)
    {
        if(focusShape != nullptr)
        {
            crPos = focusShape->atCrPoints(focusShape->translate( getLoc(point)));
            if(crPos == -1)
            {
                VPointGroupShape * pgs = dynamic_cast<VPointGroupShape *>(focusShape);
                if (pgs != nullptr)
                {
                    int tmp = pgs->atPoints(focusShape->translate( getLoc(point)));
                    if(tmp != -1) crPos = tmp+8;
                }

            }
        }

        if(crPos == -1)
           focusShape = getShape(point);

        update();
    }
    lastMove=VPoint(pressPoint.x(),pressPoint.y());
}

void TestWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QPoint pressPoint=event->pos();
    VPoint point(pressPoint.x(), pressPoint.y());
    qDebug()<<point.x<<" "<<point.y<<endl;
    VText * vt = dynamic_cast<VText *>(focusShape);
    if (vt != nullptr)
    {
        ChangeTextDialog::showDialog(vt);
    }
}

void TestWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(cursorType == VCursorType::MOVE)
        this->setCursor(Qt::OpenHandCursor);
    crPos = -1;
}

void TestWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    VPoint vpoint(qpoint.x(), qpoint.y());
    VPoint pos = groupShape.translate((getLoc(vpoint)));
    if(cursorType == VCursorType::CHOOSE)
    {
        if(crPos == -1 && groupShape.contains(pos))
        {
            this->setCursor(Qt::SizeAllCursor);
        }
        else if(crPos>=0 && crPos < 8 || focusShape!=nullptr && focusShape->atCrPoints(focusShape->translate(pos)) != -1)
        {
            static QPixmap pixmap("://icon/mover.png");
            this->setCursor(QCursor(pixmap.scaled(30, 30), 15, 15));
        }
        else
        {
            this->setCursor(Qt::ArrowCursor);
        }
    }
    if(event->buttons()&Qt::LeftButton)
    {
        if(cursorType == VCursorType::MOVE)
        {
            canvasLocation.x+=qpoint.x()-lastMove.x;
            canvasLocation.y+=qpoint.y()-lastMove.y;
            //qDebug()<<"canvasLocation: ("<<canvasLocation.x<<","<<canvasLocation.y<<")"<<endl;
            update();
        }else if(cursorType == VCursorType::CHOOSE){
            if(focusShape != nullptr)
            {
                VPoint loc = focusShape->getLocation();
                VPoint lp = groupShape.translate(getLoc(lastMove));
                VVector v(lp, pos);
                if(crPos == -1)
                {
                    focusShape->moveLoc(v+loc);
                    this->setCursor(Qt::SizeAllCursor);
                }
                else if(crPos < 8)
                {
                    //qDebug()<<"move"<<crPos;
                    focusShape->changeMag(crPos, v.rotate(-(focusShape->getAngle())));
                }
                else if(crPos >= 8)
                {
                    dynamic_cast<VPointGroupShape *>(focusShape)->changePoint(crPos - 8, focusShape->translate(lp));
                }
            }

            update();
        }
    }
    else
    {
        VPoint point(qpoint.x()-(this->width()/2+canvasLocation.x),qpoint.y()-(this->height()/2+canvasLocation.y));
        mainwindow->statusBar()->showMessage(QString("%1,%2").arg(floor(point.x/scale+0.5)).arg(floor(point.y/scale+0.5)));
    }
    lastMove = vpoint;
}

void TestWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(antialiasing)painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(this->width()/2+canvasLocation.x,this->height()/2+canvasLocation.y);

    painter.scale(scale,scale);
    painter.save();
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::lightGray,1));
    painter.drawRect(-canvasSize.width/2, -canvasSize.height/2, canvasSize.width, canvasSize.height);
    painter.restore();
    groupShape.draw(&painter,groupShape.getMagnification());

    if(focusShape != nullptr)
    {
        painter.save();
        double angle = focusShape->getAngle();
        VPoint loc = focusShape->getLocation();
        painter.translate(loc.x, loc.y);
        painter.rotate(angle);
        focusShape->drawCR(&painter);
        //qDebug() << *it;
        painter.restore();
    }

}


bool TestWidget::eventFilter(QObject * obj, QEvent * ev)
{
    Q_UNUSED(obj);
    if(ev->type()==QEvent::KeyPress)
    {
        QKeyEvent *event=static_cast<QKeyEvent*>(ev);
        switch(event->key())
        {
        case Qt::Key_Up:
            if(focusShape!=nullptr)
            {
                VPoint loc=focusShape->getLocation();
                focusShape->setLocation(VPoint(loc.x,loc.y-1));
            }
            break;
        case Qt::Key_Down:
            if(focusShape!=nullptr)
            {
                VPoint loc=focusShape->getLocation();
                focusShape->setLocation(VPoint(loc.x,loc.y+1));
            }
            break;
        case Qt::Key_Left:
            if(focusShape!=nullptr)
            {
                VPoint loc=focusShape->getLocation();
                focusShape->setLocation(VPoint(loc.x-1,loc.y));
            }
            break;
        case Qt::Key_Right:
            if(focusShape!=nullptr)
            {
                VPoint loc=focusShape->getLocation();
                focusShape->setLocation(VPoint(loc.x+1,loc.y));
            }
            break;
        }
        update();
        return true;
    }
    return false;
}

void TestWidget::changeCursor(VCursorType type)
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
    default:
    {
        this->setCursor(Qt::ArrowCursor);
    }
    }
}

VShape * TestWidget::getShape(const VPoint &point)
{
    VPoint subPoint, subLocation;
    double subAngle;
    VPoint loc = getLoc(point);
//    qDebug() << loc;
    VMagnification subMag;
    for(VShape * it:this->groupShape.getShapeVector())
    {
        subLocation = it->getLocation();
        subAngle = it->getAngle();
        subMag = it->getMagnification();
        subPoint = VPoint(loc.x - subLocation.x, loc.y - subLocation.y).rotate(VPoint(0,0),-subAngle)/subMag;
//        qDebug() << subPoint;
//        qDebug() << subMag;
        if(it->contains(subPoint))
        {
//            qDebug() << it->type();
            return it;
        }
    }
    return nullptr;
}

VPoint TestWidget::getLoc(const VPoint & point)
{
    return VPoint((point.x-(this->width()/2+canvasLocation.x))/scale,(point.y-(this->height()/2+canvasLocation.y))/scale);
}
