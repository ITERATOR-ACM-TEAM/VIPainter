#ifndef CANVASSIZEDIALOG_H
#define CANVASSIZEDIALOG_H

#include <QDialog>
#include "vsize.h"

namespace Ui {
class CanvasSizeDialog;
}

class CanvasSizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CanvasSizeDialog(QWidget *parent = 0);
    ~CanvasSizeDialog();
    static VSize showDialog(const QString title, const VSize &size);

private:
    Ui::CanvasSizeDialog *ui;
};

#endif // CANVASSIZEDIALOG_H
