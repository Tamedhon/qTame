#include "qTame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    a.setOrganizationName("Tamedhon");
    a.setApplicationName("qTame");

    qTame w;
	w.show();

	return a.exec();
}
