/*
 * TaskNew.h
 *
 *  Created on: 13 Feb 2012
 *      Author: Max Foster
 */

#ifndef TASKNEW_H
#define TASKNEW_H

#include <QDialog>

namespace Ui
{
    class TaskNew;
}

class TaskNew : public QDialog
{
public:
    explicit TaskNew(QWidget * parent = NULL);
    ~TaskNew();

private:
    Q_OBJECT
    Ui::TaskNew * ui;
};

#endif // TASKNEW_H
