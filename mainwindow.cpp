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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vdocktitlebar.h"
#include "vtype.h"
#include "canvassizedialog.h"
#include "vtransform.h"
#include "vtext.h"
#include "penstyledialog.h"
#include <QJsonDocument>
#include <QApplication>
#include <QJsonObject>
#include <QFile>
#include <QScrollArea>
#include <QDockWidget>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QString>
#include <QFileDialog>
#include <QStringList>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QAction>
#include <QIcon>
#include <QList>
#include <QPixmap>
#include <QMessageBox>
#include <QTimer>
#include <QColorDialog>
#include <QContextMenuEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),focus(nullptr),cursorState(VCursorType::CHOOSE)
{
    ui->setupUi(this);
    delete takeCentralWidget();//去掉中心控件
    setDockNestingEnabled(true);//设置DOCK可随意移动
    this->setContextMenuPolicy(Qt::NoContextMenu);//取消右键菜单
    ui->menuTools->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuTools->addAction(ui->shapeBar->toggleViewAction());
    ui->menuTools->addAction(ui->shapesDock->toggleViewAction());
    setWindowTitle("VIPainter");
    ui->shapesDock->hide();

    //菜单逻辑
    connect(ui->menuEdit,&QMenu::aboutToShow,[this]{
        VectorgraphWidget *widget=qobject_cast<VectorgraphWidget *>(getPaintWidget());
        changeMenuAction(widget,VPoint(0,0));
    });

    //工具栏编组 bar group
    barGroup = new QActionGroup(this);
    barGroup->addAction(ui->actionChoose);
    barGroup->addAction(ui->actionMove);
    barGroup->addAction(ui->actionRotate);
    barGroup->addAction(ui->actionDraw);
    connect(this, SIGNAL(cursorChange(VCursorType)), this, SLOT(changeCursor(VCursorType)));
    QTimer::singleShot(0,this,SLOT(initAction(QDir)));

    //菜单编组 menu group
    menuGroup = new QActionGroup(this);
    menuGroup->addAction(ui->actionCurveLine);
    menuGroup->addAction(ui->actionPolyLine);

    //List View init
    listView=new VListView(this);
    ui->shapesDock->setWidget(listView);
    delegate=new VDelegate(this);
    listView->setItemDelegate(delegate);
    connect(delegate,SIGNAL(dataChanged(const QModelIndex &)),this,SLOT(changeShapeName(const QModelIndex &)));

    //contextMenu init
    contextMenu=new QMenu(this);
    contextMenu->addAction(ui->actionSelectAll);
    contextMenu->addAction(ui->actionCanvasSize);
    contextMenu->addSeparator();
    contextMenu->addAction(ui->actionCopy);
    contextMenu->addAction(ui->actionCut);
    contextMenu->addAction(ui->actionPaste);
    contextMenu->addAction(ui->actionDelete);
    contextMenu->addSeparator();
    contextMenu->addAction(ui->actionPen);
    contextMenu->addAction(ui->actionPenStyle);
    contextMenu->addAction(ui->actionBrush);
    contextMenu->addSeparator();
    contextMenu->addAction(ui->actionGroup);
    contextMenu->addAction(ui->actionBreakUp);

    update();
}

void MainWindow::loadPlugin(QString filename)
{
    ui->statusBar->showMessage(tr("加载插件 ")+filename);
    QFile file(filename);
    if(!file.open(QFile::ReadOnly|QFile::Text))return;
    QJsonDocument doc=QJsonDocument::fromJson(file.readAll());
    file.close();
    VShape *shape;
    if(doc.isObject()&&doc.object().value("type")==QString("canvas"))
    {
        doc.setArray(doc.object().value("shapes").toArray());
    }
    if(doc.isArray())
    {
        VGroupShape *groupshape=new VGroupShape(doc.array());
        groupshape->getCircumscribedRectangle(true);
        shape=groupshape;
    }
    else if(doc.isObject())shape=VShape::fromJsonObject(doc.object());
    else return;
    if(shape==nullptr)return;
    if(shape->getName()==QString(""))shape->setName(filename.split('/').back().split('.').first());
    QAction *action=new QAction(ui->shapeBar);

    const int SIZE=30;
    VSize size=shape->getSize();
    VMagnification mag;
    if(size.width>size.height)mag=SIZE/size.width;
    else mag=SIZE/size.height;

    QPixmap pixmap(SIZE,SIZE);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.translate(SIZE/2,SIZE/2);
    painter.setRenderHint(QPainter::Antialiasing);
    VTransform transform;
    transform.scale(mag);
    shape->draw(&painter,transform);

    action->setIcon(QIcon(pixmap));
    action->setToolTip(shape->getName());
    ui->shapeBar->addAction(action);

    connect(action,&QAction::triggered,[this,shape]{
        //qDebug()<<"add"<<*shape;
        VectorgraphWidget *widget=qobject_cast<VectorgraphWidget*>(getPaintWidget());
        if(widget==nullptr)return;
        VShape *newShape=shape->clone();
//        newShape->setPen(this->pen);
//        newShape->setBrush(this->brush);
        widget->groupShape.insertShape(newShape);
        widget->updateList();
        widget->update();
        widget->saveSwp();
    });
}

void MainWindow::changeShapeName(const QModelIndex &index)
{
    VectorgraphWidget *widget=qobject_cast<VectorgraphWidget*>(getPaintWidget());
    if(widget==nullptr)return;
    widget->groupShape.getShapes().at(widget->groupShape.getVectorSize()-index.row()-1)->setName(index.data().toString());
}

void MainWindow::initAction(QDir dir)
{
    dir.setFilter(QDir::Files|QDir::Readable);
    dir.setNameFilters(QStringList()<<"*.json"<<"*.JSON");
    QStringList files=dir.entryList();
    for(auto i:files)
    {
        loadPlugin(dir.filePath(i));
    }

}

QDockWidget* MainWindow::newDock(QString dockname)
{
    QDockWidget *old = focus;

    static int id = 0;

    VectorgraphWidget* widget=new VectorgraphWidget(this,ui->actionAntialiasing->isChecked());
    connect(ui->actionAntialiasing,SIGNAL(triggered(bool)),widget,SLOT(on_actionAntialiasing_toggled(bool)));
    connect(this, SIGNAL(cursorChange(VCursorType)), widget, SLOT(changeCursor(VCursorType)));
    emit cursorChange(this->cursorState);
    widget->installEventFilter(this);

    QDockWidget *dockWidget=new QDockWidget;
    dockWidget->installEventFilter(widget);
    dockWidget->setWidget(widget);
    widget->setDock(dockWidget);
    //dockWidget->setAttribute(Qt::WA_DeleteOnClose);
    this->addDockWidget(Qt::TopDockWidgetArea,dockWidget);

    dockWidget->setFocusPolicy(Qt::StrongFocus);
    dockWidget->installEventFilter(this);

    dockWidget->show();
    if(old != nullptr && !old->isFloating())
    {
        this->tabifyDockWidget(old, dockWidget);
    }
    else
    {
        for(auto it: docks)
        {
            if(!it->isFloating())
            {
                this->tabifyDockWidget(it, dockWidget);
            }
        }
    }
    //dockWidget->setTitleBarWidget(new VDockTitleBar());
    dockWidget->setBaseSize(400,300);

    this->focusDock(dockWidget);

    connect(dockWidget,SIGNAL(visibilityChanged(bool)),this,SLOT(focusDock(bool)));

    if(dockname=="")dockWidget->setWindowTitle(QString("untitled %1").arg(id++));
    else dockWidget->setWindowTitle(dockname);
    docks.push_back(dockWidget);

    //ui->shapesDock->show();
    if(!ui->shapesDock->isVisible())
    {
        ui->shapesDock->show();
        splitDockWidget(dockWidget,ui->shapesDock,Qt::Horizontal);
    }

    return dockWidget;
}

MainWindow::~MainWindow()
{
    for(auto &i:plugins)delete i;
    delete ui;
}

void MainWindow::on_actionZoomIn_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionZoomOut_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionResume_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionSave_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionSaveAs_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionOpen_triggered()
{
    QStringList filenames=
            QFileDialog::getOpenFileNames(this,
                                         tr("打开文件"),
                                         tr(""),
                                         tr("json file (*.json);;"
                                            "all (*)"));
    for(auto &filename:filenames)
    {
        if(filename=="")return;
        bool flag=false;
        for(auto &dock:docks)
        {
            VectorgraphWidget *widget=qobject_cast<VectorgraphWidget*>(getPaintWidget(dock));
            if(widget->getFileName()==filename)
            {
                focusDock(dock);
                flag=true;
            }
        }
        if(flag)continue;
        ui->statusBar->showMessage(tr("打开文件 ")+filename);
        QFile file(filename);
        if(!file.open(QFile::ReadOnly|QFile::Text))return;
        QJsonDocument doc=QJsonDocument::fromJson(file.readAll());
        file.close();
        if(!doc.isArray()&&!doc.isObject())
        {
            QMessageBox::warning(this,tr("错误"),tr("打开文件 ")+filename+tr("失败"));
            continue;
        }
        QDockWidget * newWidget = newDock(filename.split("/").back());
        VectorgraphWidget *widget=qobject_cast<VectorgraphWidget*>(getPaintWidget(newWidget));
        if(doc.isObject()&&doc.object().value("type")==QString("canvas"))
        {
            widget->setCanvasSize(doc.object().value("size").toObject());
            doc.setArray(doc.object().value("shapes").toArray());
        }
        if(doc.isArray())widget->groupShape=doc.array();
        else if(doc.isObject())
        {
            widget->groupShape.insertShape(VShape::fromJsonObject(doc.object()));
        }
        widget->setFileName(filename);
        widget->saveSwp();
        newWidget->update();
    }
}

void MainWindow::on_actionMove_triggered()
{
    if(ui->actionMove->isChecked())
        emit cursorChange(VCursorType::MOVE);
    else
        emit cursorChange(VCursorType::DEFAULT);
}

void MainWindow::on_actionChoose_triggered()
{
    if(ui->actionChoose->isChecked())
        emit cursorChange(VCursorType::CHOOSE);
    else
        emit cursorChange(VCursorType::DEFAULT);
}

void MainWindow::changeCursor(VCursorType type)
{
    if(cursorState == VCursorType::DRAWBEZIERCURVE || cursorState == VCursorType::DRAWPOLYLINE)
    {
        ui->actionDelete->setEnabled(true);
        ui->actionCopy->setEnabled(true);
        ui->actionCut->setEnabled(true);
        ui->actionRedo->setEnabled(true);
        ui->actionUndo->setEnabled(true);
        ui->actionGroup->setEnabled(true);
        ui->actionBreakUp->setEnabled(true);
        ui->actionSelectAll->setEnabled(true);
        for(auto &i:docks)
        {
            VectorgraphWidget *widget=qobject_cast<VectorgraphWidget*>(getPaintWidget(i));
            if(widget->crPos!=-1)widget->saveSwp();
        }
    }
    cursorState = type;
    if(type == VCursorType::DRAWBEZIERCURVE || type == VCursorType::DRAWPOLYLINE)
    {
        ui->actionDelete->setEnabled(false);
        ui->actionCopy->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionRedo->setEnabled(false);
        ui->actionUndo->setEnabled(false);
        ui->actionGroup->setEnabled(false);
        ui->actionBreakUp->setEnabled(false);
        ui->actionSelectAll->setEnabled(false);
        //for(auto &i:docks)getPaintWidget(i)->saveSwp();
    }
    for(auto &i:docks)
    {
        VectorgraphWidget *widget=qobject_cast<VectorgraphWidget*>(getPaintWidget(i));
        widget->crPos=-1;
    }

}

void MainWindow::on_actionNew_triggered()
{
    QDockWidget * dock = newDock();
    VectorgraphWidget *widget=qobject_cast<VectorgraphWidget*>(getPaintWidget(dock));
    widget->saveSwp();
}

//判断Action的显示状态
void MainWindow::changeMenuAction(VectorgraphWidget *widget, VPoint loc)
{
    if(cursorState == VCursorType::DRAWBEZIERCURVE || cursorState == VCursorType::DRAWPOLYLINE)return;
    bool flag=false;
    if(widget==nullptr||widget->focusShapes.empty())
    {
        ui->actionCopy->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionDelete->setEnabled(false);
        ui->actionPen->setVisible(false);
        ui->actionPenStyle->setVisible(false);
        ui->actionBrush->setVisible(false);
        ui->actionGroup->setVisible(false);
        ui->actionBreakUp->setVisible(false);
    }
    else
    {
        ui->actionCopy->setEnabled(true);
        ui->actionCut->setEnabled(true);
        ui->actionDelete->setEnabled(true);
        for(VShape *shape:widget->focusShapes)
        {
            if(shape->contains(shape->transformPoint(loc)))
            {
                flag=true;
                break;
            }
        }
        if(flag)
        {
            ui->actionPen->setVisible(true);
            ui->actionPenStyle->setVisible(true);
            ui->actionBrush->setVisible(true);
            ui->actionGroup->setVisible(true);
            ui->actionBreakUp->setVisible(true);
            if(widget->focusShapes.size()==1
                    &&widget->focusShapes.first()->type()==VType::GroupShape)
            {
                ui->actionBreakUp->setEnabled(true);
            }
            else ui->actionBreakUp->setEnabled(false);
        }
        else
        {
            ui->actionPen->setVisible(false);
            ui->actionPenStyle->setVisible(false);
            ui->actionBrush->setVisible(false);
            ui->actionGroup->setVisible(false);
            ui->actionBreakUp->setVisible(false);
        }
    }
}

bool MainWindow::closeWidget(VectorgraphWidget *widget)
{
    bool flag=false;
    QString filename=widget->getFileName();
    if(filename=="")
    {
        if(!widget->groupShape.getShapes().empty())flag=true;
    }
    else
    {
        QFile file(filename);
        if(file.open(QFile::ReadOnly|QFile::Text))
        {
            QJsonDocument doc=QJsonDocument::fromJson(file.readAll());
            file.close();
            QJsonDocument jsonDocument;
            QJsonObject obj;
            obj.insert("type",QString("canvas"));
            obj.insert("size",widget->getCanvasSize());
            obj.insert("shapes",widget->groupShape.toJsonArray());
            jsonDocument.setObject(obj);
            if(doc!=jsonDocument)flag=true;
        }
    }
    if(flag)
    {
        int button
                =QMessageBox::information(this,tr("退出"),
                                          widget->parentWidget()->windowTitle()+tr("已修改,是否在退出之前保存文件?"),
                                          QMessageBox::Cancel,QMessageBox::No,QMessageBox::Yes);
        if(button==QMessageBox::Yes)on_actionSave_triggered();
        else if(button!=QMessageBox::No)
        {
            return false;
        }
    }
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool flag=true;
    for(auto &dock:docks)
    {
        VectorgraphWidget *widget=qobject_cast<VectorgraphWidget*>(getPaintWidget(dock));
        if(!closeWidget(widget))flag=false;
    }
    if(!flag)
    {
        event->ignore();
    }
}

bool MainWindow::eventFilter(QObject * obj, QEvent * ev)
{
    switch(ev->type())
    {
    case QEvent::FocusIn:
    {
        QDockWidget *dock=qobject_cast<QDockWidget*>(obj);
        if(focus!=nullptr)
        {
            PaintWidget *widget=getPaintWidget(focus);
            Q_UNUSED(widget)//But it is really used...
#define KDISCONNECT(action) disconnect(ui->action,SIGNAL(triggered()),widget,SLOT(on_##action##_triggered()))
            KDISCONNECT(actionZoomIn);
            KDISCONNECT(actionZoomOut);
            KDISCONNECT(actionResume);
            KDISCONNECT(actionSave);
            KDISCONNECT(actionSaveAs);
            KDISCONNECT(actionCanvasSize);
            KDISCONNECT(actionShapeSize);
            KDISCONNECT(actionBreakUp);
            KDISCONNECT(actionRedo);
            KDISCONNECT(actionUndo);
            KDISCONNECT(actionDelete);
            KDISCONNECT(actionCopy);
            KDISCONNECT(actionCut);
            KDISCONNECT(actionPaste);
            KDISCONNECT(actionGroup);
#undef KDISCONNECT
        }
        if(dock!=nullptr)
        {
            PaintWidget *widget=getPaintWidget(dock);
            Q_UNUSED(widget)//But it is really used...
#define KCONNECT(action) connect(ui->action,SIGNAL(triggered()),widget,SLOT(on_##action##_triggered()))
            KCONNECT(actionZoomIn);
            KCONNECT(actionZoomOut);
            KCONNECT(actionResume);
            KCONNECT(actionSave);
            KCONNECT(actionSaveAs);
            KCONNECT(actionCanvasSize);
            KCONNECT(actionShapeSize);
            KCONNECT(actionBreakUp);
            KCONNECT(actionRedo);
            KCONNECT(actionUndo);
            KCONNECT(actionDelete);
            KCONNECT(actionCopy);
            KCONNECT(actionCut);
            KCONNECT(actionPaste);
            KCONNECT(actionGroup);
#undef KCONNECT

            focus = dock;
            VectorgraphWidget *vectorgraphWidget=qobject_cast<VectorgraphWidget *>(widget);
            if(vectorgraphWidget!=nullptr)
            {
                listView->setModel(vectorgraphWidget->listModel);
                listView->setSelectionModel(vectorgraphWidget->selectionModel);
                vectorgraphWidget->updateList();
            }
            return false;
        }
    }
        break;
    case QEvent::Close:
    {
        auto it=std::find(docks.begin(),docks.end(),obj);
        if(it!=docks.end())
        {
            VectorgraphWidget *widget=qobject_cast<VectorgraphWidget *>(getPaintWidget(*it));
            if(!closeWidget(widget))
            {
                ev->ignore();
                return true;
            }
            (*it)->deleteLater();
            docks.erase(it);
            if(!docks.empty())
            {
                this->focusDock(docks.back());
            }
            else if(ui->shapesDock->isVisible())ui->shapesDock->hide();
            focus = nullptr;
            return true;
        }
    }
        break;
    case QEvent::ContextMenu:
    {
        if(cursorState != VCursorType::DRAWBEZIERCURVE && cursorState != VCursorType::DRAWPOLYLINE)
        {
            QContextMenuEvent *event=static_cast<QContextMenuEvent*>(ev);
            VectorgraphWidget *widget=qobject_cast<VectorgraphWidget*>(obj);
            QPoint qpoint=event->pos();
            VPoint point(qpoint.x(),qpoint.y());
            VPoint loc=widget->getLoc(point);
            if(widget!=nullptr)
            {
                changeMenuAction(widget,loc);
                contextMenu->exec(event->globalPos());
                return true;
            }
        }
    }
        break;
    default:
        break;
    }

    return false;
}


void MainWindow::focusDock(bool checked)
{
    if(checked)
    {
        focusDock(qobject_cast<QDockWidget*>(sender()));
    }
}

void MainWindow::focusDock(QDockWidget * target)
{
    if(target==nullptr)return;
    target->raise();
    target->setFocus();
    qApp->postEvent(target, new QFocusEvent(QEvent::FocusIn));
//    qDebug() << "focusing";
//    focus = target;
}

void MainWindow::on_actionCanvasSize_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionShapeSize_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

PaintWidget * MainWindow::getPaintWidget()
{
    if(focus==nullptr)return nullptr;
    return qobject_cast<PaintWidget *>(focus->widget());
}

PaintWidget * MainWindow::getPaintWidget(QDockWidget *target)
{
    return qobject_cast<PaintWidget *>(target->widget());
}

void MainWindow::on_actionBreakUp_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionRotate_triggered()
{

    if(ui->actionRotate->isChecked())
        emit cursorChange(VCursorType::ROTATE);
    else
        emit cursorChange(VCursorType::DEFAULT);

}

void MainWindow::on_actionRedo_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionReloadPlugon_triggered()
{
    for(auto &i:ui->shapeBar->actions())
    {
        delete i;
    }
    for(auto &i:plugins)delete i;
    plugins.clear();
    this->initAction();
}

void MainWindow::on_actionLoadExPlugin_triggered()
{
    QStringList filenames=
            QFileDialog::getOpenFileNames(this,
                                         tr("打开文件"),
                                         tr(""),
                                         tr("json file (*.json);;"
                                            "all (*)"));
    for(auto &filename:filenames)loadPlugin(filename);
}

void MainWindow::on_actionAntialiasing_toggled(bool antialiasing)
{
    Q_UNUSED(antialiasing);
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionDelete_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionClose_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionCopy_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionCut_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionPaste_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionGroup_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionSelectAll_triggered()
{
    VectorgraphWidget *widget=qobject_cast<VectorgraphWidget *>(getPaintWidget());
    if(widget==nullptr)return;
    widget->focusShapes.clear();
    for(auto &i:widget->groupShape.getShapes())widget->focusShapes.append(i);
    widget->update();
}

void MainWindow::on_actionBrush_triggered()
{
    VectorgraphWidget *widget=qobject_cast<VectorgraphWidget *>(getPaintWidget());
    if(widget==nullptr)return;
    if(widget->focusShapes.empty())return;
    QColorDialog dialog(widget->focusShapes.first()->getBrush().color(),this);
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    if(dialog.exec()==QDialog::Accepted)
    {
        for(VShape *shape:widget->focusShapes)shape->setBrush(dialog.selectedColor());
        widget->update();
        widget->saveSwp();
    }
}

void MainWindow::on_actionPen_triggered()
{
    VectorgraphWidget *widget=qobject_cast<VectorgraphWidget *>(getPaintWidget());
    if(widget==nullptr)return;
    if(widget->focusShapes.empty())return;
    QColorDialog dialog(widget->focusShapes.first()->getPen().color(),this);
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    if(dialog.exec()==QDialog::Accepted)
    {
        for(VShape *shape:widget->focusShapes)shape->setPen(dialog.selectedColor());
        widget->update();
        widget->saveSwp();
    }
}

void MainWindow::on_actionPenStyle_triggered()
{
    VectorgraphWidget *widget=qobject_cast<VectorgraphWidget *>(getPaintWidget());
    if(widget==nullptr)return;
    if(widget->focusShapes.empty())return;
    PenStyleDialog::showDialog(tr("线条设置"),widget->focusShapes);
    widget->update();
    widget->saveSwp();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,tr("关于VIPainter")
                             ,tr("VIPainter是一款轻量的矢量图绘图软件\n"
                                 "\n"
                                 "版权所有(C) 2017 VIPainter\n"
                                 "本程序为自由软件；您可依据自由软件基金会所发表的GNU通用公共授权条款，对本程序再次发布和/或修改；无论您依据的是本授权的第三版，或（您可选的）任一日后发行的版本。\n"
                                 "本程序是基于使用目的而加以发布，然而不负任何担保责任；亦无对适售性或特定目的适用性所为的默示性担保。详情请参照GNU通用公共授权。\n"
                                 "您应已收到附随于本程序的GNU通用公共授权的副本；如果没有，请参照\n"
                                 "<http://www.gnu.org/licenses/>.\n"));
}

void MainWindow::on_actionDraw_triggered()
{
    if(ui->actionDraw->isChecked())
    {
        if(ui->actionCurveLine->isChecked())
        {
            emit cursorChange(VCursorType::DRAWBEZIERCURVE);
        }
        else if(ui->actionPolyLine->isChecked())
        {
            emit cursorChange(VCursorType::DRAWPOLYLINE);
        }
    }

}

void MainWindow::on_actionUndo_triggered()
{
    //Do nothing
    //Edit in PaintWidget
}

void MainWindow::on_actionCurveLine_triggered()
{
    on_actionDraw_triggered();
}

void MainWindow::on_actionPolyLine_triggered()
{
    on_actionDraw_triggered();
}
