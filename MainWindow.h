#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Globals.h"
#include "Database.h"

namespace Ui {
    class MainWindow;
}

class Customer;
class Job;

class MainWindow : public QMainWindow {
    Q_OBJECT
    Ui::MainWindow *ui;

    static AssignOncePointer< Database<Customer> > customers;
    static AssignOncePointer< Database<Job> > jobs;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static Database<Customer> * customerDatabase();

private slots:
    void on_pushButton_openCustomerScreen_clicked();
};

#endif // MAINWINDOW_H
