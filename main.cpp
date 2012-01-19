#include <iostream>
using namespace std;

#include <QtGui/QApplication>
#include "MainWindow.h"

#include <gtest/gtest.h>

int main(int argc, char * argv[]) {
    QApplication a(argc, argv);

    for (int i = 0; i < argc; ++i) {
        if ((strcmp(argv[i], "-t") == 1) || (strcmp(argv[i], "--test") == 1)) {
            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();;
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}
