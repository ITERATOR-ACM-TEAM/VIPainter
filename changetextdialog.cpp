#include "changetextdialog.h"
#include "ui_changetextdialog.h"
#include <QPushButton>

ChangeTextDialog::ChangeTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeTextDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("取消"));
}

void ChangeTextDialog::showDialog(VText *vt){
    ChangeTextDialog dialog;
    dialog.exec();
    QString str =dialog.ui->textEdit->toPlainText();
    vt->setText(str);
    vt->calSize();
}

ChangeTextDialog::~ChangeTextDialog()
{
    delete ui;
}
