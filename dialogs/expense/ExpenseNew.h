#ifndef EXPENSENEW_H
#define EXPENSENEW_H

#include <QDialog>

namespace Ui
{
    class ExpenseNew;
}

class ExpenseNew : public QDialog
{
    Q_OBJECT

public:
    explicit ExpenseNew(QWidget * parent = NULL);
    ~ExpenseNew();

private:
    Ui::ExpenseNew * ui;
};

#endif // EXPENSENEW_H
