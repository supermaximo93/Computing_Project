#-------------------------------------------------
#
# Project created by QtCreator 2011-11-28T15:52:03
#
#-------------------------------------------------

QT       += core gui

TARGET = Computing_Project
TEMPLATE = app

debug {
LIBS += -lgtest
DEFINES += COMPILE_TESTS
}

SOURCES += main.cpp\
        Record.cpp\
        Customer.cpp\
        Expense.cpp\
        Job.cpp\
        Part.cpp\
        Task.cpp \
    testing/unit/Customer_testing.cc \
    Utils.cpp \
    testing/unit/Expense_testing.cc \
    testing/unit/Job_testing.cc \
    testing/unit/Part_testing.cc \
    testing/unit/Task_testing.cc \
    testing/integration/CustomerDatabase_testing.cc \
    dialogs/main/MainWindow.cpp \
    dialogs/task/TaskDialog.cpp \
    dialogs/customer/CustomerNew.cpp \
    dialogs/expense/ExpenseNew.cpp \
    dialogs/job/JobShow.cpp \
    dialogs/job/JobIndex.cpp \
    JobController.cpp \
    Databases.cpp \
    CustomerController.cpp \
    Globals.cpp \
    dialogs/job/JobForm.cpp \
    PartController.cpp \
    TaskController.cpp \
    dialogs/job/PaymentMethodDialog.cpp \
    testing/integration/JobController_testing.cc \
    dialogs/part/PartForm.cpp \
    dialogs/part/PartShow.cpp \
    testing/integration/PartController_testing.cc \
    testing/integration/TaskController_testing.cc \
    testing/integration/CustomerController_testing.cc \
    ExpenseController.cpp \
    testing/integration/ExpenseController_testing.cc

HEADERS  +=\
        Database.h\
        Record.h\
        Globals.h\
        Customer.h\
        Expense.h\
        Job.h\
        Part.h\
        Task.h \
    AssignOncePointer.h \
    Utils.h \
    dialogs/main/MainWindow.h \
    dialogs/customer/CustomerNew.h \
    dialogs/expense/ExpenseNew.h \
    dialogs/task/TaskNew.h \
    JobController.h \
    dialogs/job/JobShow.h \
    dialogs/job/JobIndex.h \
    Databases.h \
    CustomerController.h \
    dialogs/job/JobForm.h \
    PartController.h \
    TaskController.h \
    dialogs/job/PaymentMethodDialog.h \
    dialogs/part/PartForm.h \
    dialogs/part/PartShow.h \
    ExpenseController.h

FORMS += \
    dialogs/main/MainWindow.ui \
    dialogs/task/TaskDialog.ui \
    dialogs/customer/CustomerNew.ui \
    dialogs/expense/ExpenseNew.ui \
    dialogs/job/JobShow.ui \
    dialogs/job/JobIndex.ui \
    dialogs/job/JobForm.ui \
    dialogs/job/PaymentMethodDialog.ui \
    dialogs/part/PartForm.ui \
    dialogs/part/PartShow.ui
