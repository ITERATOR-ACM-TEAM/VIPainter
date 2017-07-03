#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vdocktitlebar.h"
#include "vtype.h"
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
#include "canvassizedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),focus(nullptr),cursorState(VCursorType::CHOOSE)
{
    ui->setupUi(this);
    delete takeCentralWidget();
    setDockNestingEnabled(true);
    setWindowTitle("VIPainter");
    group = new QActionGroup(this);
    group->addAction(ui->actionChoose);
    group->addAction(ui->actionMove);
    group->addAction(ui->actionUndo);
    connect(this, SIGNAL(cursorChange(VCursorType)), this, SLOT(changeCursor(VCursorType)));
    QTimer::singleShot(0,this,SLOT(initAction(QDir)));

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
    if(doc.isArray())
    {
        VGroupShape *groupshape=new VGroupShape(doc.array());
        groupshape->getCircumscribedRectangle(true);
        shape=groupshape;
    }
    else if(doc.isObject())shape=VShape::fromJsonObject(doc.object());
    else return;
    if(shape==nullptr)return;
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
    shape->draw(&painter,mag);

    action->setIcon(QIcon(pixmap));
    action->setToolTip(shape->getName());
    ui->shapeBar->addAction(action);

    connect(action,&QAction::triggered,[this,shape]{
        //qDebug()<<"add"<<*shape;
        if(getTestWidget()==nullptr)return;
        getTestWidget()->groupShape.insertShape(shape->clone());
        getTestWidget()->update();
    });
}

void MainWindow::initAction(QDir dir)
{
    dir.setFilter(QDir::Files|QDir::Readable);
    dir.setNameFilters(QStringList()<<"*.json");
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

    TestWidget* newWidget=new TestWidget(this,ui->actionAntialiasing->isChecked());
    connect(this, SIGNAL(cursorChange(VCursorType)), newWidget, SLOT(changeCursor(VCursorType)));
    emit cursorChange(this->cursorState);

    QDockWidget *dockWidget=new QDockWidget;
    dockWidget->installEventFilter(newWidget);
    dockWidget->setWidget(newWidget);
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
//    QAction *act=dockWidget->toggleViewAction();
//    connect(act, &QAction::toggled,
//            [dockWidget](bool checked){
//        qDebug()<<"toggled"<<checked;
//        dockWidget->raise();
//        dockWidget->setFocus();
//        dockWidget->activateWindow();
//    });

    if(dockname=="")dockWidget->setWindowTitle(QString("untitled %1").arg(id++));
    else dockWidget->setWindowTitle(dockname);
    docks.push_back(dockWidget);
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
    qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionZoomOut_triggered()
{
    if(getTestWidget()==nullptr)return ;
    getTestWidget()->scale/=1.1;
    qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionResume_triggered()
{
    if(getTestWidget()==nullptr)return ;
    getTestWidget()->scale=1.0;
    getTestWidget()->canvasLocation=VPoint(0,0);
    qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
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
    if(filename.split('.').back()==tr("json"))
    {
        QJsonDocument jsonDocument;
        jsonDocument.setArray(getTestWidget()->groupShape.toJsonArray());
        QFile file(filename);
        file.open(QFile::WriteOnly|QFile::Text);
        file.write(jsonDocument.toJson());
        file.close();
        focus->setWindowTitle(filename.split("/").back());
    }
    else
    {
        QImage image(getTestWidget()->canvasSize.width,getTestWidget()->canvasSize.height,QImage::Format_ARGB32);
        image.fill(0x00ffffff);
        QPainter painter(&image);
        if(ui->actionAntialiasing->isChecked())painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(getTestWidget()->canvasSize.width/2,getTestWidget()->canvasSize.height/2);
        getTestWidget()->groupShape.draw(&painter,getTestWidget()->groupShape.getMagnification());
        image.save(filename);
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    if(getTestWidget()==nullptr)return;
    QString filename=
            QFileDialog::getSaveFileName(this,
                                         tr("保存文件"),
                                         tr("image.json"),
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
        if(doc.isArray())getTestWidget(newWidget)->groupShape=doc.array();
        else if(doc.isObject())getTestWidget(newWidget)->groupShape.insertShape(VShape::fromJsonObject(doc.object()));
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
    cursorState = type;
}

void MainWindow::on_actionNew_triggered()
{
    newDock();
}

bool MainWindow::eventFilter(QObject * obj, QEvent * ev)
{
    for(auto it = docks.begin(); it != docks.end(); it++)
    {
        if(obj == *it)
        {
            if(ev->type() == QEvent::FocusIn)
            {
                qDebug() << "focusing"<<obj;
                focus = *it;
                return false;
            }else if(ev->type() == QEvent::Close)
            {
                qDebug() << "closing";
                delete *it;
                docks.erase(it);
                focus = nullptr;
                if(!docks.empty())
                {
                    this->focusDock(docks.back());
                }
                qDebug() << docks.size();
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
            widget->groupShape.getMagnification();
    VSize toSize=CanvasSizeDialog::showDialog(tr("图像大小"),size);
    VMagnification mag=toSize/size;
    for(auto &i:widget->groupShape.getShapeVector())
    {
        i->zoomin(mag);
        i->setLocation(i->getLocation()*mag);
    }
}

TestWidget * MainWindow::getTestWidget()
{
    if(focus==nullptr)return nullptr;
    return qobject_cast<TestWidget *>(focus->widget());
}

TestWidget * MainWindow::getTestWidget(QDockWidget * target)
{
    return qobject_cast<TestWidget *>(target->widget());
}

void MainWindow::on_actionBreakUp_triggered()
{
    if(focus == nullptr) return;
    if(getTestWidget()->focusShape == nullptr) return;
    if(getTestWidget()->focusShape->type() == VType::GroupShape)
    {
        //qDebug()<<*(getTestWidget()->focusShape);
        int cnt = 0;
        for(auto it: getTestWidget()->groupShape.getShapeVector() )
        {
            if(it == getTestWidget()->focusShape)
            {
                QVector<VShape *> shs = VGroupShape::breakUp(dynamic_cast<VGroupShape*>(it));
                getTestWidget()->groupShape.insertShape(shs);
                getTestWidget()->focusShape = nullptr;
                break;
            }
            cnt++;
        }
    }
    getTestWidget()->update();
}

void MainWindow::on_actionUndo_triggered()
{

    if(ui->actionUndo->isChecked())
        emit cursorChange(VCursorType::ROTATE);
    else
        emit cursorChange(VCursorType::DEFAULT);

}

void MainWindow::on_actionRedo_triggered()
{
//    if(focus == nullptr) return;
//    if(getTestWidget()->focusShape == nullptr) return;
//    double angle = getTestWidget()->focusShape->getAngle();
//    getTestWidget()->focusShape->setAngle(angle-10);
//    getTestWidget()->update();
//    emit cursorChange(VCursorType::ROTSATE);
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
        if(widget->focusShape!=nullptr)
        {
            widget->groupShape.eraseShape(widget->focusShape);
            widget->focusShape=nullptr;
            widget->update();
        }
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
        if(widget->focusShape!=nullptr)
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
            QJsonDocument doc;
            doc.setObject(widget->focusShape->toJsonObject());
            newMimeData->setData("application/x-JavaScript", doc.toBinaryData());
            VSize size=widget->focusShape->getSize()*widget->focusShape->getMagnification();
            QImage image(size.width+2,size.height+2,QImage::Format_ARGB32);
            image.fill(0x00ffffff);
            QPainter painter(&image);
            if(ui->actionAntialiasing->isChecked())painter.setRenderHint(QPainter::Antialiasing);
            painter.translate(size.width/2+1,size.height/2+1);
            widget->focusShape->draw(&painter,widget->focusShape->getMagnification());\
            newMimeData->setImageData(image);
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
            VShape *shape=VShape::fromJsonObject(
                        QJsonDocument::fromBinaryData(mimeData->data("application/x-JavaScript")).object()
                        );
            qDebug()<<*shape;
            widget->groupShape.insertShape(shape);
            widget->focusShape=shape;
            widget->update();
        }
    }

}
