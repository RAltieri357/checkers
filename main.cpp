#include "checkers.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    srand(time(0)); //random seed based on time
    QApplication a(argc, argv);
    Checkers w;
    w.show();

    return a.exec();
}
