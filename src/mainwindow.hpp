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
	void showAnglemodUnit();
	void showGrid();
	void showPrePM();
	void showPostPM();

private:
	QAction *openAct;
	QAction *anglemodUnitAct;
	QAction *showGridAct;
	QAction *prePlayerMoveAct;
	QAction *postPlayerMoveAct;
	QActionGroup *playerMoveGroup;

	QTableView *logTableView;
	LogTableModel *logTableModel;

	void setupMenuBar();
	void setupStatusBar();
	void setupUi();
};
