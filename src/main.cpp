#include "EQAutoKeys.hpp"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("Fusion"));
    EQAutoKeys w;
    w.show();
    return QApplication::exec();
}