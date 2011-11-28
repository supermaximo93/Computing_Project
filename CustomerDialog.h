#ifndef CUSTOMERDIALOG_H
#define CUSTOMERDIALOG_H

#include <QDialog>

namespace Ui {
    class CustomerDialog;
}

class CustomerDialog : public QDialog {
    Q_OBJECT

    Ui::CustomerDialog *ui;

public:
    explicit CustomerDialog(QWidget *parent = 0);
    ~CustomerDialog();
};

#endif // CUSTOMERDIALOG_H
