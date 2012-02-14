#ifndef EXPENSENEW_H
#define EXPENSENEW_H

#include <QDialog>

namespace Ui
{
    class ExpenseNew;
}

class ExpenseNew : public QDialog
{
public:
    explicit ExpenseNew(QWidget * parent = NULL);
    ~ExpenseNew();

private:
    Q_OBJECT
    Ui::ExpenseNew * ui;
};

#endif // EXPENSENEW_H
