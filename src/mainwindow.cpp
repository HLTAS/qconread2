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
	openAct = fileMenu->addAction("&Open...", this, SLOT(openLogFile()));
	openAct->setShortcuts(QKeySequence::Open);

	QMenu *viewMenu = menuBar()->addMenu("&View");
	anglemodUnitAct = viewMenu->addAction("Viewangles in Anglemod Unit",
		this, SLOT(showAnglemodUnit()), QKeySequence("Ctrl+Shift+P"));
	anglemodUnitAct->setCheckable(true);

	showGridAct = viewMenu->addAction("Show &grid", this, SLOT(showGrid()), QKeySequence("Ctrl+G"));
	showGridAct->setCheckable(true);
	showGridAct->setChecked(true);

	viewMenu->addSeparator();

	prePlayerMoveAct = viewMenu->addAction("Show P&re-PM State",
		this, SLOT(showPrePM()), QKeySequence("{"));
	prePlayerMoveAct->setCheckable(true);
	postPlayerMoveAct = viewMenu->addAction("Show P&ost-PM State",
		this, SLOT(showPostPM()), QKeySequence("}"));
	postPlayerMoveAct->setCheckable(true);
	playerMoveGroup = new QActionGroup(this);
	playerMoveGroup->addAction(prePlayerMoveAct);
	playerMoveGroup->addAction(postPlayerMoveAct);
	prePlayerMoveAct->setChecked(true);
}

void MainWindow::showAnglemodUnit()
{
	logTableModel->setShowAnglemodUnit(anglemodUnitAct->isChecked());
}

void MainWindow::showGrid()
{
	logTableView->setShowGrid(showGridAct->isChecked());
}

void MainWindow::showPrePM()
{
	logTableModel->setShowPlayerMove(true);
}

void MainWindow::showPostPM()
{
	logTableModel->setShowPlayerMove(false);
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
	logTableView->horizontalHeader()->setSectionsMovable(true);
	logTableView->horizontalHeader()->setDefaultSectionSize(90);
	logTableView->verticalHeader()->setDefaultSectionSize(25);
	setCentralWidget(logTableView);

	logTableModel = new LogTableModel(logTableView);
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
	logTableView->setColumnWidth(CommandFrameTimeHeader, 40);
	logTableView->setColumnWidth(FramebulkIdHeader, 50);

	resize(800, 600);
}

void MainWindow::openLogFile()
{
	const QString fileName = QFileDialog::getOpenFileName(this, "Open Log File");
	if (fileName.isEmpty())
		return;

	logTableModel->openLogFile(fileName);
}
