/*
 * MoveFileExistsDialog.cpp
 *
 *  Created on: 25 March 2012
 *      Author: Max Foster
 */

#include "MoveFileExistsDialog.h"
#include "ui_MoveFileExistsDialog.h"

#include <QCloseEvent>

MoveFileExistsDialog::MoveFileExistsDialog(const QString &filename, const QString &existingFilename, QWidget *parent)
    : QDialog(parent), ui(new Ui::MoveFileExistsDialog)
{
    ui->setupUi(this);
    ui->label_messageE->setText("The file '" + filename + "' cannot be moved because the file '" + existingFilename
                                + "' already exists. Would you like to...");
}

MoveFileExistsDialog::~MoveFileExistsDialog()
{
    delete ui;
}

void MoveFileExistsDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

void MoveFileExistsDialog::on_pushButton_moveAndReplace_clicked()
{
    done(MOVE_AND_REPLACE);
}

void MoveFileExistsDialog::on_pushButton_deleteAndKeep_clicked()
{
    done(DELETE_AND_KEEP);
}

void MoveFileExistsDialog::on_pushButton_doNothing_clicked()
{
    done(DO_NOTHING);
}
