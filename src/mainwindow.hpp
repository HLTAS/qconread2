#pragma once

#include <QtWidgets>
#include "logtablemodel.hpp"
#include "fileinfodialog.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private slots:
	void openLogFile();
	void reloadLogFile();
	void showLogFileInfo();
	void showAnglemodUnit();
	void showGrid();
	void showPrePM();
	void showPostPM();
	void jumpToStartOfLog();
	void jumpToEndOfLog();

private:
	QAction *openAct;
	QAction *reloadAct;
	QAction *logFileInfoAct;
	QAction *anglemodUnitAct;
	QAction *showGridAct;
	QAction *prePlayerMoveAct;
	QAction *postPlayerMoveAct;
	QAction *jumpToStartOfLogAct;
	QAction *jumpToEndOfLogAct;
	QActionGroup *playerMoveGroup;

	FileInfoDialog *fileInfoDialog;

	QTableView *logTableView;
	LogTableModel *logTableModel;

	void setupMenuBar();
	void setupStatusBar();
	void setupUi();
};
