/*
 * CustomerForm.cpp
 *
 *  Created on: 23 Feb 2012
 *      Author: Max Foster
 */

#include "CustomerForm.h"
#include "ui_CustomerForm.h"

#include "Customer.h"
#include "CustomerController.h"

CustomerForm::CustomerForm(Customer & customer, QWidget *parent)
    : QDialog(parent), formType(customer.null() ? NEW : EDIT), ui(new Ui::CustomerForm), customer(customer)
{
    ui->setupUi(this);
    ui->label_customerFormTitle->setText(formType == NEW ? "New customer" : "Editing customer");

    updateView();
}

CustomerForm::~CustomerForm()
{
    delete ui;
}

void CustomerForm::updateView()
{
    ui->lineEdit_forename->setText(customer.getForename());
    ui->lineEdit_surname->setText(customer.getSurname());
    ui->lineEdit_addressLine1->setText(customer.getAddressLine1());
    ui->lineEdit_addressLine2->setText(customer.getAddressLine2());
    ui->lineEdit_town->setText(customer.getTown());
    ui->lineEdit_postcode->setText(customer.getPostcode());
    ui->lineEdit_homePhone->setText(customer.getHomePhoneNumber());
    ui->lineEdit_mobilePhone->setText(customer.getMobilePhoneNumber());
    ui->lineEdit_emailAddress->setText(customer.getEmailAddress());
}

void CustomerForm::on_pushButton_submit_released()
{
    if (!setRecordData()) return;

    if (formType == EDIT)
    {
        if (CustomerController::Update(customer, this)) done(Accepted);
    }
    else
    {
        if (CustomerController::Create(customer, this)) done(Accepted);
    }
}

void CustomerForm::on_pushButton_cancel_released()
{
    done(Rejected);
}

void CustomerForm::on_lineEdit_forename_textChanged(const QString &value)
{
    bool success = true;
    try { customer.setForename(value.toStdString().c_str()); }
    catch (const std::exception & e)
    {
        success = false;
        ui->lineEdit_forename->setToolTip(e.what());
        ui->lineEdit_forename->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_forename->setStyleSheet("");
        ui->lineEdit_forename->setToolTip("");
    }
}

void CustomerForm::on_lineEdit_surname_textEdited(const QString &value)
{
    bool success = true;
    try { customer.setSurname(value.toStdString().c_str()); }
    catch (const std::exception & e)
    {
        success = false;
        ui->lineEdit_surname->setToolTip(e.what());
        ui->lineEdit_surname->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_surname->setStyleSheet("");
        ui->lineEdit_surname->setToolTip("");
    }
}

void CustomerForm::on_lineEdit_addressLine1_textChanged(const QString &value)
{
    bool success = true;
    try { customer.setAddressLine1(value.toStdString().c_str()); }
    catch (const std::exception & e)
    {
        success = false;
        ui->lineEdit_addressLine1->setToolTip(e.what());
        ui->lineEdit_addressLine1->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_addressLine1->setStyleSheet("");
        ui->lineEdit_addressLine1->setToolTip("");
    }
}

void CustomerForm::on_lineEdit_addressLine2_textChanged(const QString &value)
{
    bool success = true;
    try { customer.setAddressLine2(value.toStdString().c_str()); }
    catch (const std::exception & e)
    {
        success = false;
        ui->lineEdit_addressLine2->setToolTip(e.what());
        ui->lineEdit_addressLine2->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_addressLine2->setStyleSheet("");
        ui->lineEdit_addressLine2->setToolTip("");
    }
}

void CustomerForm::on_lineEdit_town_textChanged(const QString &value)
{
    bool success = true;
    try { customer.setTown(value.toStdString().c_str()); }
    catch (const std::exception & e)
    {
        success = false;
        ui->lineEdit_town->setToolTip(e.what());
        ui->lineEdit_town->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_town->setStyleSheet("");
        ui->lineEdit_town->setToolTip("");
    }
}

void CustomerForm::on_lineEdit_postcode_textChanged(const QString &value)
{
    bool success = true;
    try { customer.setPostcode(value.toStdString().c_str()); }
    catch (const std::exception & e)
    {
        success = false;
        ui->lineEdit_postcode->setToolTip(e.what());
        ui->lineEdit_postcode->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_postcode->setStyleSheet("");
        ui->lineEdit_postcode->setToolTip("");
    }
}

void CustomerForm::on_lineEdit_homePhone_textChanged(const QString &value)
{
    bool success = true;
    try { customer.setHomePhoneNumber(value.toStdString().c_str()); }
    catch (const std::exception & e)
    {
        success = false;
        ui->lineEdit_homePhone->setToolTip(e.what());
        ui->lineEdit_homePhone->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_homePhone->setStyleSheet("");
        ui->lineEdit_homePhone->setToolTip("");
    }
}

void CustomerForm::on_lineEdit_mobilePhone_textChanged(const QString &value)
{
    bool success = true;
    try { customer.setMobilePhoneNumber(value.toStdString().c_str()); }
    catch (const std::exception & e)
    {
        success = false;
        ui->lineEdit_mobilePhone->setToolTip(e.what());
        ui->lineEdit_mobilePhone->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_mobilePhone->setStyleSheet("");
        ui->lineEdit_mobilePhone->setToolTip("");
    }
}

void CustomerForm::on_lineEdit_emailAddress_textChanged(const QString &value)
{
    bool success = true;
    try { customer.setEmailAddress(value.toStdString().c_str()); }
    catch (const std::exception & e)
    {
        success = false;
        ui->lineEdit_emailAddress->setToolTip(e.what());
        ui->lineEdit_emailAddress->setStyleSheet("QLineEdit { background-color: red; }");
    }
    if (success)
    {
        ui->lineEdit_emailAddress->setStyleSheet("");
        ui->lineEdit_emailAddress->setToolTip("");
    }
}

bool CustomerForm::setRecordData()
{
    bool success = true;

    on_lineEdit_forename_textChanged(ui->lineEdit_forename->text());
    if (ui->lineEdit_forename->styleSheet() != "") success = false;

    on_lineEdit_surname_textEdited(ui->lineEdit_surname->text());
    if (ui->lineEdit_surname->styleSheet() != "") success = false;

    on_lineEdit_addressLine1_textChanged(ui->lineEdit_addressLine1->text());
    if (ui->lineEdit_addressLine1->styleSheet() != "") success = false;

    on_lineEdit_addressLine2_textChanged(ui->lineEdit_addressLine2->text());
    if (ui->lineEdit_addressLine2->styleSheet() != "") success = false;

    on_lineEdit_town_textChanged(ui->lineEdit_town->text());
    if (ui->lineEdit_town->styleSheet() != "") success = false;

    on_lineEdit_postcode_textChanged(ui->lineEdit_postcode->text());
    if (ui->lineEdit_postcode->styleSheet() != "") success = false;

    on_lineEdit_homePhone_textChanged(ui->lineEdit_homePhone->text());
    if (ui->lineEdit_homePhone->styleSheet() != "") success = false;

    on_lineEdit_mobilePhone_textChanged(ui->lineEdit_mobilePhone->text());
    if (ui->lineEdit_mobilePhone->styleSheet() != "") success = false;

    on_lineEdit_emailAddress_textChanged(ui->lineEdit_emailAddress->text());
    if (ui->lineEdit_emailAddress->styleSheet() != "") success = false;

    return success;
}
