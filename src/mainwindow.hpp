#pragma once

#include <QtWidgets>
#include "logtableview.hpp"
#include "logtablemodel.hpp"
#include "fileinfodialog.hpp"
#include "frameinspectorwindow.hpp"

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
	void showInspector();

	void currentChanged(const QModelIndex &current, const QModelIndex &previous);

private:
	QAction *openAct;
	QAction *reloadAct;
	QAction *logFileInfoAct;
	QAction *anglemodUnitAct;
	QAction *showGridAct;
	QAction *prePlayerMoveAct;
	QAction *postPlayerMoveAct;
	QActionGroup *playerMoveGroup;
	QAction *jumpToStartOfLogAct;
	QAction *jumpToEndOfLogAct;
	QAction *showInspectorAct;

	FileInfoDialog *fileInfoDialog = nullptr;
	FrameInspectorWindow *frameInspectorWindow = nullptr;

	LogTableView *logTableView;
	LogTableModel *logTableModel;

	void setupMenuBar();
	void setupStatusBar();
	void setupUi();

	void inspectCurrentRow();
};
