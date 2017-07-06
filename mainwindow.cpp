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
#include <QClipboard>
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
#include <QMimeData>
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
        TestWidget *widget=getTestWidget();
        if(widget!=nullptr)
        {
            changeMenuAction(widget,VPoint(0,0));
        }
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
        if(getTestWidget()==nullptr)return;
        VShape *newShape=shape->clone();
//        newShape->setPen(this->pen);
//        newShape->setBrush(this->brush);
        getTestWidget()->groupShape.insertShape(newShape);
        getTestWidget()->updateList();
        getTestWidget()->update();
        getTestWidget()->saveSwp();
    });
}

void MainWindow::changeShapeName(const QModelIndex &index)
{
    TestWidget *widget=getTestWidget();
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

    TestWidget* widget=new TestWidget(this,ui->actionAntialiasing->isChecked());
    connect(this, SIGNAL(cursorChange(VCursorType)), widget, SLOT(changeCursor(VCursorType)));
    emit cursorChange(this->cursorState);
    widget->installEventFilter(this);

    QDockWidget *dockWidget=new QDockWidget;
    dockWidget->installEventFilter(widget);
    dockWidget->setWidget(widget);
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
    connect(ui->actionBreakUp,SIGNAL(triggered()),widget,SLOT(updateList()));
    connect(ui->actionCut,SIGNAL(triggered()),widget,SLOT(updateList()));
    connect(ui->actionPaste,SIGNAL(triggered()),widget,SLOT(updateList()));
    connect(ui->actionDelete,SIGNAL(triggered()),widget,SLOT(updateList()));
    connect(ui->actionGroup,SIGNAL(triggered()),widget,SLOT(updateList()));
    //connect(getTestWidget(dockWidget),SIGNAL(selected(const QModelIndex&)),listView,SLOT(setCurrentIndex(const QModelIndex &index)));

    if(dockname=="")dockWidget->setWindowTitle(QString("untitled %1").arg(id++));
    else dockWidget->setWindowTitle(dockname);
    docks.push_back(dockWidget);

    //ui->shapesDock->show();
    if(!ui->shapesDock->isVisible())
    {
        ui->shapesDock->show();
        splitDockWidget(dockWidget,ui->shapesDock,Qt::Horizontal);
    }
    widget->saveSwp();

    return dockWidget;
}

MainWindow::~MainWindow()
{
    for(auto &i:plugins)delete i;
    delete ui;
}

void MainWindow::on_actionZoomIn_triggered()
{
    if(getTestWidget()==nullptr)return ;
    getTestWidget()->scale*=1.1;
    //qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionZoomOut_triggered()
{
    if(getTestWidget()==nullptr)return ;
    getTestWidget()->scale/=1.1;
    //qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionResume_triggered()
{
    if(getTestWidget()==nullptr)return ;
    getTestWidget()->scale=1.0;
    getTestWidget()->canvasLocation=VPoint(0,0);
    //qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionSave_triggered()
{
    if(getTestWidget()==nullptr)return;
}

void MainWindow::saveFile(QString filename)
{
    if(filename=="")return;
    if(focus == nullptr) return;
    ui->statusBar->showMessage(tr("保存到文件 ")+filename);
    QString format=filename.split('.').back().toUpper();
    if(format==tr("JSON"))
    {
        QJsonDocument jsonDocument;
        QJsonObject obj;
        obj.insert("type",QString("canvas"));
        obj.insert("size",getTestWidget()->canvasSize);
        obj.insert("shapes",getTestWidget()->groupShape.toJsonArray());
        jsonDocument.setObject(obj);
        QFile file(filename);
        if(file.open(QFile::WriteOnly|QFile::Text))
        {
            file.write(jsonDocument.toJson());
            file.close();
            focus->setWindowTitle(filename.split("/").back());
            getTestWidget(focus)->setFileName(filename);
        }
        else QMessageBox::warning(this,tr("错误"),tr("保存文件")+filename+tr("失败"));
    }
    else if(format=="JPG"||format=="PNG"||format=="BMP")
    {
        QImage image(getTestWidget()->canvasSize.width,getTestWidget()->canvasSize.height,QImage::Format_ARGB32_Premultiplied);
        image.fill(0x00ffffff);
        QPainter painter(&image);
        if(ui->actionAntialiasing->isChecked())painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(getTestWidget()->canvasSize.width/2,getTestWidget()->canvasSize.height/2);
        getTestWidget()->groupShape.draw(&painter,getTestWidget()->groupShape.getTransform());
        //image.scaled(getTestWidget()->canvasSize.width,getTestWidget()->canvasSize.height).save(filename);
        if(!image.save(filename,format.toStdString().c_str(),100))QMessageBox::warning(this,tr("错误"),tr("保存文件")+filename+tr("失败"));
    }
    else QMessageBox::warning(this,tr("错误"),format+tr("不能识别的文件格式"));
}

void MainWindow::on_actionSaveAs_triggered()
{
    if(getTestWidget()==nullptr)return;
    QString filename=getTestWidget()->getFileName();
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
        if(doc.isObject()&&doc.object().value("type")==QString("canvas"))
        {
            getTestWidget(newWidget)->canvasSize=doc.object().value("size").toObject();
            doc.setArray(doc.object().value("shapes").toArray());
        }
        if(doc.isArray())getTestWidget(newWidget)->groupShape=doc.array();
        else if(doc.isObject())
        {
            getTestWidget(newWidget)->groupShape.insertShape(VShape::fromJsonObject(doc.object()));
        }
        getTestWidget(newWidget)->setFileName(filename);
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
    }

}

void MainWindow::on_actionNew_triggered()
{
    newDock();
}

//判断Action的显示状态
void MainWindow::changeMenuAction(TestWidget *widget, VPoint loc)
{
    if(cursorState == VCursorType::DRAWBEZIERCURVE || cursorState == VCursorType::DRAWPOLYLINE)return;
    bool flag=false;
    if(widget->focusShapes.empty())
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

bool MainWindow::closeWidget(TestWidget *widget)
{
    bool flag=false;
    QString filename=widget->getFileName();
    if(filename=="")flag=true;
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
            obj.insert("size",widget->canvasSize);
            obj.insert("shapes",widget->groupShape.toJsonArray());
            jsonDocument.setObject(obj);
            if(doc!=jsonDocument)flag=true;
        }
    }
    if(flag)
    {
        int button
                =QMessageBox::information(this,tr("退出"),
                                          tr("是否在退出之前保存文件?"),
                                          QMessageBox::Yes,QMessageBox::No,QMessageBox::Cancel);
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
        TestWidget *widget=getTestWidget(dock);
        if(!closeWidget(widget))flag=false;
    }
    if(!flag)
    {
        event->ignore();
    }
}

bool MainWindow::eventFilter(QObject * obj, QEvent * ev)
{

    if(ev->type() == QEvent::FocusIn)
    {
        QDockWidget *dock=qobject_cast<QDockWidget*>(obj);
        if(dock!=nullptr)
        {
            focus = dock;
            listView->setModel(getTestWidget(dock)->listModel);
            listView->setSelectionModel(getTestWidget(dock)->selectionModel);
            getTestWidget(dock)->updateList();
            return false;
        }
    }
    else if(ev->type() == QEvent::Close)
    {
        auto it=std::find(docks.begin(),docks.end(),obj);
        if(it!=docks.end())
        {
            TestWidget *widget=getTestWidget(*it);
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
    else if(ev->type() ==QEvent::ContextMenu)
    {
        if(cursorState != VCursorType::DRAWBEZIERCURVE && cursorState != VCursorType::DRAWPOLYLINE)
        {
            QContextMenuEvent *event=static_cast<QContextMenuEvent*>(ev);
            TestWidget *widget=qobject_cast<TestWidget*>(obj);
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
    TestWidget *widget=getTestWidget();
    if(widget==nullptr)return;
    widget->canvasSize=CanvasSizeDialog::showDialog(tr("画布大小"),widget->canvasSize);
}

void MainWindow::on_actionShapeSize_triggered()
{
    TestWidget *widget=getTestWidget();
    if(widget==nullptr)return;
    VSize size=
            widget->groupShape.getSize()*
            widget->groupShape.getTransform();
    VSize toSize=CanvasSizeDialog::showDialog(tr("图像大小"),size);
    VMagnification mag=toSize/size;
    for(auto &i:widget->groupShape.getShapes())
    {
        i->zoomin(mag);
    }
    widget->saveSwp();
}

TestWidget * MainWindow::getTestWidget()
{
    if(focus==nullptr)return nullptr;
    return qobject_cast<TestWidget *>(focus->widget());
}

TestWidget * MainWindow::getTestWidget(QDockWidget *target)
{
    return qobject_cast<TestWidget *>(target->widget());
}

void MainWindow::on_actionBreakUp_triggered()
{
    TestWidget *widget=getTestWidget();
    if(focus == nullptr) return;
    if(widget->focusShapes.size()==1)
    {
        VGroupShape *shape=dynamic_cast<VGroupShape*>(widget->focusShapes.first());
        widget->focusShapes.clear();
        if(shape!=nullptr)
        {
            //qDebug()<<*(getTestWidget()->focusShape);
            QVector<VShape *> shs = VGroupShape::breakUp(shape);
            widget->groupShape.insertShape(shs);
            for(auto &i:shs)widget->focusShapes.append(i);
        }
        getTestWidget()->update();
        widget->saveSwp();
    }
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
    TestWidget *widget=getTestWidget();
    if(widget==nullptr)return;
    widget->redo();
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
    for(auto &i:this->docks)getTestWidget(i)->setAntialiasing(antialiasing);
}

void MainWindow::on_actionDelete_triggered()
{
    TestWidget *widget=getTestWidget();
    if(widget!=nullptr)
    {
        for(VShape *shape:widget->focusShapes)
        {
            widget->groupShape.eraseShape(shape);
        }
        widget->focusShapes.clear();
        widget->update();
        widget->saveSwp();
    }
}

void MainWindow::on_actionClose_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionCopy_triggered()
{
    TestWidget *widget=getTestWidget();
    if(widget!=nullptr)
    {
        if(widget->focusShapes.size()>0)
        {
            //  Get clipboard
            QClipboard *cb = QApplication::clipboard();

            // Ownership of the new data is transferred to the clipboard.
            QMimeData* newMimeData = new QMimeData();

            // Copy old mimedata
//            const QMimeData* oldMimeData = cb->mimeData();
//            for ( const QString &f : oldMimeData->formats())
//                newMimeData->setData(f, oldMimeData->data(f));

            // Copy file (gnome)
            ////////////////////////////////////////////////////////////////////////////////////////////////START
            ////////////////////////////////////JSON
            QJsonDocument doc;
            if(widget->focusShapes.size()>1)
            {
                QJsonArray arr;
                for(VShape *shape:widget->focusShapes)
                    arr.append(shape->toJsonObject());
                doc.setArray(arr);
            }
            else doc.setObject(widget->focusShapes.first()->toJsonObject());
            newMimeData->setData("application/x-JavaScript", doc.toBinaryData());

            // Copy file (gnome)
            ////////////////////////////////////TEXT

            VText *text=dynamic_cast<VText*>(widget->focusShapes.first());
            if(widget->focusShapes.size()==1&&text!=nullptr)
            {
                newMimeData->setText(text->getText());
            }
            else
            {

                ////////////////////////////////////IMAGE
                VGroupShape group;
                for(const VShape*shape:widget->groupShape.getShapes())
                {
                    if(std::find(widget->focusShapes.begin(),widget->focusShapes.end(),shape)!=widget->focusShapes.end())
                    {
                        group.insertShape(shape->clone());
                    }
                }
                group.getCircumscribedRectangle(true);
                VSize size=group.getSize()*group.getTransform();
                QImage image(size.width+4,size.height+4,QImage::Format_ARGB32);
                image.fill(0x00ffffff);
                QPainter painter(&image);
                if(ui->actionAntialiasing->isChecked())painter.setRenderHint(QPainter::Antialiasing);
                painter.translate(size.width/2+2,size.height/2+2);
                //qDebug()<<*(group.getShapeVector().back());
                group.draw(&painter,group.getTransform());
                newMimeData->setImageData(image);
            }

            //////////////////////////////////////////////////////////////////////////////////////////////////FI
            // Set the mimedata
            cb->setMimeData(newMimeData);
        }
    }
}

void MainWindow::on_actionCut_triggered()
{
    on_actionCopy_triggered();
    on_actionDelete_triggered();
}

void MainWindow::on_actionPaste_triggered()
{
    TestWidget *widget=getTestWidget();
    if(widget!=nullptr)
    {
        //  Get clipboard
        QClipboard *cb = QApplication::clipboard();
        const QMimeData* mimeData = cb->mimeData();
        if(mimeData->hasFormat("application/x-JavaScript"))
        {
//            qDebug()<<QJsonDocument::fromBinaryData(mimeData->data("application/x-JavaScript"));
            QJsonDocument doc=QJsonDocument::fromBinaryData(mimeData->data("application/x-JavaScript"));
            if(doc.isObject())
            {
                VShape *shape=VShape::fromJsonObject(
                            doc.object()
                            );
                //qDebug()<<*shape;
                widget->groupShape.insertShape(shape);
                widget->focusShapes.clear();
                widget->focusShapes.append(shape);
                widget->update();
                widget->saveSwp();
            }
            else if(doc.isArray())
            {
                widget->focusShapes.clear();
                for(auto i:doc.array())
                {
                    VShape *shape=VShape::fromJsonObject(
                                i.toObject()
                                );
                    widget->groupShape.insertShape(shape);
                    widget->focusShapes.append(shape);
                }
                widget->update();
                widget->saveSwp();
            }
        }
        else if(mimeData->hasText())
        {
            VText *text=new VText(mimeData->text());
            text->setName(mimeData->text());
            widget->groupShape.insertShape(text);
            widget->update();
            widget->saveSwp();
        }
    }

}

void MainWindow::on_actionGroup_triggered()
{
    TestWidget *widget=getTestWidget();
    if(widget==nullptr)return;
    if(widget->focusShapes.empty())return;
    VGroupShape *group=new VGroupShape;
    QString name;
    for(VShape *shape:widget->focusShapes)
    {
        if(name!="")name.append(", ");
        name.append(shape->getName());
        group->insertShape(widget->groupShape.takeShape(shape));
    }
    widget->groupShape.insertShape(group);
    group->getCircumscribedRectangle();
    widget->focusShapes.clear();
    group->setName(name);
    widget->focusShapes.append(group);
    widget->update();
    widget->saveSwp();
}

void MainWindow::on_actionSelectAll_triggered()
{
    TestWidget *widget=getTestWidget();
    if(widget==nullptr)return;
    widget->focusShapes.clear();
    for(auto &i:widget->groupShape.getShapes())widget->focusShapes.append(i);
    widget->update();
}

void MainWindow::on_actionBrush_triggered()
{
    TestWidget *widget=getTestWidget();
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
    TestWidget *widget=getTestWidget();
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
    TestWidget *widget=getTestWidget();
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
    else
        emit cursorChange(VCursorType::DEFAULT);

}

void MainWindow::on_actionUndo_triggered()
{
    TestWidget *widget=getTestWidget();
    if(widget==nullptr)return;
    widget->undo();
}
