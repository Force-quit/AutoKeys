#include "../Headers/EQAutoClicker.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    EQAutoClicker w;
    w.show();
    return QApplication::exec();
}