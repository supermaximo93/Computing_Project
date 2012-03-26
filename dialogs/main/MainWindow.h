#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Database.h"

namespace Ui
{
    class MainWindow;
}

class Job;
class Task;
class QTimer;
class QModelIndex;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void checkInternetConnection();

    void on_pushButton_allJobs_released();

    void on_pushButton_allExpenses_released();

    void on_pushButton_allCustomers_released();

    void on_calendar_selectionChanged();

    void on_calendar_currentPageChanged(int year, int month);

    void on_listWidget_jobs_doubleClicked(const QModelIndex &index);

    void on_listWidget_tasks_doubleClicked(const QModelIndex &index);

    void on_label_remindCustomers_linkActivated(const QString &link);

    void action_settings_triggered();

    void action_help_triggered();

    void action_exit_triggered();

    void on_pushButton_allVatRates_clicked();

    void on_pushButton_generateReport_clicked();

private:
    static const char *windowTitle;

    Ui::MainWindow *ui;
    QTimer *timer;

    Database<Job>::recordListPtr jobs, unpaidJobs;
    Database<Task>::recordListPtr tasks;

    bool unpaidJobsReminderClicked;

    void getJobsAndTasksForCurrentDate();
    void updateListWidgets();
    void updateEverythingOtherThanExpenses();

    void calculateIncome();
    void calculateExpenses();
    void calculateGrandTotal();

    void checkForUnpaidJobs();

    void updateFinancialMonthText();
};

#endif // MAINWINDOW_H
