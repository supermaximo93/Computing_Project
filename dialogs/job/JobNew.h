#ifndef JOBNEW_H
#define JOBNEW_H

#include <QDialog>

namespace Ui
{
    class JobNew;
}

class JobNew : public QDialog
{
public:
    explicit JobNew(QWidget * parent = NULL);
    ~JobNew();

private:
    Q_OBJECT
    Ui::JobNew * ui;
};

#endif // JOBNEW_H
