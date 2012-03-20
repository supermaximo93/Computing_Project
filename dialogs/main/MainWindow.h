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

    void on_pushButton_addNewJob_released();

    void on_pushButton_viewAllExpenses_released();

    void on_pushButton_released();

    void on_calendar_selectionChanged();

    void on_listWidget_jobs_doubleClicked(const QModelIndex &index);

    void on_listWidget_tasks_doubleClicked(const QModelIndex &index);

private:
    static const char *windowTitle;

    Ui::MainWindow *ui;
    QTimer *timer;

    Database<Job>::recordListPtr jobs;
    Database<Task>::recordListPtr tasks;

    void getJobsAndTasksForCurrentDate();
    void updateListWidgets();
};

#endif // MAINWINDOW_H
