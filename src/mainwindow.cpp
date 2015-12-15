#include <QtWidgets>
#include "mainwindow.hpp"

MainWindow::MainWindow()
{
	setupMenuBar();
	setupStatusBar();
	setupUi();
}

void MainWindow::setupMenuBar()
{
	QMenu *fileMenu = menuBar()->addMenu("&File");
	openAct = fileMenu->addAction("&Open...", this, SLOT(openLogFile()), QKeySequence("Ctrl+O"));
}

void MainWindow::setupStatusBar()
{
	statusBar();
}

void MainWindow::setupUi()
{
	logTableView = new QTableView(this);
	logTableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	logTableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	logTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	setCentralWidget(logTableView);

	logTableModel = new LogTableModel(logTableView);
	logTableView->setModel(logTableModel);

	resize(800, 600);
}

void MainWindow::openLogFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open Log File");

	if (fileName.isEmpty())
		return;

	logTableModel->openLogFile(fileName);
}
