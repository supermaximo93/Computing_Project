#include "JobNew.h"
#include "ui_JobNew.h"

JobNew::JobNew(QWidget * parent)
    : QDialog(parent), ui(new Ui::JobNew)
{
    ui->setupUi(this);
}

JobNew::~JobNew()
{
    delete ui;
}
