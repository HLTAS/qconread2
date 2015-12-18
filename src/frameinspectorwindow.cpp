#include <map>
#include "frameinspectorwindow.hpp"

static const int ListWidgetFixedWidth = 60;
static const float M_U = 360.0 / 65536;
static const QString NotAppl = "N/A";
static const QString DegreesFormat = "%1˚";

static const std::map<int, QString> DamageStringTable = {
	{0, "generic"},
	{1 << 0, "crush"},
	{1 << 1, "bullet"},
	{1 << 2, "slash"},
	{1 << 3, "burn"},
	{1 << 4, "freeze"},
	{1 << 5, "fall"},
	{1 << 6, "blast"},
	{1 << 7, "club"},
	{1 << 8, "shock"},
	{1 << 9, "sonic"},
	{1 << 10, "energybeam"},
	{1 << 12, "nevergib"},
	{1 << 13, "alwaysgib"},
	{1 << 14, "drown"},
	{1 << 15, "paralyze"},
	{1 << 16, "nervegas"},
	{1 << 17, "poison"},
	{1 << 18, "radiation"},
	{1 << 19, "drownrecover"},
	{1 << 20, "acid"},
	{1 << 21, "slowburn"},
	{1 << 22, "slowfreeze"},
	{1 << 23, "mortar"},
};

FrameInspectorWindow::FrameInspectorWindow(QWidget *parent, const LogTableModel *model)
	: QWidget(parent), logTableModel(model)
{
	setupUi();
}

void FrameInspectorWindow::setupUi()
{
	setWindowTitle("Frame Inspector");
	setWindowFlags(Qt::Tool);

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	setLayout(mainLayout);

	tabWidget = new QTabWidget(this);
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	mainLayout->addWidget(tabWidget);

	setupViewanglesTab();
	setupDamageTab();
	setupCollisionTab();
	setupObjectMoveTab();
	setupVelocityTab();
	setupConsolePrintTab();
	setupCommandBufferTab();
}

void FrameInspectorWindow::setupViewanglesTab()
{
	viewanglesTab = new QWidget(tabWidget);
	tabWidget->addTab(viewanglesTab, "View");

	QGridLayout *lay = new QGridLayout(viewanglesTab);
	viewanglesTab->setLayout(lay);

	yawLabel = new QLabel("Yaw:", viewanglesTab);
	pitchLabel = new QLabel("Pitch:", viewanglesTab);
	rollLabel = new QLabel("Roll:", viewanglesTab);
	punchYawLabel = new QLabel("Punch yaw:", viewanglesTab);
	punchPitchLabel = new QLabel("Punch pitch:", viewanglesTab);
	punchRollLabel = new QLabel("Punch roll:", viewanglesTab);

	yawText = new QLabel(NotAppl, viewanglesTab);
	pitchText = new QLabel(NotAppl, viewanglesTab);
	rollText = new QLabel(NotAppl, viewanglesTab);
	punchYawText = new QLabel(NotAppl, viewanglesTab);
	punchPitchText = new QLabel(NotAppl, viewanglesTab);
	punchRollText = new QLabel(NotAppl, viewanglesTab);

	yawText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	pitchText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	rollText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	punchYawText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	punchPitchText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	punchRollText->setTextInteractionFlags(Qt::TextSelectableByMouse);

	lay->addWidget(yawLabel, 0, 0, Qt::AlignRight);
	lay->addWidget(pitchLabel, 1, 0, Qt::AlignRight);
	lay->addWidget(rollLabel, 2, 0, Qt::AlignRight);
	lay->addWidget(punchYawLabel, 3, 0, Qt::AlignRight);
	lay->addWidget(punchPitchLabel, 4, 0, Qt::AlignRight);
	lay->addWidget(punchRollLabel, 5, 0, Qt::AlignRight);

	lay->addWidget(yawText, 0, 1);
	lay->addWidget(pitchText, 1, 1);
	lay->addWidget(rollText, 2, 1);
	lay->addWidget(punchYawText, 3, 1);
	lay->addWidget(punchPitchText, 4, 1);
	lay->addWidget(punchRollText, 5, 1);

	lay->setColumnStretch(1, 1);
	lay->setRowStretch(6, 1);
}

void FrameInspectorWindow::setupConsolePrintTab()
{
	consolePrintTab = new QWidget(tabWidget);
	tabWidget->addTab(consolePrintTab, "Con");

	QVBoxLayout *lay = new QVBoxLayout(consolePrintTab);
	consolePrintTab->setLayout(lay);

	consolePrintListWidget = new QListWidget(consolePrintTab);
	consolePrintListWidget->setWordWrap(true);
	lay->addWidget(consolePrintListWidget);
}

void FrameInspectorWindow::setupVelocityTab()
{
	velocityTab = new QWidget(tabWidget);
	tabWidget->addTab(velocityTab, "Vel");

	QGridLayout *lay = new QGridLayout(velocityTab);
	velocityTab->setLayout(lay);

	velXLabel = new QLabel("Velocity X:", velocityTab);
	velYLabel = new QLabel("Velocity Y:", velocityTab);
	velZLabel = new QLabel("Velocity Z:", velocityTab);
	velHLabel = new QLabel("Horizontal speed:", velocityTab);
	vel3DLabel = new QLabel("3D speed:", velocityTab);
	velAngLabel = new QLabel("Velocity pitch:", velocityTab);
	bvelXLabel = new QLabel("Base velocity X:", velocityTab);
	bvelYLabel = new QLabel("Base velocity Y:", velocityTab);
	bvelZLabel = new QLabel("Base velocity Z:", velocityTab);

	velXText = new QLabel(NotAppl, velocityTab);
	velYText = new QLabel(NotAppl, velocityTab);
	velZText = new QLabel(NotAppl, velocityTab);
	velHText = new QLabel(NotAppl, velocityTab);
	vel3DText = new QLabel(NotAppl, velocityTab);
	velAngText = new QLabel(NotAppl, velocityTab);
	bvelXText = new QLabel(NotAppl, velocityTab);
	bvelYText = new QLabel(NotAppl, velocityTab);
	bvelZText = new QLabel(NotAppl, velocityTab);

	velXText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	velYText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	velZText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	velHText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	vel3DText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	velAngText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	bvelXText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	bvelYText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	bvelZText->setTextInteractionFlags(Qt::TextSelectableByMouse);

	lay->addWidget(velXLabel, 0, 0, Qt::AlignRight);
	lay->addWidget(velYLabel, 1, 0, Qt::AlignRight);
	lay->addWidget(velZLabel, 2, 0, Qt::AlignRight);
	lay->addWidget(velHLabel, 3, 0, Qt::AlignRight);
	lay->addWidget(vel3DLabel, 4, 0, Qt::AlignRight);
	lay->addWidget(velAngLabel, 5, 0, Qt::AlignRight);
	lay->addWidget(bvelXLabel, 6, 0, Qt::AlignRight);
	lay->addWidget(bvelYLabel, 7, 0, Qt::AlignRight);
	lay->addWidget(bvelZLabel, 8, 0, Qt::AlignRight);

	lay->addWidget(velXText, 0, 1);
	lay->addWidget(velYText, 1, 1);
	lay->addWidget(velZText, 2, 1);
	lay->addWidget(velHText, 3, 1);
	lay->addWidget(vel3DText, 4, 1);
	lay->addWidget(velAngText, 5, 1);
	lay->addWidget(bvelXText, 6, 1);
	lay->addWidget(bvelYText, 7, 1);
	lay->addWidget(bvelZText, 8, 1);

	lay->setColumnStretch(1, 1);
	lay->setRowStretch(9, 1);
}

void FrameInspectorWindow::setupObjectMoveTab()
{
	objectMoveTab = new QWidget(tabWidget);
	tabWidget->addTab(objectMoveTab, "Obj");

	QGridLayout *lay = new QGridLayout(objectMoveTab);
	objectMoveTab->setLayout(lay);

	objectMoveListWidget = new QListWidget(objectMoveTab);
	objectMoveListWidget->setFixedWidth(ListWidgetFixedWidth);
	connect(objectMoveListWidget, SIGNAL(currentRowChanged(int)),
		this, SLOT(objListCurrentRowChanged(int)));

	objPullLabel = new QLabel("Action:", objectMoveTab);
	objVelXLabel = new QLabel("Velocity X:", objectMoveTab);
	objVelYLabel = new QLabel("Velocity Y:", objectMoveTab);
	objVelZLabel = new QLabel("Velocity Z:", objectMoveTab);
	objVelHLabel = new QLabel("Horizontal speed:", objectMoveTab);
	objVelAngLabel = new QLabel("Velocity yaw:", objectMoveTab);
	objPosXLabel = new QLabel("Position X:", objectMoveTab);
	objPosYLabel = new QLabel("Position Y:", objectMoveTab);
	objPosZLabel = new QLabel("Position Z:", objectMoveTab);

	objPullText = new QLabel(NotAppl, objectMoveTab);
	objVelXText = new QLabel(NotAppl, objectMoveTab);
	objVelYText = new QLabel(NotAppl, objectMoveTab);
	objVelZText = new QLabel(NotAppl, objectMoveTab);
	objVelHText = new QLabel(NotAppl, objectMoveTab);
	objVelAngText = new QLabel(NotAppl, objectMoveTab);
	objPosXText = new QLabel(NotAppl, objectMoveTab);
	objPosYText = new QLabel(NotAppl, objectMoveTab);
	objPosZText = new QLabel(NotAppl, objectMoveTab);

	objPullText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	objVelXText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	objVelYText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	objVelZText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	objVelHText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	objVelAngText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	objPosXText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	objPosYText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	objPosZText->setTextInteractionFlags(Qt::TextSelectableByMouse);

	lay->addWidget(objectMoveListWidget, 0, 0, 10, 1);

	lay->addWidget(objPullLabel, 0, 1, Qt::AlignRight);
	lay->addWidget(objVelXLabel, 1, 1, Qt::AlignRight);
	lay->addWidget(objVelYLabel, 2, 1, Qt::AlignRight);
	lay->addWidget(objVelZLabel, 3, 1, Qt::AlignRight);
	lay->addWidget(objVelHLabel, 4, 1, Qt::AlignRight);
	lay->addWidget(objVelAngLabel, 5, 1, Qt::AlignRight);
	lay->addWidget(objPosXLabel, 6, 1, Qt::AlignRight);
	lay->addWidget(objPosYLabel, 7, 1, Qt::AlignRight);
	lay->addWidget(objPosZLabel, 8, 1, Qt::AlignRight);

	lay->addWidget(objPullText, 0, 2);
	lay->addWidget(objVelXText, 1, 2);
	lay->addWidget(objVelYText, 2, 2);
	lay->addWidget(objVelZText, 3, 2);
	lay->addWidget(objVelHText, 4, 2);
	lay->addWidget(objVelAngText, 5, 2);
	lay->addWidget(objPosXText, 6, 2);
	lay->addWidget(objPosYText, 7, 2);
	lay->addWidget(objPosZText, 8, 2);

	lay->setRowStretch(9, 1);
	lay->setColumnStretch(2, 1);
}

void FrameInspectorWindow::setupCommandBufferTab()
{
	commandBufferTab = new QWidget(tabWidget);
	tabWidget->addTab(commandBufferTab, "Cbuf");

	QVBoxLayout *lay = new QVBoxLayout(commandBufferTab);
	commandBufferTab->setLayout(lay);

	commandBufferText = new QTextEdit(commandBufferTab);
	commandBufferText->setReadOnly(true);
	commandBufferText->setPlaceholderText("Command buffer is empty");
	lay->addWidget(commandBufferText);
}

void FrameInspectorWindow::setupDamageTab()
{
	damageTab = new QWidget(tabWidget);
	tabWidget->addTab(damageTab, "Dmg");

	QGridLayout *lay = new QGridLayout(damageTab);
	damageTab->setLayout(lay);

	damageListWidget = new QListWidget(damageTab);
	damageListWidget->setFixedWidth(ListWidgetFixedWidth);
	connect(damageListWidget, SIGNAL(currentRowChanged(int)),
		this, SLOT(damageListCurrentRowChanged(int)));

	dmgAmountLabel = new QLabel("Magnitude:", this);
	dmgDirYawLabel = new QLabel("Dmg. dir. yaw:", this);
	dmgDirPitchLabel = new QLabel("Dmg. dir. pitch:", this);
	dmgTypeLabel = new QLabel("Damage type:", this);

	dmgAmountText = new QLabel(NotAppl, this);
	dmgDirYawText = new QLabel(NotAppl, this);
	dmgDirPitchText = new QLabel(NotAppl, this);
	dmgTypeText = new QLabel(NotAppl, this);

	dmgAmountText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	dmgDirYawText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	dmgDirPitchText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	dmgTypeText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	dmgTypeText->setWordWrap(true);

	lay->addWidget(damageListWidget, 0, 0, 5, 1);

	lay->addWidget(dmgAmountLabel, 0, 1, Qt::AlignRight);
	lay->addWidget(dmgTypeLabel, 1, 1, Qt::AlignRight | Qt::AlignTop);
	lay->addWidget(dmgDirYawLabel, 2, 1, Qt::AlignRight);
	lay->addWidget(dmgDirPitchLabel, 3, 1, Qt::AlignRight);

	lay->addWidget(dmgAmountText, 0, 2);
	lay->addWidget(dmgTypeText, 1, 2);
	lay->addWidget(dmgDirYawText, 2, 2);
	lay->addWidget(dmgDirPitchText, 3, 2);

	lay->setRowStretch(4, 1);
	lay->setColumnStretch(2, 1);
}

void FrameInspectorWindow::setupCollisionTab()
{
	collisionTab = new QWidget(tabWidget);
	tabWidget->addTab(collisionTab, "Col");

	QGridLayout *lay = new QGridLayout(collisionTab);
	collisionTab->setLayout(lay);

	collisionListWidget = new QListWidget(collisionTab);
	collisionListWidget->setFixedWidth(ListWidgetFixedWidth);
	connect(collisionListWidget, SIGNAL(currentRowChanged(int)),
		this, SLOT(colListCurrentRowChanged(int)));

	colEntityLabel = new QLabel("Entity:", collisionTab);
	colImpactAngLabel = new QLabel("Impact angle:", collisionTab);
	colNYawLabel = new QLabel("Normal yaw:", collisionTab);
	colNPitchLabel = new QLabel("Normal pitch:", collisionTab);

	colEntityText = new QLabel(NotAppl, collisionTab);
	colImpactAngText = new QLabel(NotAppl, collisionTab);
	colNYawText = new QLabel(NotAppl, collisionTab);
	colNPitchText = new QLabel(NotAppl, collisionTab);

	colEntityText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	colImpactAngText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	colNYawText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	colNPitchText->setTextInteractionFlags(Qt::TextSelectableByMouse);

	lay->addWidget(collisionListWidget, 0, 0, 5, 1);

	lay->addWidget(colEntityLabel, 0, 1, Qt::AlignRight);
	lay->addWidget(colNYawLabel, 1, 1, Qt::AlignRight);
	lay->addWidget(colNPitchLabel, 2, 1, Qt::AlignRight);
	lay->addWidget(colImpactAngLabel, 3, 1, Qt::AlignRight);

	lay->addWidget(colEntityText, 0, 2);
	lay->addWidget(colNYawText, 1, 2);
	lay->addWidget(colNPitchText, 2, 2);
	lay->addWidget(colImpactAngText, 3, 2);

	lay->setColumnStretch(2, 1);
	lay->setRowStretch(4, 1);
}

static QString angleToString(float angle, bool anglemodUnit)
{
	return anglemodUnit ? QString("%1u").arg(angle / M_U) : QString::number(angle);
}

void FrameInspectorWindow::inspectFrame()
{
	inspectFrame(lastRow);
}

void FrameInspectorWindow::inspectFrame(int row)
{
	lastRow = row;
	if (row == -1)
		return;

	const QWidget *currentWidget = tabWidget->widget(tabWidget->currentIndex());
	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	logTableModel->getFrameData(row, phyFrame, &cmdFrame, &pmState);

	if (currentWidget == viewanglesTab) {
		if (cmdFrame) {
			const bool showInU = logTableModel->showAnglemodUnit();
			yawText->setText(angleToString(cmdFrame->viewangles[0], showInU));
			pitchText->setText(angleToString(cmdFrame->viewangles[1], showInU));
			rollText->setText(angleToString(cmdFrame->viewangles[2], showInU));
			punchYawText->setText(angleToString(cmdFrame->punchangles[0], showInU));
			punchPitchText->setText(angleToString(cmdFrame->punchangles[1], showInU));
			punchRollText->setText(angleToString(cmdFrame->punchangles[2], showInU));
		} else {
			yawText->setText(NotAppl);
			pitchText->setText(NotAppl);
			rollText->setText(NotAppl);
			punchYawText->setText(NotAppl);
			punchPitchText->setText(NotAppl);
			punchRollText->setText(NotAppl);
		}
	} else if (currentWidget == consolePrintTab) {
		consolePrintListWidget->clear();
		if (phyFrame.consolePrintList.empty())
			return;

		for (const std::string &msg : phyFrame.consolePrintList)
			consolePrintListWidget->addItem(QString::fromStdString(msg).trimmed());
		consolePrintListWidget->setCurrentRow(0);
	} else if (currentWidget == velocityTab) {
		if (cmdFrame) {
			const float hspeed = std::hypot(pmState->velocity[0], pmState->velocity[1]);
			const float fullSpeed = std::sqrt(
				pmState->velocity[0] * pmState->velocity[0]
				+ pmState->velocity[1] * pmState->velocity[1]
				+ pmState->velocity[2] * pmState->velocity[2]
			);
			velXText->setText(QString::number(pmState->velocity[0]));
			velYText->setText(QString::number(pmState->velocity[1]));
			velZText->setText(QString::number(pmState->velocity[2]));
			velHText->setText(QString::number(hspeed));
			vel3DText->setText(QString::number(fullSpeed));
			if (pmState->velocity[0] == 0.0 && pmState->velocity[1] == 0.0
				&& pmState->velocity[2] == 0.0)
				velAngText->setText(NotAppl);
			else {
				const float ang = -std::asin(pmState->velocity[2] / fullSpeed) * 180 / M_PI;
				velAngText->setText(DegreesFormat.arg(ang));
			}
			bvelXText->setText(QString::number(pmState->baseVelocity[0]));
			bvelYText->setText(QString::number(pmState->baseVelocity[1]));
			bvelZText->setText(QString::number(pmState->baseVelocity[2]));
		} else {
			velXText->setText(NotAppl);
			velYText->setText(NotAppl);
			velZText->setText(NotAppl);
			velHText->setText(NotAppl);
			vel3DText->setText(NotAppl);
			velAngText->setText(NotAppl);
			bvelXText->setText(NotAppl);
			bvelYText->setText(NotAppl);
			bvelZText->setText(NotAppl);
		}
	} else if (currentWidget == objectMoveTab) {
		objectMoveListWidget->clear();
		if (!phyFrame.objectMoveList.empty()) {
			for (size_t i = 1; i <= phyFrame.objectMoveList.size(); i++)
				objectMoveListWidget->addItem(QString::number(i));
			objectMoveListWidget->setCurrentRow(0);
		} else {
			objPullText->setText(NotAppl);
			objVelXText->setText(NotAppl);
			objVelYText->setText(NotAppl);
			objVelZText->setText(NotAppl);
			objVelHText->setText(NotAppl);
			objVelAngText->setText(NotAppl);
			objPosXText->setText(NotAppl);
			objPosYText->setText(NotAppl);
			objPosZText->setText(NotAppl);
		}
	} else if (currentWidget == commandBufferTab) {
		commandBufferText->setText(
			phyFrame.commandBuffer.empty()
			? QString()
			: QString::fromStdString(phyFrame.commandBuffer)
		);
	} else if (currentWidget == damageTab) {
		damageListWidget->clear();
		if (!phyFrame.damageList.empty()) {
			for (size_t i = 1; i <= phyFrame.damageList.size(); i++)
				damageListWidget->addItem(QString::number(i));
			damageListWidget->setCurrentRow(0);
		} else {
			dmgAmountText->setText(NotAppl);
			dmgDirYawText->setText(NotAppl);
			dmgDirPitchText->setText(NotAppl);
			dmgTypeText->setText(NotAppl);
		}
	} else if (currentWidget == collisionTab) {
		collisionListWidget->clear();
		if (cmdFrame && !cmdFrame->collisionList.empty()) {
			for (size_t i = 1; i <= cmdFrame->collisionList.size(); i++)
				collisionListWidget->addItem(QString::number(i));
			collisionListWidget->setCurrentRow(0);
		} else {
			colEntityText->setText(NotAppl);
			colImpactAngText->setText(NotAppl);
			colNYawText->setText(NotAppl);
			colNPitchText->setText(NotAppl);
		}
	}
}

void FrameInspectorWindow::tabChanged(int)
{
	inspectFrame();
}

void FrameInspectorWindow::objListCurrentRowChanged(int index)
{
	if (index == -1)
		return;

	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	logTableModel->getFrameData(lastRow, phyFrame, &cmdFrame, &pmState);
	const TASLogger::ReaderObjectMove &obj = phyFrame.objectMoveList.at(index);

	objPullText->setText(obj.pull ? QStringLiteral("Pull") : QStringLiteral("Push"));
	objVelXText->setText(QString::number(obj.velocity[0]));
	objVelYText->setText(QString::number(obj.velocity[1]));
	objVelZText->setText(QString::number(obj.velocity[2]));
	objVelHText->setText(QString::number(std::hypot(obj.velocity[0], obj.velocity[1])));
	if (obj.velocity[0] == 0.0 && obj.velocity[1] == 0.0)
		objVelAngText->setText(NotAppl);
	else {
		const float ang = std::atan2(obj.velocity[1], obj.velocity[0]) * 180 / M_PI;
		objVelAngText->setText(DegreesFormat.arg(ang));
	}
	objPosXText->setText(QString::number(obj.position[0]));
	objPosYText->setText(QString::number(obj.position[1]));
	objPosZText->setText(QString::number(obj.position[2]));
}

void FrameInspectorWindow::damageListCurrentRowChanged(int index)
{
	if (index == -1)
		return;

	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	logTableModel->getFrameData(lastRow, phyFrame, &cmdFrame, &pmState);
	const TASLogger::ReaderDamage &dmg = phyFrame.damageList.at(index);

	const float distance = std::sqrt(
		dmg.direction[0] * dmg.direction[0]
		+ dmg.direction[1] * dmg.direction[1]
		+ dmg.direction[2] * dmg.direction[2]
	);

	dmgAmountText->setText(QString::number(dmg.damage));

	if (dmg.direction[0] == 0.0 && dmg.direction[1] == 0.0)
		dmgDirYawText->setText(NotAppl);
	else {
		const float yaw = std::atan2(dmg.direction[1], dmg.direction[0]) * 180 / M_PI;
		dmgDirYawText->setText(DegreesFormat.arg(yaw));
	}

	if (dmg.direction[0] == 0.0 && dmg.direction[1] == 0.0 && dmg.direction[2] == 0.0)
		dmgDirPitchText->setText(NotAppl);
	else {
		const float pitch = -std::asin(dmg.direction[2] / distance) * 180 / M_PI;
		dmgDirPitchText->setText(DegreesFormat.arg(pitch));
	}

	QStringList typeList;
	int bits = dmg.damageBits;
	for (auto it = DamageStringTable.cbegin(); it != DamageStringTable.cend(); ++it) {
		if (bits & it->first) {
			bits &= ~it->first;
			typeList << it->second;
		}
	}
	if (bits)
		typeList << QStringLiteral("Others");
	dmgTypeText->setText(typeList.isEmpty() ? QStringLiteral("Generic")
		: typeList.join(QStringLiteral(", ")));
}

void FrameInspectorWindow::colListCurrentRowChanged(int index)
{
	if (index == -1)
		return;

	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	logTableModel->getFrameData(lastRow, phyFrame, &cmdFrame, &pmState);
	const TASLogger::ReaderCollision &col = cmdFrame->collisionList.at(index);

	colEntityText->setText(!col.entity ? QStringLiteral("worldspawn")
		: QString::number(col.entity));

	if (col.impactVelocity[0] == 0.0 && col.impactVelocity[1] == 0.0
		&& col.impactVelocity[2] == 0.0)
		colImpactAngText->setText(NotAppl);
	else {
		const double ivel[3] = {
			-col.impactVelocity[0], -col.impactVelocity[1], -col.impactVelocity[2]
		};
		const double dp = ivel[0] * col.normal[0]
			+ ivel[1] * col.normal[1]
			+ ivel[2] * col.normal[2];
		const double impactMag = std::sqrt(
			ivel[0] * ivel[0]
			+ ivel[1] * ivel[1]
			+ ivel[2] * ivel[2]
		);
		const float ang = std::acos(dp / impactMag) * 180 / M_PI;
		colImpactAngText->setText(DegreesFormat.arg(ang));
	}

	if (col.normal[0] == 0.0 && col.normal[1] == 0.0)
		colNYawText->setText(NotAppl);
	else {
		const float yaw = std::atan2(col.normal[1], col.normal[0]) * 180 / M_PI;
		colNYawText->setText(DegreesFormat.arg(yaw));
	}

	const float pitch = -std::asin(col.normal[2]) * 180 / M_PI;
	colNPitchText->setText(DegreesFormat.arg(pitch));
}

void FrameInspectorWindow::closeEvent(QCloseEvent *event)
{
	emit aboutToClose();
	event->accept();
}

void FrameInspectorWindow::showEvent(QShowEvent *event)
{
	inspectFrame();
	event->accept();
}
