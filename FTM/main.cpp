#include "ftm.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FTM w;
	w.show();
	return a.exec();
}
