/*
 * CustomerIndex.h
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#ifndef CUSTOMERINDEX_H
#define CUSTOMERINDEX_H

#include <QDialog>

namespace Ui
{
    class CustomerIndex;
}

class CustomerIndex : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerIndex(QWidget *parent = NULL);
    ~CustomerIndex();

private:
    Ui::CustomerIndex *ui;
};

#endif // CUSTOMERINDEX_H
