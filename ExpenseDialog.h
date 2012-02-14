#ifndef EXPENSEDIALOG_H
#define EXPENSEDIALOG_H

#include <QDialog>

namespace Ui
{
    class ExpenseDialog;
}

class ExpenseDialog : public QDialog
{
public:
    explicit ExpenseDialog(QWidget * parent = NULL);
    ~ExpenseDialog();

private:
    Q_OBJECT
    Ui::ExpenseDialog * ui;
};

#endif // EXPENSEDIALOG_H
