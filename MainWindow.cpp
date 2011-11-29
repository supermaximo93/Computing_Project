#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Globals.h"
#include "CustomerDialog.h"
#include "Customer.h"
#include "JobDialog.h"
#include "Job.h"

AssignOncePointer< Database<Customer> > MainWindow::customers;
AssignOncePointer< Database<Job> > MainWindow::jobs;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    customers = new Database<Customer>;
    jobs = new Database<Job>;
}

MainWindow::~MainWindow() {
    delete ui;
}

Database<Customer> * MainWindow::customerDatabase() {
    return customers;
}

void MainWindow::on_pushButton_openCustomerScreen_clicked() {
    Customer customer = customers->findRecord(3);
    Message message;

    while (true) {
        CustomerDialog customerDialog(&customer, &message, this);
        customerDialog.exec();

        if (message.message == "cancelled") break;
        customers->updateRecord(customer);

        if (message.message == "addNewJob") {
            Job job;
            JobDialog jobDialog(&job, &message, customer.getId(), this);
            jobDialog.exec();

            if (message.message != "cancelled") jobs->addRecord(job);
        }
    }
}
