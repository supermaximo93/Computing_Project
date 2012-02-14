#include "JobDialog.h"
#include "ui_JobDialog.h"

JobDialog::JobDialog(QWidget * parent)
    : QDialog(parent), ui(new Ui::JobDialog)
{
    ui->setupUi(this);
}

JobDialog::~JobDialog()
{
    delete ui;
}
