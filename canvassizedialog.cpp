#include "canvassizedialog.h"
#include "ui_canvassizedialog.h"

CanvasSizeDialog::CanvasSizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanvasSizeDialog)
{
    ui->setupUi(this);
}

CanvasSizeDialog::~CanvasSizeDialog()
{
    delete ui;
}


VSize CanvasSizeDialog::showDialog(const QString title,const VSize &size)
{
    CanvasSizeDialog dialog;
    dialog.setWindowTitle(title);
    dialog.ui->curHeight->setText(QString("%1").arg((int)size.height));
    dialog.ui->curWidth->setText(QString("%1").arg((int)size.width));
    dialog.ui->widthSpinBox->setValue((int)size.width);
    dialog.ui->heightSpinBox->setValue((int)size.height);
    int res=dialog.exec();
    if (res == QDialog::Accepted)
    {
        return VSize(dialog.ui->widthSpinBox->value(),dialog.ui->heightSpinBox->value());
    }
    return size;
}
