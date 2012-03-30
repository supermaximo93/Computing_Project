#include <iostream>
#include <fstream>
using namespace std;

#include <QtGui/QApplication>
#include "dialogs/main/MainWindow.h"

#ifdef COMPILE_TESTS
#include <gtest/gtest.h>
#endif

#include "Databases.h"
#include "Utils.h"

#include "dialogs/utils/PasswordDialog.h"

int main(int argc, char *argv[])
{
    // Redirect buffer so that it prints to a logfile instead of the console
    ofstream log;
    log.open("logfile.log");
    streambuf *const coutStreamBuffer = cout.rdbuf();
    cout.rdbuf(log.rdbuf());

    QApplication a(argc, argv);

#ifdef COMPILE_TESTS
    for (int i = 0; i < argc; ++i)
    {
        if ((strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--test") == 0))
        {
            char arg1[] = "--gtest_output=xml:test_report.xml";
            char *args[] = { argv[0], arg1 };
            int argCount = 2;
            ::testing::InitGoogleTest(&argCount, args);
            return RUN_ALL_TESTS();
        }
    }
#endif

    Databases::init();
    int exitCode = 1;

    if (PasswordDialog().exec() == PasswordDialog::Accepted)
    {
        MainWindow w;
        w.show();

        try { exitCode = a.exec(); } catch(const std::exception &e) { showFatalDialog(e.what()); }
    }

    Databases::finalise();

    log.close();
    cout.rdbuf(coutStreamBuffer);

    return exitCode;
}
