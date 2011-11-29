#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Globals.h"
#include "CustomerDialog.h"
#include "Customer.h"
#include "JobDialog.h"
#include "Job.h"

Database<Customer> * MainWindow::customers;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    customers = new Database<Customer>;
}

MainWindow::~MainWindow() {
    delete customers;
    delete ui;
}

Database<Customer> * MainWindow::customerDatabase() {
    return customers;
}

void MainWindow::on_pushButton_openCustomerScreen_clicked() {
    Customer customer = customers->findRecord(1);
    Message message;
    CustomerDialog customerDialog(&customer, &message, this);
    customerDialog.exec();
    customers->updateRecord(customer);

    if (message.message == "addNewJob") {
        JobDialog jobDialog(customer.getId(), this);
        jobDialog.exec();
    }
}
