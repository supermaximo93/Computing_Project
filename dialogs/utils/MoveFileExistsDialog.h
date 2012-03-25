/*
 * MoveFileExistsDialog.h
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#ifndef MOVEFILEEXISTSDIALOG_H
#define MOVEFILEEXISTSDIALOG_H

#include <QDialog>

namespace Ui
{
    class MoveFileExistsDialog;
}

class MoveFileExistsDialog : public QDialog
{
    Q_OBJECT

public:
    enum Choice
    {
        MOVE_AND_REPLACE = 0,
        DELETE_AND_KEEP,
        DO_NOTHING
    };

    explicit MoveFileExistsDialog(const QString &filename, const QString &existingFilename, QWidget *parent = NULL);
    ~MoveFileExistsDialog();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButton_moveAndReplace_clicked();

    void on_pushButton_deleteAndKeep_clicked();

    void on_pushButton_doNothing_clicked();

private:
    Ui::MoveFileExistsDialog *ui;
};

#endif // MOVEFILEEXISTSDIALOG_H
