#include "kinectgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	KinectGUI w;
	w.show();
	return a.exec();
}
