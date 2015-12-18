#pragma once

#include <QtWidgets>
#include "logtablemodel.hpp"

class FrameInspectorWindow : public QWidget
{
	Q_OBJECT

public:
	FrameInspectorWindow(QWidget *parent, const LogTableModel *model);

	void setupUi();

public slots:
	void inspectFrame();
	void inspectFrame(int row);

signals:
	void aboutToClose();

protected:
	void closeEvent(QCloseEvent *event) override;
	void showEvent(QShowEvent *event) override;

private slots:
	void tabChanged(int index);
	void damageListCurrentRowChanged(int index);
	void objListCurrentRowChanged(int index);
	void colListCurrentRowChanged(int index);

private:
	int lastRow = -1;
	const LogTableModel *logTableModel;

	QTabWidget *tabWidget;
	QWidget *damageTab;
	QWidget *velocityTab;
	QWidget *objectMoveTab;
	QWidget *collisionTab;
	QWidget *consolePrintTab;
	QWidget *commandBufferTab;
	QWidget *viewanglesTab;

	QLabel *yawLabel;
	QLabel *pitchLabel;
	QLabel *rollLabel;
	QLabel *punchYawLabel;
	QLabel *punchPitchLabel;
	QLabel *punchRollLabel;

	QLabel *yawText;
	QLabel *pitchText;
	QLabel *rollText;
	QLabel *punchYawText;
	QLabel *punchPitchText;
	QLabel *punchRollText;

	QListWidget *consolePrintListWidget;

	QLabel *velXLabel;
	QLabel *velYLabel;
	QLabel *velZLabel;
	QLabel *velHLabel;
	QLabel *vel3DLabel;
	QLabel *velAngLabel;
	QLabel *bvelXLabel;
	QLabel *bvelYLabel;
	QLabel *bvelZLabel;

	QLabel *velXText;
	QLabel *velYText;
	QLabel *velZText;
	QLabel *velHText;
	QLabel *vel3DText;
	QLabel *velAngText;
	QLabel *bvelXText;
	QLabel *bvelYText;
	QLabel *bvelZText;

	QListWidget *objectMoveListWidget;

	QLabel *objPullLabel;
	QLabel *objVelXLabel;
	QLabel *objVelYLabel;
	QLabel *objVelZLabel;
	QLabel *objVelHLabel;
	QLabel *objVelAngLabel;
	QLabel *objPosXLabel;
	QLabel *objPosYLabel;
	QLabel *objPosZLabel;

	QLabel *objPullText;
	QLabel *objVelXText;
	QLabel *objVelYText;
	QLabel *objVelZText;
	QLabel *objVelHText;
	QLabel *objVelAngText;
	QLabel *objPosXText;
	QLabel *objPosYText;
	QLabel *objPosZText;

	QTextEdit *commandBufferText;

	QListWidget *damageListWidget;

	QLabel *dmgAmountLabel;
	QLabel *dmgDirYawLabel;
	QLabel *dmgDirPitchLabel;
	QLabel *dmgTypeLabel;

	QLabel *dmgAmountText;
	QLabel *dmgDirYawText;
	QLabel *dmgDirPitchText;
	QLabel *dmgTypeText;

	QListWidget *collisionListWidget;

	QLabel *colEntityLabel;
	QLabel *colImpactAngLabel;
	QLabel *colNYawLabel;
	QLabel *colNPitchLabel;

	QLabel *colEntityText;
	QLabel *colImpactAngText;
	QLabel *colNYawText;
	QLabel *colNPitchText;

	void setupViewanglesTab();
	void setupConsolePrintTab();
	void setupVelocityTab();
	void setupObjectMoveTab();
	void setupCommandBufferTab();
	void setupDamageTab();
	void setupCollisionTab();
};
