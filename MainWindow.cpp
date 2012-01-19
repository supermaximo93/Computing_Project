#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Globals.h"
#include "CustomerDialog.h"
#include "Customer.h"
#include "JobDialog.h"
#include "Job.h"
#include "ExpenseDialog.h"
#include "Expense.h"

AssignOncePointer< Database<Customer> > MainWindow::customers;
AssignOncePointer< Database<Job> > MainWindow::jobs;
AssignOncePointer< Database<Expense> > MainWindow::expenses;

Database<Customer> * MainWindow::customerDatabase()
{
    return customers;
}

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    customers = new Database<Customer>;
    jobs = new Database<Job>;
    expenses = new Database<Expense>;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_openCustomerScreen_clicked()
{
    Customer customer;
    Message message;

    while (true)
    {
        CustomerDialog customerDialog(&customer, &message, this);
        customerDialog.exec();

        if (message.message == "cancelled") break;
        customers->updateRecord(customer);

        if (message.message == "addNewJob")
        {
            Job job;
            JobDialog jobDialog(&job, &message, customer.getId(), this);
            jobDialog.exec();

            if (message.message != "cancelled") jobs->addRecord(job);
        }
    }
}

void MainWindow::on_pushButton_newBusinessExpense_clicked()
{
    Expense expense;
    Message message;
    ExpenseDialog expenseDialog(&expense, &message, this);
    expenseDialog.exec();

    if (message.message == "cancelled") return;
    expenses->addRecord(expense);
}
