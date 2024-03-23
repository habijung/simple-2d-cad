#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	w.resize(600, 400);
	w.setWindowTitle("Simple 2D CAD");
	w.show();

	return a.exec();
}
