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
	void reloadLogFile();
	void showAnglemodUnit();
	void showGrid();
	void showPrePM();
	void showPostPM();
	void jumpToStartOfLog();
	void jumpToEndOfLog();

private:
	QAction *openAct;
	QAction *reloadAct;
	QAction *anglemodUnitAct;
	QAction *showGridAct;
	QAction *prePlayerMoveAct;
	QAction *postPlayerMoveAct;
	QAction *jumpToStartOfLogAct;
	QAction *jumpToEndOfLogAct;
	QActionGroup *playerMoveGroup;

	QTableView *logTableView;
	LogTableModel *logTableModel;

	void setupMenuBar();
	void setupStatusBar();
	void setupUi();
};
