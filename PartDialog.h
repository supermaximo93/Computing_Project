/*
 * PartDialog.h
 *
 *  Created on: 13 2 2012
 *      Author: Max Foster
 */

#ifndef PARTDIALOG_H
#define PARTDIALOG_H

#include <QDialog>

namespace Ui
{
    class PartDialog;
}

class PartDialog : public QDialog
{
public:
    explicit PartDialog(QWidget * parent = 0);
    ~PartDialog();

private:
    Q_OBJECT
    Ui::PartDialog * ui;
};

#endif // PARTDIALOG_H
