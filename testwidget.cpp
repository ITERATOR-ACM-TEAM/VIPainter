/**
 * Copyright (C) 2017 kkkeQAQ
 *               2017 Bcai0797
 *               2017 Penn000
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "testwidget.h"
#include "vpolygon.h"
#include "vellipse.h"
#include "vpoint.h"
#include "vsize.h"
#include "vcurveline.h"
#include "vmagnification.h"
#include "vvector.h"
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
            qDebug() << focusShape->transformPoint(getLoc(point));
            crPos = focusShape->atCrPoints(focusShape->transformPoint( getLoc(point)),scale);
            if(crPos == -1)
            {
                VPointGroupShape * pgs = dynamic_cast<VPointGroupShape *>(focusShape);
                if (pgs != nullptr)
                {
                    int tmp = pgs->atPoints(focusShape->transformPoint( getLoc(point)));
                    if(tmp != -1) crPos = tmp+8;
                }

            }
        }

        if(crPos == -1)
           focusShape = getShape(point);

        update();
    }
    else if(cursorType == VCursorType::ROTATE)
    {
        if(focusShape != nullptr)
            lastAngle = 0;
    }
    lastPress=VPoint(pressPoint.x(),pressPoint.y());
}

void TestWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(cursorType == VCursorType::MOVE)
        this->setCursor(Qt::OpenHandCursor);
//    else if(cursorType == VCursorType::ROTATE)
//    {
//        if(focusShape != nullptr)
//        {
//            VVector vlp(focusShape->getLocation(), getLoc(lastPress)),
//                    vnow(focusShape->getLocation(), getLoc(VPoint(event->pos().x(), event->pos().y())));
//            qDebug() << VVector::rotationAngle(vlp, vnow);
//            focusShape->setAngle(-VVector::rotationAngle(vlp, vnow));
//            update();
//        }
//    }
    crPos = -1;
}

void TestWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    VPoint vpoint(qpoint.x(), qpoint.y());
    VPoint pos = getLoc(vpoint);
    if(cursorType == VCursorType::CHOOSE)
    {
        VPointGroupShape * pgs = dynamic_cast<VPointGroupShape *>(focusShape);

        if((crPos == -1 && groupShape.contains(pos)) && (pgs == nullptr || pgs->atPoints(focusShape->transformPoint(pos)) == -1))
        {
            this->setCursor(Qt::SizeAllCursor);
        }
        else if(crPos < 8 && (crPos >= 0 || (focusShape!=nullptr && focusShape->atCrPoints(focusShape->transformPoint(pos),scale) != -1)))
        {
            this->setCursor(QCursor(VSizeAll, 15, 15));
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
                VPoint lp = groupShape.transformPoint(getLoc(lastMove));
                //qDebug() << pos << lp;
                //qDebug() << loc;
                VPoint v(pos.x-lp.x, pos.y-lp.y);
                if(crPos == -1)
                {
                    focusShape->moveLoc(focusShape->transformPoint(v+loc));
                    this->setCursor(Qt::SizeAllCursor);
                }
                else if(crPos < 8)
                {
                    //qDebug()<<"move"<<crPos;
                    focusShape->changeMag(crPos, focusShape->transformPoint(pos));
                }
                else if(crPos >= 8)
                {
                    VPointGroupShape * shape=dynamic_cast<VPointGroupShape *>(focusShape);
                    if(shape!=nullptr)
                    {
                        shape->changePoint(crPos - 8, focusShape->transformPoint(lp));
                    }
                }
            }

            update();
        }
        else if(cursorType == VCursorType::ROTATE)
        {
            if(focusShape != nullptr)
            {
                VVector vlp(focusShape->getLocation(), getLoc(lastMove)),
//                        vlm(focusShape->getLocation(), getLoc(lastMove)),
                        vnow(focusShape->getLocation(), pos);
//                qDebug() << VVector::rotationAngle(vlp, vnow)
//                         << vnow+VPoint(0,0) << vlm+VPoint(0,0);
                focusShape->getTransform().rotate(VVector::rotationAngle(vlp, vnow));
                update();
            }
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

    painter.save();
    {
        painter.scale(scale,scale);
        painter.save();
        {
            painter.setBrush(QBrush(Qt::white));
            painter.setPen(QPen(Qt::lightGray,1));
            painter.drawRect(-canvasSize.width/2, -canvasSize.height/2, canvasSize.width, canvasSize.height);
        }
        painter.restore();
        groupShape.draw(&painter,groupShape.getTransform());
    }
    painter.restore();

    if(focusShape != nullptr)
    {
        painter.save();
        VTransform trans;
        trans.scale(VMagnification(scale));
        trans=focusShape->getTransform()*trans;
        focusShape->drawCR(&painter,trans,scale);
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
                focusShape->moveLoc(focusShape->transformPoint(VPoint(loc.x,loc.y-1)));
            }
            break;
        case Qt::Key_Down:
            if(focusShape!=nullptr)
            {
                VPoint loc=focusShape->getLocation();
                focusShape->moveLoc(focusShape->transformPoint(VPoint(loc.x,loc.y+1)));
            }
            break;
        case Qt::Key_Left:
            if(focusShape!=nullptr)
            {
                VPoint loc=focusShape->getLocation();
                focusShape->moveLoc(focusShape->transformPoint(VPoint(loc.x-1,loc.y)));
            }
            break;
        case Qt::Key_Right:
            if(focusShape!=nullptr)
            {
                VPoint loc=focusShape->getLocation();
                focusShape->moveLoc(focusShape->transformPoint(VPoint(loc.x+1,loc.y)));
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
        crPos = -1;
    }break;
    case VCursorType::MOVE:
    {
        this->setCursor(Qt::OpenHandCursor);
        crPos = -1;
    }break;
    case VCursorType::ROTATE:
    {
        this->setCursor(QCursor(VRotate, 15, 15));
        crPos = -1;
    }break;
    default:
    {
        this->setCursor(Qt::ArrowCursor);
        crPos = -1;
    }
    }
}

VShape * TestWidget::getShape(const VPoint &point)
{
    VPoint subPoint;
    VPoint loc = getLoc(point);
//    qDebug() << loc;
    VMagnification subMag;
    if(focusShape!=nullptr)
    {
//        subLocation = focusShape->getLocation();
//        subAngle = focusShape->getAngle();
//        subMag = focusShape->getMagnification();
//        subPoint = VPoint(loc.x - subLocation.x, loc.y - subLocation.y).rotate(VPoint(0,0),-subAngle)/subMag;
        subPoint=focusShape->transformPoint(loc);
//        qDebug() << subPoint;
//        qDebug() << subMag;
        if(focusShape->contains(subPoint))
        {
//            qDebug() << it->type();
            return focusShape;
        }
    }
    for(VShape * it:this->groupShape.getShapeVector())
    {
        subPoint = it->transformPoint(loc);
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
