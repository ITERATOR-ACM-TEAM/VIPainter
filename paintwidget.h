#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QDockWidget>
#include "vpoint.h"

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
    void setAntialiasing(bool antialiasing);
    virtual void saveFile(QString filename)=0;

signals:

public slots:
    virtual void on_actionZoomIn_triggered();
    virtual void on_actionZoomOut_triggered();
    virtual void on_actionResume_triggered();
    virtual void on_actionSave_triggered();
    virtual void on_actionSaveAs_triggered();
//    virtual void on_actionOpen_triggered();
//    virtual void on_actionMove_triggered();
//    virtual void on_actionChoose_triggered();
//    virtual void changeCursor(VCursorType type);
//    virtual void on_actionNew_triggered();
//    virtual void on_actionCanvasSize_triggered();
//    virtual void on_actionShapeSize_triggered();
//    virtual void on_actionBreakUp_triggered();
//    virtual void on_actionRotate_triggered();
//    virtual void on_actionRedo_triggered();
//    virtual void on_actionReloadPlugon_triggered();
//    virtual void on_actionLoadExPlugin_triggered();
//    virtual void on_actionAntialiasing_toggled(bool antialiasing);
//    virtual void on_actionDelete_triggered();
//    virtual void on_actionClose_triggered();
//    virtual void on_actionCopy_triggered();
//    virtual void on_actionCut_triggered();
//    virtual void on_actionPaste_triggered();
//    virtual void on_actionGroup_triggered();
//    virtual void initAction(QDir dir=QDir("plugin"));
//    virtual void focusDock(bool checked);
//    virtual void changeShapeName(const QModelIndex &index);
//    virtual void on_actionSelectAll_triggered();
//    virtual void on_actionBrush_triggered();
//    virtual void on_actionPen_triggered();
//    virtual void on_actionPenStyle_triggered();
//    virtual void on_actionDraw_triggered();
//    virtual void on_actionAbout_triggered();
//    virtual void on_actionUndo_triggered();
//    virtual void on_actionPolyLine_triggered();
//    virtual void on_actionCurveLine_triggered();
};

#endif // PAINTWIDGET_H
