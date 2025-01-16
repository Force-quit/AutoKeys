#include "EQAutoKeys.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EQAutoKeys w;
    w.show();
    return QApplication::exec();
}