/*
 * PartShow.h
 *
 *  Created on: 15 Feb 2012
 *      Author: Max Foster
 */

#ifndef PARTSHOW_H
#define PARTSHOW_H

#include <QDialog>

namespace Ui
{
    class PartShow;
}

class Part;

class PartShow : public QDialog
{
    Q_OBJECT

public:
    explicit PartShow(Part & part, QWidget * parent = NULL);
    ~PartShow();

    void updateView();

private slots:
    void on_pushButton_ok_released();

    void on_pushButton_edit_released();

    void on_pushButton_delete_released();

private:
    Ui::PartShow * ui;
    Part & part;
};

#endif // PARTSHOW_H
