#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPaintEvent>
#include <QActionGroup>
#include <vcursortype.h>
#include "testwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void cursorChange(int type);

protected:
    bool eventFilter(QObject * obj, QEvent * ev)override;

private slots:
    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();

    void on_actionResume_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionOpen_triggered();

    void on_actionTestShape1_triggered();

    void on_actionMove_triggered();

    void on_actionChoose_triggered();

    void changeCursor(int type);

    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;

    TestWidget *focus;
    QVector<TestWidget *> widgetVector;

    QActionGroup  *group;
    int cursorState;

    TestWidget * newDock();
    void saveFile(QString filename);


};

#endif // MAINWINDOW_H
