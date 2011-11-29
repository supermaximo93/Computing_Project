#ifndef JOBDIALOG_H
#define JOBDIALOG_H

#include <QDialog>

namespace Ui {
    class JobDialog;
}

class JobDialog : public QDialog {
    Q_OBJECT

public:
    explicit JobDialog(QWidget *parent = 0);
    ~JobDialog();

private:
    Ui::JobDialog *ui;
};

#endif // JOBDIALOG_H
