#include <QCloseEvent>
#include <QTimer>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "EmailerThread.h"

#include "JobController.h"
#include "ExpenseController.h"
#include "CustomerController.h"

const char *MainWindow::windowTitle = "Ian Foster Services";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), timer(new QTimer(this))
{
    ui->setupUi(this);
    setWindowTitle(windowTitle);
    EmailerThread::init(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(checkInternetConnection()));
    timer->start(EmailerThread::queueCheckTimePeriod);
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete timer;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!EmailerThread::finalise()) event->ignore();
}

void MainWindow::checkInternetConnection()
{
    setWindowTitle(EmailerThread::connectionAvailable() ? windowTitle : "Internet connection unavailable");
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
