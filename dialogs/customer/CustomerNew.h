#ifndef CUSTOMERNEW_H
#define CUSTOMERNEW_H

#include <QDialog>

namespace Ui
{
    class CustomerNew;
}

class CustomerNew : public QDialog
{
public:
    explicit CustomerNew(QWidget * parent = NULL);
    ~CustomerNew();

private:
    Q_OBJECT
    Ui::CustomerNew * ui;
};

#endif // CUSTOMERNEW_H
