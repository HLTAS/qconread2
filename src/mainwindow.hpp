#pragma once

#include <QtWidgets>
#include "logtablemodel.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private slots:
	void openLogFile();

private:
	QAction *openAct;
	QTableView *logTableView;
	LogTableModel *logTableModel;

	void setupMenuBar();
	void setupStatusBar();
	void setupUi();
};
