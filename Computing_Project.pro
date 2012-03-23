######################################################################
# Automatically generated by qmake (2.01a) Sat Mar 10 15:07:58 2012
######################################################################

QT       += core gui network

TARGET = Computing_Project
TEMPLATE = app

debug {
LIBS += -lgtest
DEFINES += COMPILE_TESTS
}

LIBS += -lhpdf -lQxtCore -lQxtNetwork

DEPENDPATH += . \
              dialogs/customer \
              dialogs/expense \
              dialogs/job \
              dialogs/main \
              dialogs/part \
              dialogs/task \
              testing/integration \
              testing/unit
INCLUDEPATH += . \
               dialogs/customer \
               dialogs/expense \
               dialogs/job \
               dialogs/main \
               dialogs/part \
               dialogs/task

# Input
HEADERS += \
           Customer.h \
           CustomerController.h \
           Database.h \
           Databases.h \
           Emailer.h \
           Expense.h \
           ExpenseController.h \
           Globals.h \
           Job.h \
           JobController.h \
           Part.h \
           PartController.h \
           PdfGenerator.h \
           Record.h \
           Task.h \
           TaskController.h \
           Utils.h \
           dialogs/customer/CustomerForm.h \
           dialogs/customer/CustomerIndex.h \
           dialogs/customer/CustomerShow.h \
           dialogs/expense/ExpenseForm.h \
           dialogs/expense/ExpenseIndex.h \
           dialogs/expense/ExpenseShow.h \
           dialogs/job/JobForm.h \
           dialogs/job/JobIndex.h \
           dialogs/job/JobShow.h \
           dialogs/job/PaymentMethodDialog.h \
           dialogs/main/MainWindow.h \
           dialogs/part/PartForm.h \
           dialogs/part/PartShow.h \
           dialogs/task/TaskForm.h \
           dialogs/task/TaskShow.h \
    EmailerThread.h \
    dialogs/utils/PendingDialog.h \
    dialogs/utils/ProcessDialog.h \
    testing/TestingHelpers.hpp \
    EmailDetails.h \
    Setting.h \
    SettingController.h \
    dialogs/setting/SettingForm.h
FORMS += dialogs/customer/CustomerForm.ui \
         dialogs/customer/CustomerIndex.ui \
         dialogs/customer/CustomerShow.ui \
         dialogs/expense/ExpenseForm.ui \
         dialogs/expense/ExpenseIndex.ui \
         dialogs/expense/ExpenseShow.ui \
         dialogs/job/JobForm.ui \
         dialogs/job/JobIndex.ui \
         dialogs/job/JobShow.ui \
         dialogs/job/PaymentMethodDialog.ui \
         dialogs/main/MainWindow.ui \
         dialogs/part/PartForm.ui \
         dialogs/part/PartShow.ui \
         dialogs/task/TaskForm.ui \
         dialogs/task/TaskShow.ui \
    dialogs/utils/PendingDialog.ui \
    dialogs/utils/ProcessDialog.ui \
    dialogs/setting/SettingForm.ui
SOURCES += Customer.cpp \
           CustomerController.cpp \
           Databases.cpp \
           Emailer.cpp \
           Expense.cpp \
           ExpenseController.cpp \
           Globals.cpp \
           Job.cpp \
           JobController.cpp \
           main.cpp \
           Part.cpp \
           PartController.cpp \
           PdfGenerator.cpp \
           Record.cpp \
           Task.cpp \
           TaskController.cpp \
           Utils.cpp \
           dialogs/customer/CustomerForm.cpp \
           dialogs/customer/CustomerIndex.cpp \
           dialogs/customer/CustomerShow.cpp \
           dialogs/expense/ExpenseForm.cpp \
           dialogs/expense/ExpenseIndex.cpp \
           dialogs/expense/ExpenseShow.cpp \
           dialogs/job/JobForm.cpp \
           dialogs/job/JobIndex.cpp \
           dialogs/job/JobShow.cpp \
           dialogs/job/PaymentMethodDialog.cpp \
           dialogs/main/MainWindow.cpp \
           dialogs/part/PartForm.cpp \
           dialogs/part/PartShow.cpp \
           dialogs/task/TaskForm.cpp \
           dialogs/task/TaskShow.cpp \
           testing/integration/CustomerController_testing.cc \
           testing/integration/CustomerDatabase_testing.cc \
           testing/integration/ExpenseController_testing.cc \
           testing/integration/JobController_testing.cc \
           testing/integration/PartController_testing.cc \
           testing/integration/TaskController_testing.cc \
           testing/unit/Customer_testing.cc \
           testing/unit/Expense_testing.cc \
           testing/unit/Job_testing.cc \
           testing/unit/Part_testing.cc \
           testing/unit/Task_testing.cc \
    EmailerThread.cpp \
    dialogs/utils/PendingDialog.cpp \
    dialogs/utils/ProcessDialog.cpp \
    testing/unit/Utils_testing.cc \
    testing/TestingHelpers.cc \
    Setting.cpp \
    SettingController.cpp \
    testing/integration/SettingController_testing.cc \
    dialogs/setting/SettingForm.cpp \
    testing/unit/Setting_testing.cc

OTHER_FILES +=
