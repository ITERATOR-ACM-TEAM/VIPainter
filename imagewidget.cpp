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
#include <cmath>
#include "canvassizedialog.h"

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
    }
    else QMessageBox::warning(this,tr("错误"),format+tr("不能识别的文件格式"));
}

bool ImageWidget::fileChanged()
{
    //TODO:
    return false;
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
    return VPoint(floor(point.x*canvas.width()/this->width()),floor(point.y*canvas.height()/this->height()));
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

    painter.save();
    painter.scale((qreal)this->width()/canvas.width(),(qreal)this->height()/canvas.height());
    painter.drawImage(0,0,canvas);
    painter.restore();

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

void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    VPoint vpoint(qpoint.x(), qpoint.y());
    VPoint loc = getLoc(vpoint);
    if(cursorType == VCursorType::PEN)
    {
        if(event->button()==Qt::LeftButton)
        {
            QPainter painter(&canvas);
            painter.drawPoint(loc.toQPointF());
            update();
        }
    }
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    VPoint vpoint(qpoint.x(), qpoint.y());
    VPoint loc = getLoc(vpoint);
    mainwindow->statusBar()->showMessage(QString("%1,%2").arg(loc.x).arg(loc.y));
    if(cursorType == VCursorType::CHOOSE)
    {
    }
    else if(cursorType == VCursorType::PLUGIN)
    {
    }

    if(event->buttons()&Qt::LeftButton)
    {
        switch(cursorType)
        {
        case VCursorType::MOVE:
        {
        }break;
        case VCursorType::CHOOSE:
        {
        }break;
        case VCursorType::ROTATE:
        {

        }break;
        case VCursorType::PEN:
        {
            if(loc!=locMove)
            {
                QPainter painter(&canvas);
                painter.drawLine(locMove.toQPointF(),loc.toQPointF());
                update();
            }
        }break;
        case VCursorType::BEZIERCURVE:
        case VCursorType::POLYLINE:
        {
        }break;
        default:
            break;
        }
    }
    locMove = loc;
    lastMove=vpoint;
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
        default:
            break;
        }
        return false;
    }
    if(qobject_cast<QScrollBar*>(obj)!=nullptr&&ev->type()==QEvent::Wheel)return true;
    return false;
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
