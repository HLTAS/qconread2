#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setOrganizationName("HLTAS");
	app.setApplicationName("qconread2");

	MainWindow mainWindow;
	mainWindow.show();

	return app.exec();
}
