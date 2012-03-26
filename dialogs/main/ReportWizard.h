/*
 * ReportWizard.h
 *
 *  Created on: 26 March 2012
 *      Author: Max Foster
 */

#ifndef REPORTWIZARD_H
#define REPORTWIZARD_H

#include <QDialog>

namespace Ui
{
    class ReportWizard;
}

class ReportWizard : public QDialog
{
    Q_OBJECT

public:
    explicit ReportWizard(QWidget *parent = NULL);
    ~ReportWizard();

private slots:
    void on_pushButton_datePickerStartDate_clicked();

    void on_pushButton_datePickerEndDate_clicked();

    void on_pushButton_submit_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::ReportWizard *ui;
};

#endif // REPORTWIZARD_H
