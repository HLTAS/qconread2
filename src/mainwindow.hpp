#pragma once

#include <QtWidgets>
#include "logtableview.hpp"
#include "logtablemodel.hpp"
#include "fileinfodialog.hpp"
#include "frameinspectorwindow.hpp"
#include "playerplotwindow.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private slots:
	void openNewInstance();
	void openLogFile();
	void reloadLogFile();
	void showLogFileInfo();
	void showAnglemodUnit();
	void showFSUValues();
	void showGrid();
	void hideMostCommonFrameTimes();
	void showPrePM();
	void showPostPM();
	void jumpToStartOfLog();
	void jumpToEndOfLog();
	void showInspector();
	void showPlayerPlot();

	void currentChanged(const QModelIndex &current, const QModelIndex &previous);

protected:
	void closeEvent(QCloseEvent *event) override;
	void showEvent(QShowEvent *event) override;

private:
	QAction *newAct;
	QAction *openAct;
	QAction *reloadAct;
	QAction *logFileInfoAct;
	QAction *anglemodUnitAct;
	QAction *showFSUValuesAct;
	QAction *showGridAct;
	QAction *hideMostCommonFrameTimesAct;
	QAction *prePlayerMoveAct;
	QAction *postPlayerMoveAct;
	QActionGroup *playerMoveGroup;
	QAction *jumpToStartOfLogAct;
	QAction *jumpToEndOfLogAct;
	QAction *showInspectorAct;
	QAction *showPlayerPlotAct;

	FileInfoDialog *fileInfoDialog = nullptr;
	FrameInspectorWindow *frameInspectorWindow = nullptr;
	PlayerPlotWindow *playerPlotWindow = nullptr;

	LogTableView *logTableView;
	LogTableModel *logTableModel;

	void setupMenuBar();
	void setupStatusBar();
	void setupUi();

	void inspectCurrentRow();
	void plotCurrentRow();
};
