#include "ExpenseDialog.h"
#include "ui_ExpenseDialog.h"

#include "Globals.h"
#include "Utils.h"
#include "Expense.h"

ExpenseDialog::ExpenseDialog(Expense * expense, Message * message, QWidget * parent)
    : QDialog(parent), ui(new Ui::ExpenseDialog)
{
    ui->setupUi(this);

    if (expense == NULL) return;
    expenseToEdit = expense;
    messageToEdit = message;

    bool disable = (expenseToEdit->getId() > -1);
    foreach (QObject * object, ui->formLayoutWidget->children())
    {
        QWidget * widget = qobject_cast<QWidget *>(object);
        if (widget != NULL) widget->setDisabled(disable);
    }

    ui->lineEdit_vat->setText(toString(Globals::vatRate).c_str());
    ui->lineEdit_totalPrice->setDisabled(true);

    if (disable)
    {
        ui->pushButton_submit->setText("Edit");
        ui->pushButton_cancel->setText("Close");
    }
}

ExpenseDialog::~ExpenseDialog()
{
    delete ui;
}

void ExpenseDialog::recalculateTotalPrice()
{
    QByteArray byteArray = ui->lineEdit_priceExclVat->text().toLocal8Bit();
    double priceExclVat = strtod(byteArray.data(), NULL);
    byteArray = ui->lineEdit_vat->text().toLocal8Bit();
    double vat = strtod(byteArray.data(), NULL) / 100.0;
    ui->lineEdit_totalPrice->setText(toString(priceExclVat * (1.0 + vat)).c_str());
}

void ExpenseDialog::on_pushButton_cancel_clicked()
{
    messageToEdit->message = "cancelled";
    hide();
}

void ExpenseDialog::on_pushButton_submit_clicked()
{
    if (ui->pushButton_submit->text() == "Edit")
    {
        foreach (QObject * object, ui->formLayoutWidget->children())
        {
            QWidget * widget = qobject_cast<QWidget *>(object);
            if (widget != NULL) widget->setDisabled(false);
        }
        ui->pushButton_submit->setText("Save");
        return;
    }

    if (expenseToEdit == NULL)
    {
        hide();
        return;
    }

    QByteArray byteArray = ui->textEdit_description->toPlainText().toLocal8Bit();
    expenseToEdit->setDescription(byteArray.data());

    if (ui->pushButton_submit->text() == "Save")
    {
        ui->pushButton_submit->setText("Edit");

        foreach (QObject * object, ui->formLayoutWidget->children())
        {
            QWidget * widget = qobject_cast<QWidget *>(object);
            if (widget != NULL) widget->setDisabled(true);
        }
    }
    else hide();
}

void ExpenseDialog::on_lineEdit_priceExclVat_textChanged(QString)
{
    recalculateTotalPrice();
}

void ExpenseDialog::on_lineEdit_vat_textChanged(QString)
{
    recalculateTotalPrice();
}
