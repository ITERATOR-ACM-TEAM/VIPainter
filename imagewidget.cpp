#include "imagewidget.h"
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QRect>
#include <QStatusBar>
#include <QBrush>
#include <QPen>
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
    this->move(1,1);
}

void ImageWidget::saveFile(QString filename)
{
    //TODO:
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
    tmp.fill(Qt::white);
    QPainter painter;
    painter.begin(&tmp);
    painter.drawImage(0,0,canvas);
    painter.end();
    canvas=std::move(tmp);
}

void ImageWidget::setImageSize(VSize size)
{
    canvas=canvas.scaled(size.toQSizeF().toSize(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
}

void ImageWidget::setScale(double scale)
{
    PaintWidget::setScale(scale);
    this->resize(canvas.size().width()*scale,canvas.size().height()*scale);
}

VSize ImageWidget::getCanvasSize()
{
    return canvas.size();
}

void ImageWidget::on_actionCanvasSize_triggered()
{
    setCanvasSize(CanvasSizeDialog::showDialog(tr("画布大小"),getCanvasSize()));
}

void ImageWidget::on_actionShapeSize_triggered()
{
    setImageSize(CanvasSizeDialog::showDialog(tr("图像大小"),getCanvasSize()));
}

VPoint ImageWidget::getLoc(const VPoint & point)
{
    return VPoint((point.x)/scale,(point.y)/scale);
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
    painter.drawRect(10,10,10,10);

    painter.save();
    painter.scale(scale,scale);
    painter.drawImage(0,0,canvas);
    painter.restore();

    painter.setPen(QPen(QBrush(Qt::lightGray),1,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
    painter.drawRect(0,0,width()-1,height()-1);
}

void ImageWidget::wheelEvent(QWheelEvent * event)
{
    if(event->delta() > 0){
        scale*=1.1;
        if(scale>10)scale=10;
    }else{
        scale/=1.1;
        if(scale<0.05)scale=0.05;
    }
    this->resize(canvas.size().width()*scale,canvas.size().height()*scale);
    update();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint qpoint=event->pos();
    VPoint vpoint(qpoint.x(), qpoint.y());
    VPoint loc = getLoc(vpoint);
    mainwindow->statusBar()->showMessage(QString("%1,%2").arg(floor(loc.x+0.5)).arg(floor(loc.y+0.5)));
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
        {return true;
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
            wheelEvent(newEvent);
            delete newEvent;
            if(this->rect().contains(this->mapFrom(scrollArea,event->pos())))return true;
        }break;
        default:
            break;
        }
        return false;
    }
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
