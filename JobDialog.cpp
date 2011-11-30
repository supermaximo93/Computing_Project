#include "JobDialog.h"
#include "ui_JobDialog.h"

#include "Globals.h"
#include "Customer.h"
#include "Job.h"
#include "MainWindow.h"

void JobDialog::setJobDataFromForm() {
    //QByteArray byteArray = ui->comboBox_customer->currentIndex();
    //customerToEdit->setForename(byteArray.data());
}

JobDialog::JobDialog(Job * job, Message * message, const int customerId, QWidget *parent) :
    QDialog(parent), ui(new Ui::JobDialog) {
    ui->setupUi(this);

    if (job == NULL) return;
    jobToEdit = job;
    messageToEdit = message;

    std::vector<Customer> * customers = MainWindow::customerDatabase()->allRecords();
    QStringList customerNames;
    customerNames.reserve(customers->size());
    int index;
    for (unsigned i = 0; i < customers->size(); ++i) {
        Customer & customer = customers->at(i);
       if (customerId == customer.getId()) index = i;
       customerNames.push_back(QString(customer.getForename())+" "+QString(customer.getSurname()));
    }
    delete customers;
    ui->comboBox_customer->addItems(customerNames);
    ui->comboBox_customer->setCurrentIndex(index);
}

JobDialog::~JobDialog() {
    delete ui;
}

void JobDialog::on_pushButton_submit_clicked() {

}

void JobDialog::on_pushButton_cancel_clicked() {
    messageToEdit->message = "cancelled";
    hide();
}
