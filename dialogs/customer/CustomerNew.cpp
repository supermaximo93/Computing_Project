#include "CustomerNew.h"
#include "ui_CustomerNew.h"

CustomerNew::CustomerNew(QWidget * parent)
    : QDialog(parent), ui(new Ui::CustomerNew)
{
    ui->setupUi(this);
}

CustomerNew::~CustomerNew()
{
    delete ui;
}
