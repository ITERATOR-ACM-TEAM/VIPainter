/**
 * Copyright (C) 2017 VIPainter
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
#include "vectorgraphwidget.h"
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
#include "vbeziercurve.h"
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
#include <QCursor>
#include <QStringList>
#include <QColor>

VectorgraphWidget::VectorgraphWidget(QMainWindow *parent, bool antialiasing) :
    PaintWidget(parent,antialiasing),crPos(-1),canvasLocation(0,0),cursorType(VCursorType::CHOOSE)
{
    mainwindow=parent;
    setMouseTracking(true);
//    groupShape.setName("main shape");
//    groupShape.setLocation(VPoint(0,0));
//    groupShape.setSize(VSize(10,10));
    listModel=new QStringListModel(this);
    selectionModel=new QItemSelectionModel(listModel,this);
    connect(selectionModel,SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &))
            ,this,SLOT(changeFocus()));
    connect(this,SIGNAL(selected(const QItemSelection&,QItemSelectionModel::SelectionFlags)),
            selectionModel,SLOT(select(const QItemSelection&,QItemSelectionModel::SelectionFlags)));
    update();
}

VectorgraphWidget::~VectorgraphWidget()
{
}

void VectorgraphWidget::wheelEvent(QWheelEvent * event)
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

void VectorgraphWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPoint=event->pos();
    VPoint point(pressPoint.x(), pressPoint.y());
    if(event->button()==Qt::LeftButton)
    {
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
                            int tmp = pgs->atPoints(shape->transformPoint( getLoc(point)),scale);
                            if(tmp != -1)
                            {
                                crPos = tmp+8;
                                focusShapes.clear();
                                focusShapes.append(shape);
                                emitSelected();
                                break;
                            }
                        }
                    }
                    else
                    {
                        focusShapes.clear();
                        focusShapes.append(shape);
                        emitSelected();
                        break;
                    }
                }
            }

            if(crPos == -1)
            {
                VShape *shape= getShape(point);
                if(shape!=nullptr)
                {
                    auto it=std::find(focusShapes.begin(),focusShapes.end(),shape);
                    if(it==focusShapes.end())
                    {
                        if(QApplication::keyboardModifiers () != Qt::ControlModifier)focusShapes.clear();
                        focusShapes.append(shape);
                        emitSelected();
                    }
                    else
                    {
                        if(QApplication::keyboardModifiers () == Qt::ControlModifier)
                        {
                            focusShapes.erase(it);
                            emitSelected();
                        }
                    }
                }
                else
                {
                    if(QApplication::keyboardModifiers () != Qt::ControlModifier)
                    {
                        focusShapes.clear();
                        emitSelected();
                    }
                    crPos=-2;
                }
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
                emitSelected();
                lastAngle = 0;
            }
        }
        else if(cursorType == VCursorType::DRAWPOLYLINE || cursorType == VCursorType::DRAWBEZIERCURVE)
        {
            VPointGroupShape * pl;
            if(crPos == -1)
            {
//                qDebug() << "!!!!!!";
                if(cursorType == VCursorType::DRAWPOLYLINE)
                    pl = new VPolyline();
                else if (cursorType == VCursorType::DRAWBEZIERCURVE)
                    pl = new VBezierCurve();
                groupShape.insertShape(pl);
                pl->moveLoc(pl->transformPoint(getLoc(point)));
                focusShapes.push_back(pl);

//                pl->addPoint(pl->transformPoint(getLoc((point))));
//                crPos++;
                updateList();
            }
            else
            {
                pl = dynamic_cast<VPointGroupShape *>(focusShapes[0]);
            }

//            qDebug() << pl << focusShapes.size() << crPos;
            pl->addPoint(pl->transformPoint(getLoc((point))));
            crPos++;
            update();
        }
    }
    lastMove=VPoint(pressPoint.x(),pressPoint.y());
    locMove=locPress=getLoc(lastMove);
}

void VectorgraphWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QPoint qpoint=event->pos();
    VPoint point(qpoint.x(),qpoint.y());
    point=getLoc(point);
    if(event->button()==Qt::LeftButton)
    {
        VShape *shape=groupShape.atShape(point);
        if(shape!=nullptr)
        {
            //qDebug()<<point.x<<" "<<point.y<<endl;
            VText * vt = dynamic_cast<VText *>(shape);
            VPolygon *vpg = dynamic_cast<VPolygon *>(shape);
            if (vpg != nullptr)
            {
                ChangeTextDialog::showDialog(vpg->getText());
                saveSwp();
            }
            if(vt != nullptr)
            {
                ChangeTextDialog::showDialog(vt);
                saveSwp();
            }
        }
    }
}

void VectorgraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    VPoint pos=getLoc(VPoint(qpoint.x(),qpoint.y()));
    if(event->button()==Qt::LeftButton)
    {
        if(cursorType == VCursorType::MOVE)
            this->setCursor(Qt::OpenHandCursor);
        else if(cursorType==VCursorType::CHOOSE)
        {
            if(crPos==-2)
            {
                QRectF rect(locPress.x,locPress.y,
                            pos.x-locPress.x,pos.y-locPress.y);
                for(VShape *shape:groupShape.getShapes())
                {
                    bool flag=true;
                    for(const VPoint &point:shape->getSizeRect())
                    {
                        VPoint p=shape->reverseTransformPoint(point);
                        if(!rect.contains(p.x,p.y))
                        {
                            flag=false;
                            break;
                        }
                    }
                    if(flag)
                    {
                        focusShapes.append(shape);
                        emitSelected();
                    }
                }
                std::sort(focusShapes.begin(),focusShapes.end());
                std::unique(focusShapes.begin(),focusShapes.end());
                //emit selected(listModel.createIndex(1,0));
                update();
            }
            else if(pos.x!=locPress.x||pos.y!=locPress.y)
            {
                saveSwp();
            }
        }
        else if(cursorType==VCursorType::ROTATE)
        {
            if(focusShapes.size()==1&&(pos.x!=locPress.x||pos.y!=locPress.y))
            {
                saveSwp();
            }
        }
        if(cursorType != VCursorType::DRAWBEZIERCURVE && cursorType != VCursorType::DRAWPOLYLINE)
            crPos = -1;
    }
}

void VectorgraphWidget::mouseMoveEvent(QMouseEvent *event)
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

                if((crPos == -1 && groupShape.contains(pos)) && (pgs == nullptr || pgs->atPoints(pgs->transformPoint(pos),scale) == -1))
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
            canvasLocation.x+=(vpoint.x-lastMove.x);
            canvasLocation.y+=(vpoint.y-lastMove.y);
            //qDebug()<<"canvasLocation: ("<<canvasLocation.x<<","<<canvasLocation.y<<")"<<endl;
            update();
        }else if(cursorType == VCursorType::CHOOSE){
            if(!focusShapes.empty())
            {
                //VPoint loc = focusShapes.first()->getLocation();
                VPoint lp = groupShape.transformPoint(locMove);
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
                            shape->changePoint(crPos - 8, shape->transformPoint(pos));
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
                VVector vlp(shape->getLocation(), locMove),
                        vnow(shape->getLocation(), pos);
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
        else if(cursorType == VCursorType::DRAWBEZIERCURVE || cursorType == VCursorType::DRAWPOLYLINE)
        {
            if(crPos > -1)
            {
                VPointGroupShape * shape=dynamic_cast<VPointGroupShape *>(focusShapes.first());
                if(shape!=nullptr)
                {
                    int index = shape->getPointList().size() - 1;
                    if(index >= 0)
                        shape->changePoint(index, shape->transformPoint(pos));
                    update();
                }
            }

        }
    }
    VPoint point(qpoint.x()-(this->width()/2+canvasLocation.x),qpoint.y()-(this->height()/2+canvasLocation.y));
    mainwindow->statusBar()->showMessage(QString("%1,%2").arg(floor(point.x/scale+0.5)).arg(floor(point.y/scale+0.5)));
    locMove = pos;
    lastMove=vpoint;
}

void VectorgraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
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

    if(crPos==-2)
    {
        VPoint point=locMove;
        painter.setPen(QPen(QBrush(Qt::gray),2,Qt::DotLine,Qt::SquareCap,Qt::MiterJoin));
        painter.setBrush(QColor(0xaa,0xaa,0xaa,9));
        painter.drawRect(locPress.x*scale,locPress.y*scale,
                         (point.x-locPress.x)*scale,(point.y-locPress.y)*scale);
    }

}


bool VectorgraphWidget::eventFilter(QObject * obj, QEvent * ev)
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
        case Qt::Key_Return:
        case Qt::Key_Escape:
            {
                if(cursorType == VCursorType::DRAWBEZIERCURVE || cursorType == VCursorType::DRAWPOLYLINE)
                {
                    focusShapes.clear();
                    crPos = -1;
                    saveSwp();
                    update();
                }
            }
        }
        update();
        return true;
    }
    return false;
}

void VectorgraphWidget::changeCursor(VCursorType type)
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
    case VCursorType::DRAWPOLYLINE: case VCursorType::DRAWBEZIERCURVE:
    {
        crPos = -1;
        focusShapes.clear();
        static QCursor pen = QCursor(QPixmap(":/icon/pen.png").scaled(20,20), 0, 19);
        this->setCursor(pen);
        update();
        updateList();
    }break;
    default:
    {
        this->setCursor(Qt::ArrowCursor);
        crPos = -1;
    }
    }
}

VShape * VectorgraphWidget::getShape(const VPoint &point)
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

VPoint VectorgraphWidget::getLoc(const VPoint & point)
{
    return VPoint((point.x-(this->width()/2+canvasLocation.x))/scale,(point.y-(this->height()/2+canvasLocation.y))/scale);
}

void VectorgraphWidget::updateList()
{
    QStringList list;
    for(int i=groupShape.getShapes().size()-1;i>=0;i--)
    {
        if(groupShape.getShapes().at(i)->getName()=="")groupShape.getShapes().at(i)->setName(tr("没有名字的图形"));
        list.append(groupShape.getShapes().at(i)->getName());
    }
    listModel->setStringList(list);
    emitSelected();
}

void VectorgraphWidget::changeFocus()
{
    if(cursorType == VCursorType::DRAWBEZIERCURVE || cursorType == VCursorType::DRAWPOLYLINE) return;
    decltype(focusShapes) newFocus;
    for(auto &index:selectionModel->selectedRows())
        newFocus.append(groupShape.getShapes().at(groupShape.getVectorSize()-index.row()-1));
    if(focusShapes!=newFocus)
    {
        focusShapes=std::move(newFocus);
        update();
    }
}

void VectorgraphWidget::emitSelected()
{
    QItemSelection list;
    for(VShape *shape:focusShapes)
    {
        QModelIndex index=listModel->index
                (groupShape.getShapes().end()-
                 std::find(groupShape.getShapes().begin(),groupShape.getShapes().end(),shape)-1
                 );
        list.select(index,index);
    }
    emit selected(list,QItemSelectionModel::ClearAndSelect);
}

void VectorgraphWidget::saveSwp()
{
    while(swpNow-swpL>=SWPSIZE)swpL++;
    swp[swpNow%SWPSIZE]=groupShape.toJsonArray();
    swpNow++;
    swpR=swpNow;
}

void VectorgraphWidget::undo()
{
    if(swpNow-swpL<=1)return;
    focusShapes.clear();
    swpNow--;
    groupShape=swp[(swpNow-1)%SWPSIZE];
    update();
    updateList();
}

void VectorgraphWidget::redo()
{
    if(swpNow>=swpR)return;
    focusShapes.clear();
    groupShape=swp[swpNow%SWPSIZE];
    swpNow++;
    update();
    updateList();
}

void VectorgraphWidget::on_actionResume_triggered()
{
    canvasLocation=VPoint(0,0);
    PaintWidget::on_actionResume_triggered();
}
