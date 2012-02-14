#include "ExpenseNew.h"
#include "ui_ExpenseNew.h"

ExpenseNew::ExpenseNew(QWidget * parent)
    : QDialog(parent), ui(new Ui::ExpenseNew)
{
    ui->setupUi(this);
}

ExpenseNew::~ExpenseNew()
{
    delete ui;
}
