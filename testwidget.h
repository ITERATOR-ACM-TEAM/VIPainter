#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QGraphicsView>

class TestWidget : public QGraphicsView
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = 0);
    ~TestWidget();

private:
};

#endif // TESTWIDGET_H
