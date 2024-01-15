#include <QApplication>
#include <QStyleFactory>
#include "../Headers/EQAutoClicker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    EQAutoClicker w;
    w.show();
    return a.exec();
}