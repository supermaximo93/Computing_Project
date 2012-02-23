/*
 * CustomerShow.h
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#ifndef CUSTOMERSHOW_H
#define CUSTOMERSHOW_H

#include <QDialog>

namespace Ui
{
    class CustomerShow;
}

class CustomerShow : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerShow(QWidget * parent = NULL);
    ~CustomerShow();

private:
    Ui::CustomerShow * ui;
};

#endif // CUSTOMERSHOW_H
