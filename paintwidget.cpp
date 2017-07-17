#include "paintwidget.h"
#include <QFile>
#include <QImage>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>

PaintWidget::PaintWidget(QWidget *parent,bool antialiasing) : QWidget(parent),canvasSize(800,600),antialiasing(antialiasing)
{

}

void PaintWidget::setScale(double scale)
{
    this->scale=scale;
}

double PaintWidget::getScale()
{
    return scale;
}

void PaintWidget::setDock(QDockWidget * dock)
{
    this->dock=dock;
}

QDockWidget* PaintWidget::getDock()
{
    return dock;
}

void PaintWidget::setCanvasSize(VSize canvasSize)
{
    this->canvasSize=canvasSize;
}

VSize PaintWidget::getCanvasSize()
{
    return canvasSize;
}

void PaintWidget::setFileName(QString filename)
{
    this->filename=filename;
}

QString PaintWidget::getFileName() const
{
    return filename;
}

void PaintWidget::setAntialiasing(bool antialiasing)
{
    this->antialiasing=antialiasing;
    //qDebug()<<"antialiasing"<<antialiasing;
    update();
}

void PaintWidget::on_actionZoomIn_triggered()
{
    scale*=1.1;
    update();
}

void PaintWidget::on_actionZoomOut_triggered()
{
    scale/=1.1;
    update();
}

void PaintWidget::on_actionResume_triggered()
{
    scale=1.0;
    update();
}

void PaintWidget::on_actionSave_triggered()
{
    QString filename = getFileName();
    if(filename=="")on_actionSaveAs_triggered();
    else saveFile(filename);
}

void PaintWidget::on_actionSaveAs_triggered()
{
    QString filename=getFileName();
    if(filename=="")filename="image.json";
    filename=
            QFileDialog::getSaveFileName(this,
                                         tr("保存文件"),
                                         filename,
                                         tr("json file (*.json);;"
                                            "png file (*.png);;"
                                            "jpg file (*.jpg);;"
                                            "bmp file (*.bmp)"));
    saveFile(filename);
}
