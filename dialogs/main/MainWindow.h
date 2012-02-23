#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget * parent = NULL);
    ~MainWindow();

private slots:
    void on_pushButton_addNewJob_released();

    void on_pushButton_viewAllExpenses_released();

private:
    Ui::MainWindow * ui;
};

#endif // MAINWINDOW_H
