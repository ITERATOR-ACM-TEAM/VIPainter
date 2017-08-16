#include "imagewidget.h"
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QRect>
#include <QStatusBar>
#include <QScrollBar>
#include <QBrush>
#include <QPen>
#include <QMessageBox>
#include <QKeyEvent>
#include <cmath>
#include <QApplication>
#include "canvassizedialog.h"
#include "vtransform.h"
#include "vpointgroupshape.h"

ImageWidget::ImageWidget(QMainWindow *mainwindow, bool antialiasing):PaintWidget(mainwindow,antialiasing)
{
    setWindowTitle(tr("画图"));
    setMouseTracking(true);

    scrollArea=new QScrollArea;
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(false);
    scrollArea->setWidget(this);
    scrollArea->setWindowTitle(this->windowTitle());
    scrollArea->installEventFilter(this);
    scrollArea->setMouseTracking(true);
    scrollArea->verticalScrollBar()->installEventFilter(this);
    scrollArea->horizontalScrollBar()->installEventFilter(this);
    this->move(1,1);
}

void ImageWidget::saveFile(QString filename)
{
    if(filename=="")return;
    QString format=filename.split('.').back().toUpper();
    if(format=="JPG"||format=="PNG"||format=="BMP")
    {
        if(!canvas.save(filename,format.toStdString().c_str(),100))QMessageBox::warning(this,tr("错误"),tr("保存文件")+filename+tr("失败"));
        else if(getFileName()=="")
        {
            dock->setWindowTitle(filename.split("/").back()+" - "+windowTitle());
            setFileName(filename);
        }
    }
    else QMessageBox::warning(this,tr("错误"),format+tr("不能识别的文件格式"));
}

bool ImageWidget::fileChanged()
{
    return swapQueue.changed();
}

QImage& ImageWidget::getCanvas()
{
    return canvas;
}

void ImageWidget::setCanvasSize(VSize canvasSize)
{
    QImage tmp(canvasSize.toQSizeF().toSize(),QImage::Format_ARGB32_Premultiplied);
    tmp.fill(Qt::transparent);
    QPainter painter;
    painter.begin(&tmp);
    painter.drawImage(0,0,canvas);
    painter.end();
    canvas=std::move(tmp);
    this->resize(canvas.width()*scale,canvas.height()*scale);
}

void ImageWidget::setImageSize(VSize size)
{
    canvas=canvas.scaled(size.toQSizeF().toSize());
    this->resize(canvas.width()*scale,canvas.height()*scale);
}

void ImageWidget::setScale(double scale)
{
    PaintWidget::setScale(scale);
    this->resize(canvas.width()*scale,canvas.height()*scale);
}

VSize ImageWidget::getCanvasSize()
{
    return canvas.size();
}

VMagnification ImageWidget::getScale()
{
    return VSize(this->width(),this->height())/getCanvasSize();
}

void ImageWidget::on_actionCanvasSize_triggered()
{
    setCanvasSize(CanvasSizeDialog::showDialog(tr("画布大小"),getCanvasSize()));
    update();
}

void ImageWidget::on_actionShapeSize_triggered()
{
    setImageSize(CanvasSizeDialog::showDialog(tr("图像大小"),getCanvasSize()));
    update();
}

VPoint ImageWidget::getLoc(const VPoint & point)
{
    return (point/getScale()).floor();
}

QScrollArea* ImageWidget::getScrollArea()
{
    return scrollArea;
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if(antialiasing)painter.setRenderHint(QPainter::Antialiasing);
    QImage tmp(canvas);
    if(focusShape!=nullptr)
    {
        QPainter tmpPainter;
        tmpPainter.begin(&tmp);
        if(antialiasing)tmpPainter.setRenderHint(QPainter::Antialiasing);
        focusShape->draw(&tmpPainter,focusShape->getTransform());
        tmpPainter.end();
    }

    painter.save();
    VMagnification mag=getScale();
    painter.scale(mag.horizontal,mag.vertical);
    painter.drawImage(0,0,tmp);
    painter.restore();
    if(focusShape!=nullptr)
    {
        VTransform trans;
        trans.scale(mag);
        focusShape->drawCR(&painter,focusShape->getTransform()*trans,mag.horizontal);
    }

    painter.setPen(QPen(QBrush(Qt::lightGray),0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter.drawRect(0,0,width(),height());
}

void ImageWidget::wheelEvent(QWheelEvent * event)
{
    QPoint point=event->pos();
    double oldScale=scale;
    if(event->delta() > 0){
        scale*=1.1;
        if(scale>20)scale=20;
    }else{
        scale/=1.1;
        if(scale<0.05)scale=0.05;
    }
    this->resize(canvas.width()*scale,canvas.height()*scale);
    scrollArea->horizontalScrollBar()->setValue(point.x()*scale/oldScale-(point.x()-scrollArea->horizontalScrollBar()->value()));
    scrollArea->verticalScrollBar()->setValue(point.y()*scale/oldScale-(point.y()-scrollArea->verticalScrollBar()->value()));
    update();
}

void ImageWidget::on_actionZoomIn_triggered()
{
    scale*=1.1;
    if(scale>20)scale=20;
    this->resize(canvas.width()*scale,canvas.height()*scale);
    update();
}

void ImageWidget::on_actionZoomOut_triggered()
{
    scale/=1.1;
    if(scale<0.05)scale=0.05;
    this->resize(canvas.width()*scale,canvas.height()*scale);
    update();
}

void ImageWidget::on_actionResume_triggered()
{
    scale=1.0;
    this->resize(canvas.size());
    update();
}

void ImageWidget::on_actionDelete_triggered()
{
    clearFocusShape();
    update();
}

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    VPoint vpoint(qpoint.x(), qpoint.y());
    VPoint loc = getLoc(vpoint);
    if(event->button()==Qt::LeftButton)
    {
        switch(cursorType)
        {
        case VCursorType::MOVE:
        {
            this->setCursor(Qt::ClosedHandCursor);
        }break;
        case VCursorType::CHOOSE:
        {
            if(focusShape!=nullptr)
            {
                crPos = focusShape->atCrPoints(focusShape->transformPoint(loc),scale);
                if(crPos == -1)
                {
                    VPointGroupShape * pgs = dynamic_cast<VPointGroupShape *>(focusShape);
                    if (pgs != nullptr)
                    {
                        int tmp = pgs->atPoints(focusShape->transformPoint(loc),scale);
                        if(tmp != -1)
                        {
                            crPos = tmp+8;
                            break;
                        }
                    }
                }

                if(crPos == -1)
                {
                    //if(!focusShape->contains(focusShape->transformPoint(loc)))finishFocusShape();
                }
            }

            update();
        }break;
        case VCursorType::ROTATE:
        {
        }break;
        case VCursorType::PEN:
        {
            finishFocusShape();
            QPainter painter(&canvas);
            if(antialiasing)painter.setRenderHint(QPainter::Antialiasing);
            painter.drawPoint(loc.toQPointF());
            update();
        }break;
        case VCursorType::PLUGIN:
        {
            finishFocusShape();
            focusShape=plugin->clone();
            focusShape->moveLoc(focusShape->transformPoint(loc));
            focusShape->changeMag(0,focusShape->transformPoint(loc+VPoint(1,1)),true);
            update();
        }break;
        default:
            break;
        }
        isPressing=true;
    }
    lastMove=vpoint;
    locMove=locPress=loc;
    globalMove=event->globalPos();
}

void ImageWidget::changeCursor(VCursorType type,VShape *plugin)
{
    PaintWidget::changeCursor(type,plugin);
    crPos=-1;
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    VPoint vpoint(qpoint.x(), qpoint.y());
    VPoint loc = getLoc(vpoint);
    VPoint globalPoint(event->globalPos());
    mainwindow->statusBar()->showMessage(QString("%1,%2").arg(loc.x).arg(loc.y));

    if(cursorType == VCursorType::CHOOSE)
    {
        if(focusShape!=nullptr)
        {
            bool flag=true;
            if(crPos < 8 && (crPos >= 0 || (focusShape->atCrPoints(focusShape->transformPoint(loc),scale) != -1)))
            {
                this->setCursor(QCursor(VSizeAll, 15, 15));
                flag=false;
            }
            if(flag)
            {
                VPointGroupShape * pgs = dynamic_cast<VPointGroupShape *>(focusShape);
                if(crPos>=8||(pgs != nullptr && pgs->atPoints(pgs->transformPoint(loc),scale) != -1))
                {
                    this->setCursor(Qt::ArrowCursor);
                    flag=false;
                }
            }
            if(flag)
            {
                if((event->buttons()&Qt::LeftButton)||focusShape->contains(focusShape->transformPoint(loc)))
                    this->setCursor(Qt::SizeAllCursor);
                else this->setCursor(Qt::ArrowCursor);
            }
        }
        else this->setCursor(Qt::ArrowCursor);
    }

    if(event->buttons()&Qt::LeftButton)
    {
        switch(cursorType)
        {
        case VCursorType::MOVE:
        {
            scrollArea->horizontalScrollBar()->setValue(scrollArea->horizontalScrollBar()->value()+globalMove.x-globalPoint.x);
            scrollArea->verticalScrollBar()->setValue(scrollArea->verticalScrollBar()->value()+globalMove.y-globalPoint.y);
        }break;
        case VCursorType::CHOOSE:
        {
            if(focusShape!=nullptr)
            {
                VPoint lp = locMove;
                VPoint v(loc.x-lp.x, loc.y-lp.y);
                if(crPos == -2)
                {
                    //do nothing
                }
                else if(crPos == -1)
                {
                    focusShape->moveLoc(focusShape->transformPoint(v+focusShape->getLocation()));
                    this->setCursor(Qt::SizeAllCursor);
                }
                else if(crPos < 8)
                {
                    focusShape->changeMag(crPos, focusShape->transformPoint(loc), QApplication::keyboardModifiers() == Qt::ControlModifier);
                }
                else if(crPos >= 8)
                {
                    VPointGroupShape * shape=dynamic_cast<VPointGroupShape *>(focusShape);
                    if(shape!=nullptr)
                    {
                        shape->changePoint(crPos - 8, shape->transformPoint(loc));
                    }
                }
            }
            update();
        }break;
        case VCursorType::ROTATE:
        {
            if(focusShape!=nullptr)
            {
                VVector vlp(focusShape->getLocation(), locMove),
                        vnow(focusShape->getLocation(), loc);
                VPoint loc=focusShape->getLocation();
                focusShape->getTransform().translate(
                            focusShape->getTransform().inverted().map(VPoint(0,0))
                            );
                VTransform trans;
                trans.rotate(VVector::rotationAngle(vlp, vnow));
                focusShape->getTransform()*=trans;
                focusShape->getTransform().translate(
                            focusShape->getTransform().inverted().map(loc)
                            );
                update();
            }
        }break;
        case VCursorType::PEN:
        {
            if(loc!=locMove)
            {
                QPainter painter(&canvas);
                if(antialiasing)painter.setRenderHint(QPainter::Antialiasing);
                painter.drawLine(locMove.toQPointF(),loc.toQPointF());
                update();
            }
        }break;
        case VCursorType::BEZIERCURVE:
        case VCursorType::POLYLINE:
        {
        }break;
        case VCursorType::PLUGIN:
        {
            if(focusShape!=nullptr)
            {
                focusShape->changeMag(0,focusShape->transformPoint(loc));
                update();
            }
        }break;
        default:
            break;
        }
    }
    locMove = loc;
    lastMove=vpoint;
    globalMove=globalPoint;
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
//    QPoint qpoint=event->pos();
//    VPoint loc=getLoc(VPoint(qpoint.x(),qpoint.y()));
    if(event->button()==Qt::LeftButton)
    {
        isPressing=false;
        switch(cursorType)
        {
        case VCursorType::MOVE:
        {
            this->setCursor(Qt::OpenHandCursor);
        }break;
        case VCursorType::CHOOSE:
        {
        }break;
        case VCursorType::ROTATE:
        {
        }break;
        case VCursorType::PEN:
        {
            saveSwp();
        }break;
        case VCursorType::PLUGIN:
        {
        }break;
        default:
            break;
        }
    }
    crPos = -1;
}

void ImageWidget::finishFocusShape()
{
    if(focusShape==nullptr)return;
    QPainter painter(&canvas);
    if(antialiasing)painter.setRenderHint(QPainter::Antialiasing);
    focusShape->draw(&painter,focusShape->getTransform());
    delete focusShape;
    focusShape=nullptr;
    saveSwp();
}


void ImageWidget::clearFocusShape()
{
    if(focusShape==nullptr)return;
    delete focusShape;
    focusShape=nullptr;
}

bool ImageWidget::eventFilter(QObject * obj, QEvent * ev)
{
    QScrollArea *scrollArea=qobject_cast<QScrollArea*>(obj);
    if(scrollArea!=nullptr)
    {
        switch (ev->type()) {
        case QEvent::MouseMove:
        {
            QMouseEvent *event=static_cast<QMouseEvent*>(ev);
            QMouseEvent *newEvent=new QMouseEvent(event->type(),
                                                 this->mapFrom(scrollArea,event->pos()),
                                                 event->windowPos(),
                                                 event->screenPos(),
                                                 event->button(),
                                                 event->buttons(),
                                                 event->modifiers(),
                                                 event->source()
                                                 );
            mouseMoveEvent(newEvent);
            delete newEvent;
        }break;
        case QEvent::Wheel:
        {
            QWheelEvent *event=static_cast<QWheelEvent*>(ev);
            QWheelEvent *newEvent=new QWheelEvent(QPointF(this->mapFrom(scrollArea,event->pos())),
                                           event->globalPosF(),
                                           event->pixelDelta(),
                                           event->angleDelta(),
                                           event->delta(),
                                           event->orientation(),
                                           event->buttons(),
                                           event->modifiers(),
                                           event->phase(),
                                           event->source()
                               );
            if(!this->rect().contains(newEvent->pos())&&this->mapToGlobal(newEvent->pos())==newEvent->globalPos())wheelEvent(newEvent);
            delete newEvent;
        }break;
        case QEvent::KeyPress:
        {
            QKeyEvent *event=static_cast<QKeyEvent*>(ev);
            switch(event->key())
            {
            case Qt::Key_Space:
            case Qt::Key_Return:
                finishFocusShape();
                update();
                break;
            case Qt::Key_Escape:
                clearFocusShape();
                update();
                break;
            }
        }break;
        default:
            break;
        }
        return false;
    }
    if(qobject_cast<QScrollBar*>(obj)!=nullptr&&ev->type()==QEvent::Wheel)return true;
    return false;
}

void ImageWidget::saveSwp()
{
    swapQueue.push(canvas);
}

void ImageWidget::on_actionSaveAs_triggered()
{
    QString filename=getFileName();
    if(filename=="")filename="image.jpg";
    filename=
            QFileDialog::getSaveFileName(this,
                                         tr("保存文件"),
                                         filename,
                                         tr(
                                            "jpg file (*.jpg);;"
                                            "png file (*.png);;"
                                            "bmp file (*.bmp)"));
    saveFile(filename);
}

void ImageWidget::on_actionUndo_triggered()
{
    if(swapQueue.atFirst())return;
    clearFocusShape();
    canvas=swapQueue.undo();
    setScale(scale);
    update();
}

void ImageWidget::on_actionRedo_triggered()
{
    if(swapQueue.atLast())return;
    clearFocusShape();
    canvas=swapQueue.redo();
    setScale(scale);
    update();
}
