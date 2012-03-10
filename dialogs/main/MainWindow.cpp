#include <QCloseEvent>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "EmailerThread.h"

#include "JobController.h"
#include "ExpenseController.h"
#include "CustomerController.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    EmailerThread::init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!EmailerThread::finalise()) event->ignore();
}

void MainWindow::on_pushButton_addNewJob_released()
{
    JobController::Index(this);
}

void MainWindow::on_pushButton_viewAllExpenses_released()
{
    ExpenseController::Index(this);
}

void MainWindow::on_pushButton_released()
{
    CustomerController::Index(this);
}
