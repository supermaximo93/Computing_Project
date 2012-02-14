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
    dialogs/job/JobNew.cpp \
    dialogs/part/PartNew.cpp \
    dialogs/job/JobEdit.cpp \
    dialogs/job/JobShow.cpp \
    dialogs/job/JobIndex.cpp \
    JobController.cpp \
    Databases.cpp

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
    dialogs/job/JobNew.h \
    dialogs/part/PartNew.h \
    dialogs/task/TaskNew.h \
    JobController.h \
    dialogs/job/JobEdit.h \
    dialogs/job/JobShow.h \
    dialogs/job/JobIndex.h \
    Databases.h

FORMS += \
    dialogs/main/MainWindow.ui \
    dialogs/task/TaskDialog.ui \
    dialogs/customer/CustomerNew.ui \
    dialogs/expense/ExpenseNew.ui \
    dialogs/job/JobNew.ui \
    dialogs/part/PartNew.ui \
    dialogs/job/JobEdit.ui \
    dialogs/job/JobShow.ui \
    dialogs/job/JobIndex.ui
