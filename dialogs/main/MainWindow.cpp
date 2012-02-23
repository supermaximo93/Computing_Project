#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "JobController.h"
#include "ExpenseController.h"

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_addNewJob_released()
{
    JobController::Index(this);
}

void MainWindow::on_pushButton_viewAllExpenses_released()
{
    ExpenseController::Index(this);
}
