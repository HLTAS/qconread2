#pragma once

#include <QtWidgets>
#include "logtableview.hpp"
#include "logtablemodel.hpp"
#include "fileinfodialog.hpp"
#include "frameinspectorwindow.hpp"
#include "playerplotwindow.hpp"
#include "settings.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private slots:
	void openNewInstance();
	void openLogFile();
	void openRecentFile();
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
	QMenu *openRecentMenu;
	QAction *reloadAct;
	QAction *closeAct;
	QAction *logFileInfoAct;
	QAction *quitAct;

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

	QAction *recentFileActionList[MaxRecentFiles];

	FileInfoDialog *fileInfoDialog = nullptr;
	FrameInspectorWindow *frameInspectorWindow = nullptr;
	PlayerPlotWindow *playerPlotWindow = nullptr;

	LogTableView *logTableView;
	LogTableModel *logTableModel;

	void setupMenuBar();
	void setupStatusBar();
	void setupUi();
	void populateRecentFiles();
	void updateRecentFiles(const QStringList &nameList);

	bool loadLogFile(const QString &fileName);

	void inspectCurrentRow();
	void plotCurrentRow();
};
