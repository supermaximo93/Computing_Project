#ifndef JOBDIALOG_H
#define JOBDIALOG_H

#include <QDialog>

namespace Ui
{
    class JobDialog;
}

class JobDialog : public QDialog
{
public:
    explicit JobDialog(QWidget * parent = NULL);
    ~JobDialog();

private:
    Q_OBJECT
    Ui::JobDialog * ui;
};

#endif // JOBDIALOG_H
