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
    CustomerDialog.cpp \
    MainWindow.cpp \
    JobDialog.cpp \
    ExpenseDialog.cpp \
    testing/unit/Customer_testing.cc \
    Utils.cpp \
    testing/unit/Expense_testing.cc \
    testing/unit/Job_testing.cc \
    testing/unit/Part_testing.cc \
    testing/unit/Task_testing.cc \
    TaskDialog.cpp \
    PartDialog.cpp \
    testing/integration/CustomerDatabase_testing.cc

HEADERS  +=\
        Database.h\
        Record.h\
        Globals.h\
        Customer.h\
        Expense.h\
        Job.h\
        Part.h\
        Task.h \
    CustomerDialog.h \
    MainWindow.h \
    JobDialog.h \
    ExpenseDialog.h \
    AssignOncePointer.h \
    Utils.h \
    TaskDialog.h \
    PartDialog.h

FORMS    += \
    CustomerDialog.ui \
    MainWindow.ui \
    JobDialog.ui \
    ExpenseDialog.ui \
    TaskDialog.ui \
    PartDialog.ui
