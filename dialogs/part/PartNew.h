/*
 * PartDialog.h
 *
 *  Created on: 13 2 2012
 *      Author: Max Foster
 */

#ifndef PARTNEW_H
#define PARTNEW_H

#include <QDialog>

namespace Ui
{
    class PartNew;
}

class PartNew : public QDialog
{
    Q_OBJECT

public:
    explicit PartNew(QWidget * parent = NULL);
    ~PartNew();

private:
    Ui::PartNew * ui;
};

#endif // PARTNEW_H
