#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Database.h"
#include "Customer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    Ui::MainWindow *ui;
    static Database<Customer> * customers;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static Database<Customer> * customerDatabase();

private slots:
    void on_pushButton_openCustomerScreen_clicked();
};

#endif // MAINWINDOW_H
