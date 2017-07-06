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
#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWidget>
#include <QMainWindow>
#include <QRect>
#include <QDir>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QKeyEvent>
#include <QModelIndex>
#include <QStringListModel>
#include <QItemSelectionModel>
#include <QList>
#include "vgroupshape.h"
#include "vsize.h"
#include "vpoint.h"
#include "vcursortype.h"
#include "vshape.h"
#include "vpoint.h"

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    QPixmap VSizeAll = QPixmap("://icon/mover.png").scaled(30,30);
    QPixmap VRotate = QPixmap("://icon/undo.png").scaled(20,20);
    explicit TestWidget(QMainWindow *mainwindow, bool antialiasing);
    double scale=1;
    QMainWindow *mainwindow;
    VGroupShape groupShape;
    QVector<VShape*> focusShapes;
    VPoint canvasLocation;
    VSize canvasSize;
    QStringListModel *listModel;
    QItemSelectionModel *selectionModel;
    QMenu *contextMenu;
    VPoint getLoc(const VPoint & point);
    ~TestWidget();

protected:
    void paintEvent(QPaintEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void wheelEvent(QWheelEvent * event)override;
    bool eventFilter(QObject * obj, QEvent * ev)override;

private:
    VCursorType cursorType;
    VShape * getShape(const VPoint &point);
    QList<QJsonObject>swp;
    VPoint locMove;
    VPoint lastMove;
    VPoint locPress;
    double lastAngle;
    int crPos;
    bool antialiasing;
    void emitSelected();

public slots:
    void updateList();
    void changeFocus();
    void saveSwp();
    void changeCursor(VCursorType type);
    void setAntialiasing(bool antialiasing);

signals:
    void selected(const QItemSelection &list,QItemSelectionModel::SelectionFlags command);
};

#endif // TESTWIDGET_H
