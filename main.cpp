#include <iostream>
using namespace std;

#include <QtGui/QApplication>
#include "MainWindow.h"

#ifdef COMPILE_TESTS
#include <gtest/gtest.h>
#endif

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);

#ifdef COMPILE_TESTS
    for (int i = 0; i < argc; ++i)
    {
        if ((strcmp(argv[i], "-t") == 1) || (strcmp(argv[i], "--test") == 1))
        {
            ::testing::InitGoogleTest(&(--argc), ++argv);
            return RUN_ALL_TESTS();;
        }
    }
#endif

    MainWindow w;
    w.show();

    int exitCode;
    try { exitCode = a.exec(); } catch(std::runtime_error & error) { cout << error.what() << endl; }
    return exitCode;
}
