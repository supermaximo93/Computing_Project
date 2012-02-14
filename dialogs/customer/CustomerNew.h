#ifndef CUSTOMERNEW_H
#define CUSTOMERNEW_H

#include <QDialog>

namespace Ui
{
    class CustomerNew;
}

class CustomerNew : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerNew(QWidget * parent = NULL);
    ~CustomerNew();

private:
    Ui::CustomerNew * ui;
};

#endif // CUSTOMERNEW_H
