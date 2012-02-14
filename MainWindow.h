#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget * parent = NULL);
    ~MainWindow();

private:
    Q_OBJECT
    Ui::MainWindow * ui;
};

#endif // MAINWINDOW_H
