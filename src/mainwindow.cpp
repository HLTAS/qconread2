#include "mainwindow.hpp"

MainWindow::MainWindow()
	: QMainWindow()
{
	setupMenuBar();
	setupStatusBar();
	setupUi();
}

void MainWindow::setupMenuBar()
{
	QMenu *fileMenu = menuBar()->addMenu("&File");
	newAct = fileMenu->addAction("&New", this, SLOT(openNewInstance()), QKeySequence::New);

	openAct = fileMenu->addAction("&Open...", this, SLOT(openLogFile()), QKeySequence::Open);

	reloadAct = fileMenu->addAction("&Reload", this, SLOT(reloadLogFile()), QKeySequence::Refresh);
	reloadAct->setEnabled(false);

	fileMenu->addSeparator();

	logFileInfoAct = fileMenu->addAction("Log File &Info...", this, SLOT(showLogFileInfo()));
	logFileInfoAct->setEnabled(false);

	QMenu *viewMenu = menuBar()->addMenu("&View");
	anglemodUnitAct = viewMenu->addAction("Viewangles in Anglemod Unit",
		this, SLOT(showAnglemodUnit()), QKeySequence("Ctrl+Shift+A"));
	anglemodUnitAct->setCheckable(true);
	anglemodUnitAct->setEnabled(false);

	showFSUValuesAct = viewMenu->addAction("Show FSU &Values", this, SLOT(showFSUValues()));
	showFSUValuesAct->setCheckable(true);
	showFSUValuesAct->setEnabled(false);

	showGridAct = viewMenu->addAction("Show &Grid", this, SLOT(showGrid()), QKeySequence("Ctrl+G"));
	showGridAct->setCheckable(true);
	showGridAct->setChecked(true);
	showGridAct->setEnabled(false);

	hideMostCommonFrameTimesAct = viewMenu->addAction("Hide Most &Frequent Frametimes",
		this, SLOT(hideMostCommonFrameTimes()), QKeySequence("Ctrl+Shift+F"));
	hideMostCommonFrameTimesAct->setCheckable(true);
	hideMostCommonFrameTimesAct->setEnabled(false);

	viewMenu->addSeparator();

	prePlayerMoveAct = viewMenu->addAction("Show P&re-PM State",
		this, SLOT(showPrePM()), QKeySequence("{"));
	prePlayerMoveAct->setCheckable(true);
	postPlayerMoveAct = viewMenu->addAction("Show P&ost-PM State",
		this, SLOT(showPostPM()), QKeySequence("}"));
	postPlayerMoveAct->setCheckable(true);
	playerMoveGroup = new QActionGroup(this);
	playerMoveGroup->setEnabled(false);
	playerMoveGroup->addAction(prePlayerMoveAct);
	playerMoveGroup->addAction(postPlayerMoveAct);
	postPlayerMoveAct->setChecked(true);

	QMenu *navigateMenu = menuBar()->addMenu("&Navigate");
	jumpToStartOfLogAct = navigateMenu->addAction("Jump to &Start of Log",
		this, SLOT(jumpToStartOfLog()), QKeySequence::MoveToStartOfDocument);
	jumpToStartOfLogAct->setEnabled(false);
	jumpToEndOfLogAct = navigateMenu->addAction("Jump to &End of Log",
		this, SLOT(jumpToEndOfLog()), QKeySequence::MoveToEndOfDocument);
	jumpToEndOfLogAct->setEnabled(false);

	QMenu *toolsMenu = menuBar()->addMenu("&Tools");
	showInspectorAct = toolsMenu->addAction("Frame &Inspector",
		this, SLOT(showInspector()), QKeySequence("F"));
	showInspectorAct->setCheckable(true);

	showPlayerPlotAct = toolsMenu->addAction("&Player Plot",
		this, SLOT(showPlayerPlot()), QKeySequence("R"));
	showPlayerPlotAct->setCheckable(true);
}

void MainWindow::openNewInstance()
{
	// No parent needed, since it has Qt::WA_DeleteOnClose
	MainWindow *newWindow = new MainWindow;
	newWindow->show();
}

void MainWindow::hideMostCommonFrameTimes()
{
	logTableModel->setHideMostCommonFrameTimes(hideMostCommonFrameTimesAct->isChecked());
}

void MainWindow::showFSUValues()
{
	logTableModel->setShowFSUValues(showFSUValuesAct->isChecked());
	if (showFSUValuesAct->isChecked()) {
		QHeaderView *header = logTableView->horizontalHeader();
		if (logTableView->columnWidth(ForwardMoveHeader) < 30)
			header->resizeSection(ForwardMoveHeader, 50);
		if (logTableView->columnWidth(SideMoveHeader) < 30)
			header->resizeSection(SideMoveHeader, 50);
		if (logTableView->columnWidth(UpMoveHeader) < 30)
			header->resizeSection(UpMoveHeader, 50);
	}
}

void MainWindow::showPlayerPlot()
{
	if (!playerPlotWindow) {
		playerPlotWindow = new PlayerPlotWindow(this, logTableModel);
		connect(playerPlotWindow, SIGNAL(aboutToClose()), showPlayerPlotAct, SLOT(toggle()));
	}

	if (showPlayerPlotAct->isChecked()) {
		playerPlotWindow->show();
		plotCurrentRow();
	} else
		playerPlotWindow->hide();
}

void MainWindow::showInspector()
{
	if (!frameInspectorWindow) {
		frameInspectorWindow = new FrameInspectorWindow(this, logTableModel);
		connect(frameInspectorWindow, SIGNAL(aboutToClose()), showInspectorAct, SLOT(toggle()));
	}

	if (showInspectorAct->isChecked()) {
		frameInspectorWindow->show();
		inspectCurrentRow();
	} else
		frameInspectorWindow->hide();
}

void MainWindow::showLogFileInfo()
{
	if (!fileInfoDialog) {
		fileInfoDialog = new FileInfoDialog(this, logTableModel);
		connect(logTableModel, SIGNAL(logFileLoaded(bool)),
			fileInfoDialog, SLOT(updateFileInfo(bool)));
	}
	fileInfoDialog->updateFileInfo(true);
	fileInfoDialog->show();
	fileInfoDialog->raise();
	fileInfoDialog->activateWindow();
}

void MainWindow::jumpToStartOfLog()
{
	logTableView->scrollToTop();
}

void MainWindow::jumpToEndOfLog()
{
	logTableView->scrollToBottom();
}

void MainWindow::inspectCurrentRow()
{
	if (!frameInspectorWindow)
		return;
	const QModelIndex &currentIndex = logTableView->currentIndex();
	if (currentIndex.isValid())
		frameInspectorWindow->inspectFrame(currentIndex.row());
}

void MainWindow::plotCurrentRow()
{
	if (!playerPlotWindow)
		return;
	const QModelIndex &currentIndex = logTableView->currentIndex();
	if (currentIndex.isValid())
		playerPlotWindow->plotFrame(currentIndex.row());
}

void MainWindow::showAnglemodUnit()
{
	logTableModel->setShowAnglemodUnit(anglemodUnitAct->isChecked());
	inspectCurrentRow();
}

void MainWindow::showGrid()
{
	logTableView->setShowGrid(showGridAct->isChecked());
}

void MainWindow::showPrePM()
{
	logTableModel->setShowPlayerMove(true);
	inspectCurrentRow();
	plotCurrentRow();
}

void MainWindow::showPostPM()
{
	logTableModel->setShowPlayerMove(false);
	inspectCurrentRow();
	plotCurrentRow();
}

void MainWindow::setupStatusBar()
{
	statusBar();
}

void MainWindow::currentChanged(const QModelIndex &current, const QModelIndex &)
{
	if (frameInspectorWindow)
		frameInspectorWindow->inspectFrame(current.row());

	if (playerPlotWindow)
		playerPlotWindow->plotFrame(current.row());
}

void MainWindow::setupUi()
{
	resize(800, 600);
	setWindowTitle("qconread2");
	setAttribute(Qt::WA_DeleteOnClose);

	logTableView = new LogTableView(this);
	logTableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	logTableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	logTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	logTableView->horizontalHeader()->setSectionsMovable(true);
	logTableView->horizontalHeader()->setDefaultSectionSize(90);
	logTableView->verticalHeader()->setDefaultSectionSize(25);
	connect(logTableView, SIGNAL(currentChanged_(const QModelIndex &, const QModelIndex &)),
		this, SLOT(currentChanged(const QModelIndex &, const QModelIndex &)));
	setCentralWidget(logTableView);

	logTableModel = new LogTableModel(logTableView);
	connect(logTableModel, SIGNAL(logFileLoaded(bool)),
		reloadAct, SLOT(setEnabled(bool)));
	connect(logTableModel, SIGNAL(logFileLoaded(bool)),
		playerMoveGroup, SLOT(setEnabled(bool)));
	connect(logTableModel, SIGNAL(logFileLoaded(bool)),
		anglemodUnitAct, SLOT(setEnabled(bool)));
	connect(logTableModel, SIGNAL(logFileLoaded(bool)),
		showFSUValuesAct, SLOT(setEnabled(bool)));
	connect(logTableModel, SIGNAL(logFileLoaded(bool)),
		showGridAct, SLOT(setEnabled(bool)));
	connect(logTableModel, SIGNAL(logFileLoaded(bool)),
		hideMostCommonFrameTimesAct, SLOT(setEnabled(bool)));
	connect(logTableModel, SIGNAL(logFileLoaded(bool)),
		jumpToStartOfLogAct, SLOT(setEnabled(bool)));
	connect(logTableModel, SIGNAL(logFileLoaded(bool)),
		jumpToEndOfLogAct, SLOT(setEnabled(bool)));
	connect(logTableModel, SIGNAL(logFileLoaded(bool)),
		logFileInfoAct, SLOT(setEnabled(bool)));

	logTableView->setModel(logTableModel);
	logTableView->resizeColumnToContents(OnGroundHeader);
	logTableView->resizeColumnToContents(DuckStateHeader);
	logTableView->resizeColumnToContents(JumpHeader);
	logTableView->resizeColumnToContents(DuckHeader);
	logTableView->resizeColumnToContents(ForwardMoveHeader);
	logTableView->resizeColumnToContents(SideMoveHeader);
	logTableView->resizeColumnToContents(UpMoveHeader);
	logTableView->resizeColumnToContents(UseHeader);
	logTableView->resizeColumnToContents(AttackHeader);
	logTableView->resizeColumnToContents(Attack2Header);
	logTableView->resizeColumnToContents(ReloadHeader);
	logTableView->resizeColumnToContents(OnLadderHeader);
	logTableView->resizeColumnToContents(WaterLevelHeader);
	logTableView->resizeColumnToContents(ClientStateHeader);
	logTableView->resizeColumnToContents(EntityFrictionHeader);
	logTableView->resizeColumnToContents(EntityGravityHeader);
	logTableView->setColumnWidth(CommandFrameTimeHeader, 40);
	logTableView->setColumnWidth(FramebulkIdHeader, 50);
	logTableView->setColumnWidth(SharedSeedHeader, 60);
	logTableView->setColumnWidth(HealthHeader, 50);
	logTableView->setColumnWidth(ArmorHeader, 50);
}

void MainWindow::reloadLogFile()
{
	logTableModel->openLogFile(logTableModel->logFileName());
}

void MainWindow::openLogFile()
{
	const QString fileName = QFileDialog::getOpenFileName(this, "Open Log File");
	if (fileName.isEmpty())
		return;

	logTableModel->openLogFile(fileName);
}
