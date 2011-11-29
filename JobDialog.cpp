#include "JobDialog.h"
#include "ui_JobDialog.h"

#include "Customer.h"
#include "MainWindow.h"

JobDialog::JobDialog(const int customerId, QWidget *parent) : QDialog(parent), ui(new Ui::JobDialog) {
    ui->setupUi(this);

    std::vector<Customer> * customers = MainWindow::customerDatabase()->allRecords();
    QStringList customerNames;
    customerNames.reserve(customers->size());
    for (unsigned i = 0; i < customers->size(); ++i) {
        Customer & customer = customers->at(i);
       if (customerId == customer.getId())
           customerNames.insert(
                       customerNames.begin(), QString(customer.getForename())+" "+QString(customer.getSurname()));
       else customerNames.push_back(QString(customer.getForename())+" "+QString(customer.getSurname()));
    }
    delete customers;
    ui->comboBox_customer->addItems(customerNames);
}

JobDialog::~JobDialog() {
    delete ui;
}
