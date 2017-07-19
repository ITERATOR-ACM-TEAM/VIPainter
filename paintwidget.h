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
#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QDockWidget>
#include "vpoint.h"
#include "vcursortype.h"

class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0, bool antialiasing=true);

protected:
    QString filename;
    double scale=1;
    VSize canvasSize;
    bool antialiasing;
    QDockWidget *dock;

public:
    void setScale(double scale);
    double getScale();
    void setDock(QDockWidget * dock);
    QDockWidget * getDock();
    void setCanvasSize(VSize canvasSize);
    VSize getCanvasSize();
    void setFileName(QString filename);
    QString getFileName()const;
    virtual void saveFile(QString filename)=0;

signals:

public slots:
    virtual void changeCursor(VCursorType type)=0;
    virtual void on_actionZoomIn_triggered();
    virtual void on_actionZoomOut_triggered();
    virtual void on_actionResume_triggered();
    virtual void on_actionSave_triggered();
    virtual void on_actionSaveAs_triggered();
    virtual void on_actionCanvasSize_triggered()=0;
    virtual void on_actionShapeSize_triggered()=0;
    virtual void on_actionBreakUp_triggered()=0;
    virtual void on_actionRedo_triggered()=0;
    virtual void on_actionUndo_triggered()=0;
    virtual void on_actionAntialiasing_toggled(bool antialiasing);
    virtual void on_actionDelete_triggered()=0;
    virtual void on_actionCopy_triggered()=0;
    virtual void on_actionCut_triggered()=0;
    virtual void on_actionPaste_triggered()=0;
    virtual void on_actionGroup_triggered()=0;
    virtual void changeModelData(const QModelIndex &index)=0;
    virtual void on_actionSelectAll_triggered()=0;
    virtual void on_actionBrush_triggered()=0;
    virtual void on_actionPen_triggered()=0;
    virtual void on_actionPenStyle_triggered()=0;
};

#endif // PAINTWIDGET_H
