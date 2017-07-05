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
#include "vtext.h"
#include "changetextdialog.h"
#include "vroundedrectangle.h"
#include <QPainter>
#include <QSize>
#include <QPen>
#include <QBrush>
#include <QtMath>
#include <QStatusBar>
#include <QDebug>
#include <QVector>
#include <QAction>
#include <QApplication>

TestWidget::TestWidget(QMainWindow *parent, bool antialiasing) :
    QWidget(parent),canvasLocation(0,0),canvasSize(400,300),cursorType(VCursorType::CHOOSE),crPos(-1),antialiasing(antialiasing)
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
        if(!focusShapes.empty())
        {
            //qDebug() << focusShape->transformPoint(getLoc(point));
            for(VShape *shape:focusShapes)
            {
                crPos = shape->atCrPoints(shape->transformPoint( getLoc(point)),scale);
                if(crPos == -1)
                {
                    VPointGroupShape * pgs = dynamic_cast<VPointGroupShape *>(shape);
                    if (pgs != nullptr)
                    {
                        int tmp = pgs->atPoints(shape->transformPoint( getLoc(point)));
                        if(tmp != -1)
                        {
                            crPos = tmp+8;
                            focusShapes.clear();
                            focusShapes.append(shape);
                            break;
                        }
                    }
                }
            }
        }

        if(crPos == -1)
        {
            VShape *shape= getShape(point);
            if(shape!=nullptr)
            {
                if(std::find(focusShapes.begin(),focusShapes.end(),shape)==focusShapes.end())
                {
                    if(QApplication::keyboardModifiers () != Qt::ControlModifier)focusShapes.clear();
                    focusShapes.append(shape);
                }
            }
            else if(QApplication::keyboardModifiers () != Qt::ControlModifier)focusShapes.clear();
        }

        update();
    }
    else if(cursorType == VCursorType::ROTATE)
    {
        if(!focusShapes.empty())
        {
            VShape *shape= focusShapes.back();
            focusShapes.clear();
            focusShapes.append(shape);
            lastAngle = 0;
        }
    }
    lastMove=VPoint(pressPoint.x(),pressPoint.y());
    lastPress=getLoc(lastMove);
}

void TestWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QPoint qpoint=event->pos();
    VPoint point(qpoint.x(),qpoint.y());
    point=getLoc(point);
    if(event->button()==Qt::LeftButton)
    {
        VShape *shape=groupShape.atShape(point);
        if(shape!=nullptr)
        {
            QPoint pressPoint=event->pos();
            VPoint point(pressPoint.x(), pressPoint.y());
            //qDebug()<<point.x<<" "<<point.y<<endl;
            VText * vt = dynamic_cast<VText *>(shape);
            VPolygon *vpg = dynamic_cast<VPolygon *>(shape);
            if (vpg != nullptr)
            {
                ChangeTextDialog::showDialog(vpg->getText());
            }
            if(vt != nullptr)
            {
                ChangeTextDialog::showDialog(vt);
            }
        }
    }
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
        bool flag=true;
        for(int i=focusShapes.size()-1;i>=0;i--)
        {
            if(crPos < 8 && (crPos >= 0 || (focusShapes[i]->atCrPoints(focusShapes[i]->transformPoint(pos),scale) != -1)))
            {
                this->setCursor(QCursor(VSizeAll, 15, 15));
                flag=false;
                break;
            }
        }
        if(flag)
        {
            for(int i=groupShape.getVectorSize()-1;i>=0;i--)
            {
                VPointGroupShape * pgs = dynamic_cast<VPointGroupShape *>(groupShape.getShapes().at(i));

                if((crPos == -1 && groupShape.contains(pos)) && (pgs == nullptr || pgs->atPoints(pgs->transformPoint(pos)) == -1))
                {
                    this->setCursor(Qt::SizeAllCursor);
                    flag=false;
                    break;
                }
            }
        }
        if(flag)this->setCursor(Qt::ArrowCursor);
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
            if(!focusShapes.empty())
            {
                //VPoint loc = focusShapes.first()->getLocation();
                VPoint lp = groupShape.transformPoint(getLoc(lastMove));
                //qDebug() << pos << lp;
                //qDebug() << loc;
                VPoint v(pos.x-lp.x, pos.y-lp.y);
                if(crPos == -1)
                {
                    for(VShape *shape:focusShapes)
                    {
                        shape->moveLoc(shape->transformPoint(v+shape->getLocation()));
                    }
                    this->setCursor(Qt::SizeAllCursor);
                }
                else if(crPos < 8)
                {
                    //qDebug()<<"move"<<crPos;
                    if(focusShapes.size()==1)
                    {
                        VShape *shape=focusShapes.first();
                        shape->changeMag(crPos, shape->transformPoint(pos));
                    }
                }
                else if(crPos >= 8)
                {
                    if(focusShapes.size()==1)
                    {
                        VPointGroupShape * shape=dynamic_cast<VPointGroupShape *>(focusShapes.first());
                        if(shape!=nullptr)
                        {
                            shape->changePoint(crPos - 8, shape->transformPoint(lp));
                        }
                    }
                }
            }

            update();
        }
        else if(cursorType == VCursorType::ROTATE)
        {
            if(focusShapes.size()==1)
            {
                VShape *shape=focusShapes.first();
                VVector vlp(shape->getLocation(), getLoc(lastMove)),
//                        vlm(focusShape->getLocation(), getLoc(lastMove)),
                        vnow(shape->getLocation(), pos);
//                qDebug() << VVector::rotationAngle(vlp, vnow)
//                         << vnow+VPoint(0,0) << vlm+VPoint(0,0);
                VPoint loc=shape->getLocation();
                shape->getTransform().translate(
                            shape->getTransform().inverted().map(VPoint(0,0))
                            );
                VTransform trans;
                trans.rotate(VVector::rotationAngle(vlp, vnow));
                shape->getTransform()*=trans;
                shape->getTransform().translate(
                            shape->getTransform().inverted().map(loc)
                            );
                update();
            }
        }
    }
    VPoint point(qpoint.x()-(this->width()/2+canvasLocation.x),qpoint.y()-(this->height()/2+canvasLocation.y));
    mainwindow->statusBar()->showMessage(QString("%1,%2").arg(floor(point.x/scale+0.5)).arg(floor(point.y/scale+0.5)));
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

    for(VShape *shape:focusShapes)
    {
        painter.save();
        VTransform trans;
        trans.scale(VMagnification(scale));
        trans=shape->getTransform()*trans;
        shape->drawCR(&painter,trans,scale);
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
            for(auto &shape:focusShapes)
            {
                VPoint loc=shape->getLocation();
                shape->moveLoc(shape->transformPoint(VPoint(loc.x,loc.y-1)));
            }
            break;
        case Qt::Key_Down:
            for(auto &shape:focusShapes)
            {
                VPoint loc=shape->getLocation();
                shape->moveLoc(shape->transformPoint(VPoint(loc.x,loc.y+1)));
            }
            break;
        case Qt::Key_Left:
            for(auto &shape:focusShapes)
            {
                VPoint loc=shape->getLocation();
                shape->moveLoc(shape->transformPoint(VPoint(loc.x-1,loc.y)));
            }
            break;
        case Qt::Key_Right:
            for(auto &shape:focusShapes)
            {
                VPoint loc=shape->getLocation();
                shape->moveLoc(shape->transformPoint(VPoint(loc.x+1,loc.y)));
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
    for(VShape *shape:focusShapes)
    {
//        subLocation = focusShape->getLocation();
//        subAngle = focusShape->getAngle();
//        subMag = focusShape->getMagnification();
//        subPoint = VPoint(loc.x - subLocation.x, loc.y - subLocation.y).rotate(VPoint(0,0),-subAngle)/subMag;
        subPoint=shape->transformPoint(loc);
//        qDebug() << subPoint;
//        qDebug() << subMag;
        if(shape->contains(subPoint))
        {
//            qDebug() << it->type();
            return shape;
        }
    }
    return this->groupShape.atShape(loc);
    //return nullptr;
}

VPoint TestWidget::getLoc(const VPoint & point)
{
    return VPoint((point.x-(this->width()/2+canvasLocation.x))/scale,(point.y-(this->height()/2+canvasLocation.y))/scale);
}
