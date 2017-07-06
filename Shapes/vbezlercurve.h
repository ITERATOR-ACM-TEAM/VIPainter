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
#ifndef VBEZLERSHAPE_H
#define VBEZLERSHAPE_H

#include "vshape.h"
#include "vpoint.h"
#include "vpointgroupshape.h"

class VBezlerCurve : public VPointGroupShape{
public:
    VBezlerCurve();
    VBezlerCurve(const VBezlerCurve &shape);
    VBezlerCurve(const QJsonObject &jsonObject);
    ~VBezlerCurve()override;
    const VBezlerCurve& operator=(const VBezlerCurve &shape);
    const VBezlerCurve& operator=(const QJsonObject &jsonObject);
    bool contains(VPoint point) override;
    //QImage toImage()override;
    void draw(QPainter *painter,const VTransform &transform)override;
    VShape* clone()const override;
    QString type()const override;
    void drawCR(QPainter * painter, const VTransform &trans, double scale);
};

#endif // VBEZLERSHAPE_H
