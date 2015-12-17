#include <cstdio>
#include <QDebug>
#include <QFont>
#include <QColor>
#include <cmath>
#include <limits>
#include "logtablemodel.hpp"

static const float M_U = 360.0 / 65536;

LogTableModel::LogTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

void LogTableModel::populateCommandToPhysicsIndex()
{
	commandToPhysicsIndex.clear();
	commandToPhysicsIndex.reserve(tasLog.physicsFrameList.size());
	for (size_t phy = 0; phy < tasLog.physicsFrameList.size(); phy++) {
		const auto &f = tasLog.physicsFrameList.at(phy);
		commandToPhysicsIndex.append(phy);
		for (size_t j = 1; j < f.commandFrameList.size(); j++)
			commandToPhysicsIndex.append(phy);
	}
}

void LogTableModel::openLogFile(const QString &fileName)
{
	_logFileName = fileName;

	const QByteArray nameBytes = fileName.toLatin1();
	FILE *file = fopen(nameBytes.data(), "rb");
	if (!file) {
		// TODO: error reporting
		return;
	}

	const rapidjson::ParseResult res = TASLogger::ParseFile(file, tasLog);
	fclose(file);

	if (!res) {
		// TODO: error reporting
		return;
	}

	removeRows(0, commandToPhysicsIndex.size());

	populateCommandToPhysicsIndex();

	logLoaded = true;
	insertRows(0, commandToPhysicsIndex.size());
}

bool LogTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row + count - 1);
	endInsertRows();
	return true;
}

bool LogTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	endRemoveRows();
	return true;
}

int LogTableModel::rowCount(const QModelIndex &) const
{
	if (logLoaded)
		return commandToPhysicsIndex.size();
	else
		return 0;
}

int LogTableModel::columnCount(const QModelIndex &) const
{
	return HorizontalHeaderCount;
}

// Search for the row index corresponding to the lowest command frame within a physics frame.
// Usually this takes only a few iterations, so it is almost constant time.
int LogTableModel::searchBaseCommandRow(int phyIndex, int row) const
{
	while (row >= 0) {
		const int ind = commandToPhysicsIndex.at(row);
		if (ind < phyIndex)
			return row + 1;
		--row;
	}
	return 0;
}

void LogTableModel::signalAllDataChanged()
{
	const QModelIndex topLeft = createIndex(0, 0);
	const QModelIndex bottomRight = createIndex(rowCount() - 1, columnCount() - 1);
	emit dataChanged(topLeft, bottomRight);
}

void LogTableModel::setShowPlayerMove(bool pre)
{
	showPrePlayerMove = pre;
	signalAllDataChanged();
}

void LogTableModel::setShowAnglemodUnit(bool enable)
{
	showAnglemodUnit = enable;
	signalAllDataChanged();
}

void LogTableModel::getAllFrames(int row,
	TASLogger::ReaderPhysicsFrame &phyFrame,
	TASLogger::ReaderCommandFrame **cmdFrame,
	TASLogger::ReaderPlayerState **pmState) const
{
	const int ind = commandToPhysicsIndex.at(row);
	const int baseRow = searchBaseCommandRow(ind, row);
	const int cmdInd = row - baseRow;

	phyFrame = tasLog.physicsFrameList.at(ind);
	*cmdFrame = nullptr;
	*pmState = nullptr;
	if (!phyFrame.commandFrameList.empty()) {
		*cmdFrame = &phyFrame.commandFrameList.at(cmdInd);
		*pmState = &(showPrePlayerMove ? (*cmdFrame)->prePMState : (*cmdFrame)->postPMState);
	}
}

QVariant LogTableModel::dataForeground(int row, int column) const
{
	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	getAllFrames(row, phyFrame, &cmdFrame, &pmState);

	switch (column) {
	case PhysicsFrameTimeHeader:
		return QColor(phyFrame.consolePrintList.empty() ? Qt::darkGray : Qt::white);
	case CommandFrameTimeHeader:
		return QColor(Qt::darkGray);
	case FramebulkIdHeader:
		return QColor(Qt::darkGray);
	case HorizontalSpeedHeader:
		if (phyFrame.objectMoveList.empty())
			break;
		return QColor(Qt::blue);
	case VerticalSpeedHeader:
		if (!cmdFrame || pmState->velocity[2] == 0.0)
			break;
		return QColor(pmState->velocity[2] > 0.0 ? Qt::blue : Qt::red);
	case ForwardMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[0] == 0.0)
			break;
		return QColor(Qt::white);
	case SideMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[1] == 0.0)
			break;
		return QColor(Qt::white);
	case UpMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[2] == 0.0)
			break;
		return QColor(Qt::white);
	case HealthHeader:
		if (phyFrame.damageList.empty())
			break;
		return QColor(Qt::white);
	case ArmorHeader:
		if (phyFrame.damageList.empty())
			break;
		return QColor(Qt::white);
	case FrameTimeRemainderHeader:
		if (!cmdFrame)
			break;
		return QColor(Qt::darkGray);
	}

	return QVariant();
}

QVariant LogTableModel::dataBackground(int row, int column) const
{
	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	getAllFrames(row, phyFrame, &cmdFrame, &pmState);

	switch (column) {
	case PhysicsFrameTimeHeader:
		if (phyFrame.consolePrintList.empty())
			break;
		return QColor(Qt::darkGray);
	case HorizontalSpeedHeader:
	case VerticalSpeedHeader:
		if (!cmdFrame || cmdFrame->collisionList.empty())
			break;
		return QColor(255, 233, 186);
	case OnGroundHeader:
		if (!cmdFrame || !pmState->onGround)
			break;
		return QColor(Qt::green);
	case DuckStateHeader:
		if (!cmdFrame || pmState->duckState == TASLogger::UNDUCKED)
			break;
		return QColor(pmState->duckState == TASLogger::INDUCK ? Qt::gray : Qt::black);
	case JumpHeader:
		if (!cmdFrame || !(cmdFrame->buttons & IN_JUMP))
			break;
		return QColor(Qt::cyan);
	case DuckHeader:
		if (!cmdFrame || !(cmdFrame->buttons & IN_DUCK))
			break;
		return QColor(Qt::magenta);
	case ForwardMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[0] == 0.0)
			break;
		return QColor(cmdFrame->FSU[0] > 0 ? Qt::blue : Qt::red);
	case SideMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[1] == 0.0)
			break;
		return QColor(cmdFrame->FSU[1] > 0 ? Qt::blue : Qt::red);
	case UpMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[2] == 0.0)
			break;
		return QColor(cmdFrame->FSU[2] > 0 ? Qt::blue : Qt::red);
	case YawHeader:
		if (!cmdFrame || cmdFrame->punchangles[0] == 0.0)
			break;
		return QColor(Qt::yellow);
	case PitchHeader:
		if (!cmdFrame || cmdFrame->punchangles[1] == 0.0)
			break;
		return QColor(Qt::yellow);
	case HealthHeader:
		if (phyFrame.damageList.empty())
			break;
		return QColor(Qt::red);
	case ArmorHeader:
		if (phyFrame.damageList.empty())
			break;
		return QColor(Qt::red);
	case UseHeader:
		if (!cmdFrame || !(cmdFrame->buttons & IN_USE))
			break;
		return QColor(Qt::darkYellow);
	case AttackHeader:
		if (!cmdFrame || !(cmdFrame->buttons & IN_ATTACK))
			break;
		return QColor(Qt::darkYellow);
	case Attack2Header:
		if (!cmdFrame || !(cmdFrame->buttons & IN_ATTACK2))
			break;
		return QColor(Qt::darkYellow);
	case ReloadHeader:
		if (!cmdFrame || !(cmdFrame->buttons & IN_RELOAD))
			break;
		return QColor(Qt::darkYellow);
	case OnLadderHeader:
		if (!cmdFrame || !pmState->onLadder)
			break;
		return QColor(125, 58, 19);
	case WaterLevelHeader:
		if (!cmdFrame || !pmState->waterLevel)
			break;
		return QColor(pmState->waterLevel == 1 ? Qt::blue : Qt::darkBlue);
	case EntityFrictionHeader:
		if (!cmdFrame || cmdFrame->entFriction == 1.0)
			break;
		return QColor(Qt::gray);
	case EntityGravityHeader:
		if (!cmdFrame || cmdFrame->entGravity == 1.0)
			break;
		return QColor(Qt::gray);
	}

	return QVariant();
}

QVariant LogTableModel::dataDisplay(int row, int column) const
{
	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	getAllFrames(row, phyFrame, &cmdFrame, &pmState);

	switch (column) {
	case PhysicsFrameTimeHeader:
		return phyFrame.frameTime;
	case CommandFrameTimeHeader:
		if (!cmdFrame)
			break;
		return cmdFrame->msec;
	case FramebulkIdHeader:
		if (!cmdFrame)
			break;
		return cmdFrame->framebulkId;
	case HorizontalSpeedHeader: {
		if (!cmdFrame)
			break;
		float speed = std::hypot(pmState->velocity[0], pmState->velocity[1]);
		return speed == 0.0 ? QVariant() : speed;
	}
	case VerticalSpeedHeader:
		if (!cmdFrame)
			break;
		return pmState->velocity[2] == 0.0 ? QVariant() : pmState->velocity[2];
	case ForwardMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[0] == 0.0)
			break;
		return cmdFrame->FSU[0] > 0 ? QStringLiteral("F") : QStringLiteral("B");
	case SideMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[1] == 0.0)
			break;
		return cmdFrame->FSU[1] > 0 ? QStringLiteral("R") : QStringLiteral("L");
	case UpMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[2] == 0.0)
			break;
		return cmdFrame->FSU[2] > 0 ? QStringLiteral("U") : QStringLiteral("D");
	case YawHeader:
		if (!cmdFrame)
			break;
		if (showAnglemodUnit)
			return QString("%1u").arg(cmdFrame->viewangles[0] / M_U);
		else
			return cmdFrame->viewangles[0];
	case PitchHeader:
		if (!cmdFrame)
			break;
		if (showAnglemodUnit)
			return QString("%1u").arg(cmdFrame->viewangles[1] / M_U);
		else
			return cmdFrame->viewangles[1];
	case HealthHeader:
		if (!cmdFrame)
			break;
		return cmdFrame->health;
	case ArmorHeader:
		if (!cmdFrame)
			break;
		return cmdFrame->armor;
	case FrameTimeRemainderHeader:
		if (!cmdFrame)
			break;
		return QString::number(cmdFrame->frameTimeRemainder, 'e', 3);
	case PositionZHeader:
		if (!cmdFrame)
			break;
		return pmState->position[2];
	case PositionXHeader:
		if (!cmdFrame)
			break;
		return pmState->position[0];
	case PositionYHeader:
		if (!cmdFrame)
			break;
		return pmState->position[1];
	}

	return QVariant();
}

QVariant LogTableModel::dataFont(int row, int column) const
{
	static const QFont boldFont = QFont(QStringLiteral(""), -1, QFont::Bold);

	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	getAllFrames(row, phyFrame, &cmdFrame, &pmState);

	switch (column) {
	case HorizontalSpeedHeader:
		if (phyFrame.objectMoveList.empty())
			break;
		return boldFont;
	case HealthHeader:
		if (phyFrame.damageList.empty())
			break;
		return boldFont;
	case ArmorHeader:
		if (phyFrame.damageList.empty())
			break;
		return boldFont;
	}

	return QVariant();
}

QVariant LogTableModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
		return dataDisplay(index.row(), index.column());
	else if (role == Qt::BackgroundRole)
		return dataBackground(index.row(), index.column());
	else if (role == Qt::ForegroundRole)
		return dataForeground(index.row(), index.column());
	else if (role == Qt::FontRole)
		return dataFont(index.row(), index.column());

	return QVariant();
}

QVariant LogTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal)
			return HorizontalHeaderList[section][0];
	} else if (role == Qt::TextAlignmentRole) {
		if (orientation == Qt::Vertical)
			return Qt::AlignRight;
	} else if (role == Qt::ToolTipRole) {
		if (orientation == Qt::Horizontal)
			return HorizontalHeaderList[section][1];
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}
