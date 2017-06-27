#include "vellipse.h"
#include "cmath"
#include <QPainter>

VEllipse::VEllipse()
{
}

VEllipse::~VEllipse()
{
}

QImage VEllipse::toImage()
{
    QImage image(size.x,size.y,QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.drawEllipse(0,0,size.x,size.y);
}

void VEllipse::setSize(const VSize &size)
{
    this->size=size;
}

VSize VEllipse::getSize() const
{
    return size;
}
