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
#include "paintwidget.h"
#include <QFile>
#include <QImage>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>

PaintWidget::PaintWidget(QWidget *parent,bool antialiasing) : QWidget(parent),canvasSize(800,600),antialiasing(antialiasing),cursorType(VCursorType::CHOOSE)
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

void PaintWidget::on_actionAntialiasing_toggled(bool antialiasing)
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
                                            "svg file (*.svg);;"
                                            "png file (*.png);;"
                                            "jpg file (*.jpg);;"
                                            "bmp file (*.bmp)"));
    saveFile(filename);
}

void PaintWidget::changeCursor(VCursorType type,VShape *plugin)
{
    if(cursorType==VCursorType::PLUGIN)update();
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
    case VCursorType::ROTATE:
    {
        this->setCursor(QCursor(VRotate, 15, 15));
    }break;
    case VCursorType::POLYLINE: case VCursorType::BEZIERCURVE:
    {
        static QCursor pen = QCursor(QPixmap(":/icon/pen.png").scaled(20,20), 0, 19);
        this->setCursor(pen);
    }break;
    case VCursorType::PLUGIN:
    {
        this->setCursor(Qt::ArrowCursor);
        this->plugin=plugin;
    }break;
    default:
    {
        this->setCursor(Qt::ArrowCursor);
    }
    }
}
