#include <QCloseEvent>
#include <QTimer>
#include <QDate>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Database.h"
#include "Databases.h"

#include "EmailerThread.h"
#include "EmailDetails.h"

#include "Job.h"
#include "Customer.h"
#include "Task.h"
#include "Part.h"
#include "Expense.h"

#include "JobController.h"
#include "TaskController.h"
#include "ExpenseController.h"
#include "CustomerController.h"

const char *MainWindow::windowTitle = "Ian Foster Services";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), timer(new QTimer(this)), unpaidJobsReminderClicked(false)
{
    ui->setupUi(this);
    setWindowTitle(windowTitle);
    EmailerThread::init(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(checkInternetConnection()));
    timer->start(EmailerThread::queueCheckTimePeriod);

    getJobsAndTasksForCurrentDate();
    updateListWidgets();

    calculateIncome();
    calculateExpenses();
    calculateGrandTotal();

    checkForUnpaidJobs();
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

void MainWindow::on_pushButton_allJobs_released()
{
    JobController::Index(this);
    updateEverythingOtherThanExpenses();
}

void MainWindow::on_pushButton_allExpenses_released()
{
    ExpenseController::Index(this);
    calculateExpenses();
    calculateGrandTotal();
}

void MainWindow::on_pushButton_allCustomers_released()
{
    CustomerController::Index(this);
    updateEverythingOtherThanExpenses();
}

void MainWindow::on_calendar_selectionChanged()
{
    getJobsAndTasksForCurrentDate();
    updateListWidgets();
}


struct DateBounds
{
    time_t lowerBound, upperBound;
    DateBounds(time_t lowerBound, time_t upperBound) : lowerBound(lowerBound), upperBound(upperBound) {}
};

template<typename T>
bool isRecordWithinDateBounds(const T &record, void *data)
{
    DateBounds *dateBounds = static_cast<DateBounds *>(data);
    return (record.getDate() >= dateBounds->lowerBound) && (record.getDate() <= dateBounds->upperBound);
}

template<typename T>
int compareRecordDates(const T &record1, const T &record2)
{
    const time_t date1 = record1.getDate(), date2 = record2.getDate();
    return date1 < date2 ? -1 : (date1 > date2 ? 1 : 0);
}

void MainWindow::getJobsAndTasksForCurrentDate()
{
    const QDate qdate = ui->calendar->selectedDate();
    const Date date(0, 0, qdate.day(), qdate.month(), qdate.year());
    DateBounds dateBounds(date, date + 86400);

    jobs = JobController::getAllJobs();
    Databases::jobs().keepRecords(*jobs, isRecordWithinDateBounds, &dateBounds);
    if (jobs->size() > 1) Databases::jobs().sortRecords(*jobs, 0, jobs->size() - 1, compareRecordDates);

    tasks = TaskController::getAllTasks();
    Databases::tasks().keepRecords(*tasks, isRecordWithinDateBounds, &dateBounds);
    if (tasks->size() > 1) Databases::tasks().sortRecords(*tasks, 0, tasks->size() - 1, compareRecordDates);
}


void MainWindow::updateListWidgets()
{
    const QDate qdate = ui->calendar->selectedDate();
    const Date date(0, 0, qdate.day(), qdate.month(), qdate.year());
    ui->label_selectedDate1->setText(date.toQStringWithoutTime());
    ui->label_selectedDate2->setText(date.toQStringWithoutTime());

    ui->listWidget_jobs->clear();
    ui->listWidget_tasks->clear();

    Customer customer;
    for (unsigned i = 0; i < jobs->size(); ++i)
    {
        customer = CustomerController::getCustomer(jobs->at(i).getCustomerId());
        ui->listWidget_jobs->addItem(createFullName(customer.getForename(), customer.getSurname()));
    }

    for (unsigned i = 0; i < tasks->size(); ++i)
        ui->listWidget_tasks->addItem(limitLength(tasks->at(i).getDescription(), 30));
}

void MainWindow::updateEverythingOtherThanExpenses()
{
    getJobsAndTasksForCurrentDate();
    updateListWidgets();
    calculateIncome();
    calculateGrandTotal();
    checkForUnpaidJobs();
}

void MainWindow::on_listWidget_jobs_doubleClicked(const QModelIndex &index)
{
    Job & job = jobs->at(index.row());
    JobController::Show(job, this);
    if (job.null()) jobs->erase(jobs->begin() + index.row());

    updateEverythingOtherThanExpenses();
}

void MainWindow::on_listWidget_tasks_doubleClicked(const QModelIndex &index)
{
    Task & task = tasks->at(index.row());
    Job job = JobController::getJob(task.getJobId());
    JobController::Show(job, this);

    Task possiblyUpdatedTask = TaskController::getTask(task.getId());
    if (!task.completeCompare(possiblyUpdatedTask))
    {
        if (possiblyUpdatedTask.null()) tasks->erase(tasks->begin() + index.row());
        else tasks->at(index.row()) = possiblyUpdatedTask;
    }

    updateEverythingOtherThanExpenses();
}

void MainWindow::on_label_remindCustomers_linkActivated(const QString &)
{
    static const char *emailSubject = "", *emailBody = "";

    unpaidJobsReminderClicked = true;
    ui->label_remindCustomers->setHidden(true);

    for (unsigned i = 0; i < unpaidJobs->size(); ++i)
    {
        Customer customer = CustomerController::getCustomer(unpaidJobs->at(i).getCustomerId());

        EmailDetails email(customer.getEmailAddress(), emailSubject, emailBody);
        EmailerThread::enqueueEmail(email);
    }
}

void MainWindow::calculateIncome()
{
    Database<Job>::recordListPtr thisYearsJobs = JobController::getAllJobs();
    Date now(time(NULL));
    const Date thisYearLowerBound(0, 0, 0, 0, now.year), thisYearUpperBound(time_t(Date(0, 0, 0, 0, now.year + 1)) - 1);
    DateBounds dateBounds(thisYearLowerBound, thisYearUpperBound);
    Databases::jobs().keepRecords(*thisYearsJobs, isRecordWithinDateBounds, &dateBounds);

    double income = 0.0, vat = 0.0;
    for (unsigned i = 0; i < thisYearsJobs->size(); ++i)
    {
        Job &job = thisYearsJobs->at(i);
        if (job.getCompletionState() == Job::DONE_PAID)
        {
            Database<Part>::recordListPtr parts = JobController::getJobParts(job.getId());
            for (unsigned j = 0; j < parts->size(); ++j)
            {
                Part &part = parts->at(j);
                income += part.getPrice();
                vat += part.getPrice() * (part.getVatRate() / 100.0);
            }

            income += job.getLabourCharge();
            vat += job.getVat();
        }
    }

    ui->doubleSpinBox_income->setValue(income);
    ui->doubleSpinBox_vat->setValue(vat);
    ui->doubleSpinBox_totalIncome->setValue(income + vat);
}

void MainWindow::calculateExpenses()
{
    Database<Expense>::recordListPtr expenses = ExpenseController::getAllExpenses();
    Date now(time(NULL));
    const Date thisYearLowerBound(0, 0, 0, 0, now.year), thisYearUpperBound(time_t(Date(0, 0, 0, 0, now.year + 1)) - 1);
    DateBounds dateBounds(thisYearLowerBound, thisYearUpperBound);
    Databases::expenses().keepRecords(*expenses, isRecordWithinDateBounds, &dateBounds);

    double expenseTotal = 0.0;
    for (unsigned i = 0; i < expenses->size(); ++i) expenseTotal += expenses->at(i).getTotalPrice();

    ui->doubleSpinBox_expenses->setValue(expenseTotal);
}

void MainWindow::calculateGrandTotal()
{
    ui->doubleSpinBox_grandTotal->setValue(
                ui->doubleSpinBox_totalIncome->value() - ui->doubleSpinBox_expenses->value());
}

void MainWindow::checkForUnpaidJobs()
{
    static const int numberOfDaysBeforeReminder = 7;

    if (unpaidJobsReminderClicked) {
        ui->label_remindCustomers->setHidden(true);
        return;
    }

    // Can't do nested functions in C++, but can have class definitions inside function
    struct NestedFunctions
    {
        static bool isJobDoneButUnpaid(const Job &job, void *)
        {
            return (job.getCompletionState() == Job::DONE_UNPAID);
        }
    };

    unpaidJobs = JobController::getAllJobs();
    DateBounds dateBounds(0, time(NULL) - (numberOfDaysBeforeReminder * 24 * 60 * 60));

    Databases::jobs().keepRecords(*unpaidJobs, isRecordWithinDateBounds, &dateBounds);
    Databases::jobs().keepRecords(*unpaidJobs, NestedFunctions::isJobDoneButUnpaid, NULL);

    ui->label_remindCustomers->setHidden(unpaidJobs->empty());

}
