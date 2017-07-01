#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vdocktitlebar.h"
#include "vtype.h"
#include <QJsonDocument>
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
#include <QIcon>
#include <QPixmap>
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
    initAction(QDir("plugin"));
    connect(this, SIGNAL(cursorChange(VCursorType)), this, SLOT(changeCursor(VCursorType)));
    update();
}


void MainWindow::initAction(QDir dir)
{
    dir.setFilter(QDir::Files|QDir::Readable);
    dir.setNameFilters(QStringList()<<"*.vp");
    QStringList files=dir.entryList();
    for(auto i:files)
    {
        QFile file(dir.filePath(i));
        if(!file.open(QFile::ReadOnly|QFile::Text))continue;
        VGroupShape *shape=new VGroupShape(QJsonDocument::fromJson(file.readAll()).array());
        shape->getCircumscribedRectangle(true);
        file.close();
        if(shape==nullptr)continue;
        qDebug()<<file.fileName()<<" loading";
        QAction *action=new QAction(ui->shapeBar);
        VSize size=shape->getSize();
        QPixmap pixmap(size.width,size.height);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.translate(size.width/2,size.height/2);
        shape->draw(&painter,shape->getMagnification());
        action->setIcon(QIcon(pixmap));
        action->setToolTip(shape->getName());
        ui->shapeBar->addAction(action);
    }

}

QDockWidget* MainWindow::newDock()
{
    QDockWidget *old = focus;

    static int id = 0;

    TestWidget* newWidget=new TestWidget(this);
    connect(this, SIGNAL(cursorChange(VCursorType)), newWidget, SLOT(changeCursor(VCursorType)));
    emit cursorChange(this->cursorState);

    QDockWidget *dockWidget=new QDockWidget;
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

//    connect(dockWidget->toggleViewAction(), &QAction::toggled,
//            [dockWidget](bool checked){
//        qDebug()<<"triggled"<<checked;
//        dockWidget->raise();
//        dockWidget->setFocus();
//        dockWidget->activateWindow();
//    });

    dockWidget->setWindowTitle(QString("untitled %1").arg(id++));
    docks.push_back(dockWidget);
    return dockWidget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionZoomIn_triggered()
{

    getTestWidget()->scale*=1.1;
    qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionZoomOut_triggered()
{
    getTestWidget()->scale/=1.1;
    qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionResume_triggered()
{
    getTestWidget()->scale=1.0;
    getTestWidget()->canvasLocation=VPoint(0,0);
    qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionSave_triggered()
{
}

void MainWindow::saveFile(QString filename)
{
    if(focus == nullptr) return;
    if(filename.split('.').back()==tr("vp"))
    {
        QJsonDocument jsonDocument;
        jsonDocument.setArray(getTestWidget()->groupShape.toJsonArray());
        QFile file(filename);
        file.open(QFile::WriteOnly|QFile::Text);
        file.write(jsonDocument.toJson());
        file.close();
    }
    else
    {
        QImage image(getTestWidget()->canvasSize.width,getTestWidget()->canvasSize.height,QImage::Format_ARGB32);
        image.fill(0x00ffffff);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(getTestWidget()->canvasSize.width/2,getTestWidget()->canvasSize.height/2);
        getTestWidget()->groupShape.draw(&painter,getTestWidget()->groupShape.getMagnification());
        image.save(filename);
    }
    focus->setWindowTitle(filename.split("/").back());
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename=
            QFileDialog::getSaveFileName(this,
                                         tr("Save File"),
                                         tr("image.vp"),
                                         tr("vp file (*.vp);;"
                                            "png file (*.png);;"
                                            "jpg file (*.jpg);;"
                                            "bmp file (*.bmp)"));
    saveFile(filename);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename=
            QFileDialog::getOpenFileName(this,
                                         tr("Open File"),
                                         tr(""),
                                         tr("vp file (*.vp)"));

    QFile file(filename);
    file.open(QFile::ReadOnly|QFile::Text);
    QDockWidget * newWidget = newDock();

    getTestWidget(newWidget)->groupShape=QJsonDocument::fromJson(file.readAll()).array();
    newWidget->setWindowTitle(filename.split("/").back());
    file.close();
    newWidget->update();
}

void MainWindow::on_actionTestShape1_triggered()
{
    QString filename = "plugin/testShape1.vp";
    QFile file(filename);
    file.open(QFile::ReadOnly|QFile::Text);
    VGroupShape * gs= new VGroupShape(QJsonDocument::fromJson(file.readAll()).object());
    if(focus==nullptr)return;
    qDebug() << focus ;
    getTestWidget()->groupShape.insertShape(gs);
    file.close();
    getTestWidget()->update();
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
                qDebug() << "focusing";
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


void MainWindow::focusDock(QDockWidget * target)
{
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
        int cnt = 0;
        for(auto it: getTestWidget()->groupShape.getShapeVector() )
        {
            if(it == getTestWidget()->focusShape)
            {
                QVector<VShape *> shs = VGroupShape::breakUp(dynamic_cast<VGroupShape*>(it));
                getTestWidget()->groupShape.eraseShape(cnt);
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
    if(focus == nullptr) return;
    if(getTestWidget()->focusShape == nullptr) return;
    double angle = getTestWidget()->focusShape->getAngle();
    getTestWidget()->focusShape->setAngle(angle+10);
    getTestWidget()->update();
}

void MainWindow::on_actionRedo_triggered()
{
    if(focus == nullptr) return;
    if(getTestWidget()->focusShape == nullptr) return;
    double angle = getTestWidget()->focusShape->getAngle();
    getTestWidget()->focusShape->setAngle(angle-10);
    getTestWidget()->update();
}
