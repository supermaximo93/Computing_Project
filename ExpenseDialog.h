#ifndef EXPENSEDIALOG_H
#define EXPENSEDIALOG_H

#include <QDialog>

namespace Ui
{
    class ExpenseDialog;
}

class Expense;
struct Message;

class ExpenseDialog : public QDialog
{
public:
    explicit ExpenseDialog(Expense * expense, Message * message, QWidget * parent = NULL);
    ~ExpenseDialog();

private:
    void recalculateTotalPrice();

    Q_OBJECT
    Ui::ExpenseDialog * ui;

    Expense * expenseToEdit;
    Message * messageToEdit;

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_submit_clicked();
    void on_lineEdit_priceExclVat_textChanged(QString);
    void on_lineEdit_vat_textChanged(QString);
};

#endif // EXPENSEDIALOG_H
