#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Globals.h"
#include "Database.h"

namespace Ui
{
    class MainWindow;
}

class Customer;
class Job;
class Expense;

class MainWindow : public QMainWindow
{
public:
    static Database<Customer> * customerDatabase();

    explicit MainWindow(QWidget * parent = NULL);
    ~MainWindow();

private:
    static AssignOncePointer< Database<Customer> > customers;
    static AssignOncePointer< Database<Job> > jobs;
    static AssignOncePointer< Database<Expense> > expenses;

    Q_OBJECT
    Ui::MainWindow * ui;

private slots:
    void on_pushButton_openCustomerScreen_clicked();
    void on_pushButton_newBusinessExpense_clicked();
};

#endif // MAINWINDOW_H
