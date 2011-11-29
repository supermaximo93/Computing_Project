#ifndef CUSTOMERDIALOG_H
#define CUSTOMERDIALOG_H

#include <QDialog>

namespace Ui {
    class CustomerDialog;
}

class Customer;
struct Message;

class CustomerDialog : public QDialog {
    Q_OBJECT

    Ui::CustomerDialog *ui;
    Customer * customerToEdit;
    Message * messageToEdit;

    void setCustomerDataFromForm();

public:
    explicit CustomerDialog(Customer * customer, Message * message, QWidget *parent = 0);
    ~CustomerDialog();
private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_submit_clicked();
    void on_pushButton_newJob_clicked();
};

#endif // CUSTOMERDIALOG_H
