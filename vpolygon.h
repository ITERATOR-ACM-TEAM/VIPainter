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
#ifndef VPOLYGON
#define VPOLYGON

#include "vshape.h"
#include "vpoint.h"
#include "vpolyline.h"
#include "vtext.h"

class VPolygon : public VPointGroupShape{
protected:
    VText *text;
public:
    VPolygon();
    VPolygon(const VPolygon &shape);
    VPolygon(const QJsonObject &jsonObject);
    ~VPolygon()override;
    const VPolygon& operator=(const VPolygon &shape);
    const VPolygon& operator=(const QJsonObject &jsonObject);
    bool contains(VPoint point) override;
    //QImage toImage()override;
    void draw(QPainter *painter,const VTransform &transform)override;
    VShape* clone()override;
    QString type()const override;
    VText* getText();
};

#endif //#ifndef VPOLYGON
