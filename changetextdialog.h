#ifndef CHANGETEXTDIALOG_H
#define CHANGETEXTDIALOG_H

#include <QDialog>
#include "vtext.h"

namespace Ui {
class ChangeTextDialog;
}

class ChangeTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeTextDialog(QWidget *parent = 0);
    static void showDialog(VText *vt);
    ~ChangeTextDialog();

private:
    Ui::ChangeTextDialog *ui;
};

#endif // CHANGETEXTDIALOG_H
