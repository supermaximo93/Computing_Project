#ifndef CUSTOMERDIALOG_H
#define CUSTOMERDIALOG_H

#include <QDialog>

namespace Ui
{
    class CustomerDialog;
}

class CustomerDialog : public QDialog
{
public:
    explicit CustomerDialog(QWidget * parent = NULL);
    ~CustomerDialog();

private:
    Q_OBJECT
    Ui::CustomerDialog * ui;
};

#endif // CUSTOMERDIALOG_H
