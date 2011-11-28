#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "CustomerDialog.h"
#include "Customer.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    customers = new Database<Customer>;
}

MainWindow::~MainWindow() {
    delete customers;
    delete ui;
}

void MainWindow::on_pushButton_openCustomerScreen_clicked() {
    Customer customer = customers->findRecord(1);
    CustomerDialog customerDialog(&customer, this);
    customerDialog.exec();
    customers->updateRecord(customer);
}
