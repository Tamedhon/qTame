#include "qTame.h"
#include <QApplication>
#include <QRandomGenerator>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    QRandomGenerator::securelySeeded();

    qTame w;
	w.show();

	return a.exec();
}
