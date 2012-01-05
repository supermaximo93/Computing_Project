#-------------------------------------------------
#
# Project created by QtCreator 2011-11-28T15:52:03
#
#-------------------------------------------------

QT       += core gui

TARGET = Computing_Project
TEMPLATE = app

LIBS += -lgtest

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
    testing/Customer_testing.cc

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
    ExpenseDialog.h

FORMS    += \
    CustomerDialog.ui \
    MainWindow.ui \
    JobDialog.ui \
    ExpenseDialog.ui
