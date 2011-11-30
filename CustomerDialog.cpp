#include "CustomerDialog.h"
#include "ui_CustomerDialog.h"

#include "Globals.h"
#include "Database.h"
#include "Customer.h"

void CustomerDialog::setCustomerDataFromForm() {
    QByteArray byteArray = ui->lineEdit_forename->text().toLocal8Bit();
    customerToEdit->setForename(byteArray.data());

    byteArray = ui->lineEdit_surname->text().toLocal8Bit();
    customerToEdit->setSurname(byteArray.data());

    byteArray = ui->lineEdit_addressLine1->text().toLocal8Bit();
    customerToEdit->setAddressLine1(byteArray.data());

    byteArray = ui->lineEdit_addressLine2->text().toLocal8Bit();
    customerToEdit->setAddressLine2(byteArray.data());

    byteArray = ui->lineEdit_town->text().toLocal8Bit();
    customerToEdit->setTown(byteArray.data());

    byteArray = ui->lineEdit_postcode->text().toLocal8Bit();
    customerToEdit->setPostcode(byteArray.data());

    byteArray = ui->lineEdit_homePhone->text().toLocal8Bit();
    customerToEdit->setHomePhoneNumber(byteArray.data());

    byteArray = ui->lineEdit_mobilePhone->text().toLocal8Bit();
    customerToEdit->setMobilePhoneNumber(byteArray.data());

    byteArray = ui->lineEdit_emailAddress->text().toLocal8Bit();
    customerToEdit->setEmailAddress(byteArray.data());
}

CustomerDialog::CustomerDialog(Customer * customer, Message * message, QWidget *parent) :
    QDialog(parent), ui(new Ui::CustomerDialog) {
    ui->setupUi(this);

    if (customer == NULL) return;
    customerToEdit = customer;
    messageToEdit = message;
    if (messageToEdit == NULL) ui->pushButton_newJob->setDisabled(true);

    bool disable = (customerToEdit->getId() > -1);
    foreach (QObject * object, ui->formLayoutWidget->children()) {
        QWidget * widget = qobject_cast<QWidget *>(object);
        if (widget != NULL) widget->setDisabled(disable);
    }

    if (disable) {
        ui->pushButton_submit->setText("Edit");
        ui->pushButton_cancel->setText("Close");

        ui->lineEdit_forename->setText(customer->getForename());
        ui->lineEdit_surname->setText(customer->getSurname());
        ui->lineEdit_addressLine1->setText(customer->getAddressLine1());
        ui->lineEdit_addressLine2->setText(customer->getAddressLine2());
        ui->lineEdit_town->setText(customer->getTown());
        ui->lineEdit_postcode->setText(customer->getPostcode());
        ui->lineEdit_homePhone->setText(customer->getHomePhoneNumber());
        ui->lineEdit_mobilePhone->setText(customer->getMobilePhoneNumber());
        ui->lineEdit_emailAddress->setText(customer->getEmailAddress());
    }
}

CustomerDialog::~CustomerDialog() {
    delete ui;
}

void CustomerDialog::on_pushButton_cancel_clicked() {
    messageToEdit->message = "cancelled";
    hide();
}

void CustomerDialog::on_pushButton_submit_clicked() {
    if (ui->pushButton_submit->text() == "Edit") {
        foreach (QObject * object, ui->formLayoutWidget->children()) {
            QWidget * widget = qobject_cast<QWidget *>(object);
            if (widget != NULL) widget->setDisabled(false);
        }
        ui->pushButton_submit->setText("Save");
        return;
    }

    if (customerToEdit == NULL) {
        hide();
        return;
    }

    setCustomerDataFromForm();

    if (ui->pushButton_submit->text() == "Save") {
        ui->pushButton_submit->setText("Edit");

        foreach (QObject * object, ui->formLayoutWidget->children()) {
            QWidget * widget = qobject_cast<QWidget *>(object);
            if (widget != NULL) widget->setDisabled(true);
        }
    } else hide();
}

void CustomerDialog::on_pushButton_newJob_clicked() {
    if (messageToEdit != NULL) {
        messageToEdit->message = "addNewJob";
        messageToEdit->data = customerToEdit->getId();
        setCustomerDataFromForm();
        hide();
    }
}
