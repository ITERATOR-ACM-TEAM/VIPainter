/**
 * Copyright (C) 2017 kkkeQAQ
 *               2017 Bcai0797
 *               2017 Penn000
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

#ifndef VLISTVIEW_H
#define VLISTVIEW_H

#include <QListView>
#include <QItemSelection>

class VListView : public QListView
{
    Q_OBJECT

public:
    VListView(QWidget *parent=nullptr);

protected slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)override;

signals:
    void selectedListChanged(QModelIndexList indexList);
};

#endif // VLISTVIEW_H
