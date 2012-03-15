#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class QTimer;

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

private:
    static const char *windowTitle;

    Ui::MainWindow *ui;
    QTimer *timer;
};

#endif // MAINWINDOW_H
