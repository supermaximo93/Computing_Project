#ifndef CUSTOMERDIALOG_H
#define CUSTOMERDIALOG_H

#include <QDialog>

#include "Customer.h"

namespace Ui {
    class CustomerDialog;
}

class CustomerDialog : public QDialog {
    Q_OBJECT

    Ui::CustomerDialog *ui;
    Customer * customerToEdit;

public:
    explicit CustomerDialog(Customer * customer, QWidget *parent = 0);
    ~CustomerDialog();
private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_submit_clicked();
};

#endif // CUSTOMERDIALOG_H
