#include "mainwindow.h"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),focus(nullptr)
{
    ui->setupUi(this);
    delete takeCentralWidget();
    setDockNestingEnabled(true);
    setWindowTitle("VIPainter");
    group = new QActionGroup(this);
    group->addAction(ui->actionChoose);
    group->addAction(ui->actionMove);
    connect(this, SIGNAL(cursorChange(int)), this, SLOT(changeCursor(int)));
    update();
}

QDockWidget* MainWindow::newDock()
{
    QDockWidget *old = focus;

    static int id = 0;

    TestWidget* newWidget=new TestWidget(this);
    connect(this, SIGNAL(cursorChange(int)), newWidget, SLOT(changeCursor(int)));
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

    qobject_cast<TestWidget *>(focus->widget())->scale*=1.1;
    qDebug()<<"scale:"<<qobject_cast<TestWidget *>(focus->widget())->scale<<endl;
    qobject_cast<TestWidget *>(focus->widget())->update();
}

void MainWindow::on_actionZoomOut_triggered()
{
    qobject_cast<TestWidget *>(focus->widget())->scale/=1.1;
    qDebug()<<"scale:"<<qobject_cast<TestWidget *>(focus->widget())->scale<<endl;
    qobject_cast<TestWidget *>(focus->widget())->update();
}

void MainWindow::on_actionResume_triggered()
{
    qobject_cast<TestWidget *>(focus->widget())->scale=1.0;
    qobject_cast<TestWidget *>(focus->widget())->canvasLocation=VPoint(0,0);
    qDebug()<<"scale:"<<qobject_cast<TestWidget *>(focus->widget())->scale<<endl;
    qobject_cast<TestWidget *>(focus->widget())->update();
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
        jsonDocument.setObject(qobject_cast<TestWidget *>(focus->widget())->groupShape.toJsonObject());
        QFile file(filename);
        file.open(QFile::WriteOnly|QFile::Text);
        file.write(jsonDocument.toJson());
        file.close();
    }
    else
    {
        QImage image(qobject_cast<TestWidget *>(focus->widget())->canvasSize.width,qobject_cast<TestWidget *>(focus->widget())->canvasSize.height,QImage::Format_ARGB32);
        image.fill(0x00ffffff);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(qobject_cast<TestWidget *>(focus->widget())->canvasSize.width/2,qobject_cast<TestWidget *>(focus->widget())->canvasSize.height/2);
        qobject_cast<TestWidget *>(focus->widget())->groupShape.draw(&painter,VMagnification(1,1));
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
    qobject_cast<TestWidget *>(newWidget->widget())->groupShape=QJsonDocument::fromJson(file.readAll()).object();
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
    qobject_cast<TestWidget *>(focus->widget())->groupShape.insertShape(gs);
    file.close();
    qobject_cast<TestWidget *>(focus->widget())->update();
}

void MainWindow::on_actionMove_triggered()
{
    if(ui->actionMove->isChecked())
        emit cursorChange(VCursorType::MOVE);
    else
        emit cursorChange(VCursorType::CHOOSE);
}

void MainWindow::on_actionChoose_triggered()
{
    if(ui->actionChoose->isChecked())
        emit cursorChange(VCursorType::CHOOSE);
    else
        emit cursorChange(VCursorType::CHOOSE);
}

void MainWindow::changeCursor(int type)
{
    cursorState = type;
}

void MainWindow::on_actionNew_triggered()
{
    QDockWidget * newWidget = newDock();
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
