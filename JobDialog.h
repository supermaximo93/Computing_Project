#ifndef JOBDIALOG_H
#define JOBDIALOG_H

#include <QDialog>

namespace Ui {
    class JobDialog;
}

class Job;
struct Message;

class JobDialog : public QDialog {
    Q_OBJECT
    Ui::JobDialog *ui;

    Job * jobToEdit;
    Message * messageToEdit;

    void setCustomerDataFromForm();

public:
    explicit JobDialog(Job * job, Message * message, const int customerId = -1, QWidget *parent = 0);
    ~JobDialog();

private slots:
    void on_pushButton_submit_clicked();
    void on_pushButton_cancel_clicked();
};

#endif // JOBDIALOG_H
