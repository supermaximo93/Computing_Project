#include "CustomerDialog.h"
#include "ui_CustomerDialog.h"

CustomerDialog::CustomerDialog(QWidget * parent)
    : QDialog(parent), ui(new Ui::CustomerDialog)
{
    ui->setupUi(this);
}

CustomerDialog::~CustomerDialog()
{
    delete ui;
}
