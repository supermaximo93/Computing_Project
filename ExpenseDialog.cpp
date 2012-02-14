#include "ExpenseDialog.h"
#include "ui_ExpenseDialog.h"

ExpenseDialog::ExpenseDialog(QWidget * parent)
    : QDialog(parent), ui(new Ui::ExpenseDialog)
{
    ui->setupUi(this);
}

ExpenseDialog::~ExpenseDialog()
{
    delete ui;
}
