#include <cstdio>
#include <cmath>
#include <limits>
#include "logtablemodel.hpp"

static const float M_U = 360.0 / 65536;
static const QString BaseVelFormat = "*%1";

LogTableModel::LogTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

void LogTableModel::populateCommandToPhysicsIndex()
{
	commandToPhysicsIndex.clear();
	commandToPhysicsIndex.reserve(_tasLog.physicsFrameList.size());
	for (size_t phy = 0; phy < _tasLog.physicsFrameList.size(); phy++) {
		const auto &f = _tasLog.physicsFrameList.at(phy);
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

	const rapidjson::ParseResult res = TASLogger::ParseFile(file, _tasLog);
	fclose(file);

	if (!res) {
		// TODO: error reporting
		return;
	}

	removeRows(0, commandToPhysicsIndex.size());

	populateCommandToPhysicsIndex();

	logLoaded = true;
	insertRows(0, commandToPhysicsIndex.size());

	emit logFileLoaded(true);
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
	_showAnglemodUnit = enable;
	signalAllDataChanged();
}

void LogTableModel::setShowFSUValues(bool enable)
{
	_showFSUValues = enable;
	signalAllDataChanged();
}

void LogTableModel::getFrameData(int row,
	TASLogger::ReaderPhysicsFrame &phyFrame,
	TASLogger::ReaderCommandFrame **cmdFrame,
	TASLogger::ReaderPlayerState **pmState) const
{
	const int ind = commandToPhysicsIndex.at(row);
	const int baseRow = searchBaseCommandRow(ind, row);
	const int cmdInd = row - baseRow;

	phyFrame = _tasLog.physicsFrameList.at(ind);
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
	getFrameData(row, phyFrame, &cmdFrame, &pmState);

	switch (column) {
	case PhysicsFrameTimeHeader:
		return QColor(phyFrame.consolePrintList.empty() ? Qt::darkGray : Qt::white);
	case CommandFrameTimeHeader:
		return QColor(Qt::darkGray);
	case FramebulkIdHeader:
		return QColor(Qt::darkGray);
	case HorizontalSpeedHeader:
	case VelocityAngleHeader:
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
	case SharedSeedHeader:
		return QColor(Qt::darkGray);
	case ClientStateHeader:
		if (phyFrame.paused)
			return QColor(Qt::white);
		else
			return QColor(phyFrame.clientState == 5 ? Qt::darkGray : Qt::red);
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
	getFrameData(row, phyFrame, &cmdFrame, &pmState);

	switch (column) {
	case PhysicsFrameTimeHeader:
		if (phyFrame.consolePrintList.empty())
			break;
		return QColor(Qt::darkGray);
	case HorizontalSpeedHeader:
	case VelocityAngleHeader:
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
	case ClientStateHeader:
		if (!phyFrame.paused)
			break;
		return QColor(Qt::darkCyan);
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
	getFrameData(row, phyFrame, &cmdFrame, &pmState);

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
		const bool hbasevelExist = pmState->baseVelocity[0] != 0.0
			|| pmState->baseVelocity[1] != 0.0;
		if (pmState->velocity[0] == 0.0 && pmState->velocity[1] == 0.0
			&& !hbasevelExist
			&& phyFrame.objectMoveList.empty())
			return QVariant();
		const float hspeed = std::hypot(pmState->velocity[0], pmState->velocity[1]);
		if (hbasevelExist)
			return BaseVelFormat.arg(hspeed);
		else
			return hspeed;
	}
	case VelocityAngleHeader:
		if (!cmdFrame || (pmState->velocity[0] == 0.0 && pmState->velocity[1] == 0.0))
			break;
		return std::atan2(pmState->velocity[1], pmState->velocity[0]) * 180 / M_PI;
	case VerticalSpeedHeader:
		if (!cmdFrame || (pmState->velocity[2] == 0.0 && pmState->baseVelocity[2] == 0.0))
			break;
		if (pmState->baseVelocity[2] != 0.0)
			return BaseVelFormat.arg(pmState->velocity[2]);
		else
			return pmState->velocity[2];
	case ForwardMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[0] == 0.0)
			break;
		if (_showFSUValues)
			return cmdFrame->FSU[0];
		else
			return cmdFrame->FSU[0] > 0 ? QStringLiteral("F") : QStringLiteral("B");
	case SideMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[1] == 0.0)
			break;
		if (_showFSUValues)
			return cmdFrame->FSU[1];
		else
			return cmdFrame->FSU[1] > 0 ? QStringLiteral("R") : QStringLiteral("L");
	case UpMoveHeader:
		if (!cmdFrame || cmdFrame->FSU[2] == 0.0)
			break;
		if (_showFSUValues)
			return cmdFrame->FSU[2];
		else
			return cmdFrame->FSU[2] > 0 ? QStringLiteral("U") : QStringLiteral("D");
	case YawHeader:
		if (!cmdFrame)
			break;
		if (_showAnglemodUnit)
			return QString("%1u").arg(cmdFrame->viewangles[0] / M_U);
		else
			return cmdFrame->viewangles[0];
	case PitchHeader:
		if (!cmdFrame)
			break;
		if (_showAnglemodUnit)
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
	case ClientStateHeader:
		return phyFrame.clientState;
	case FrameTimeRemainderHeader:
		if (!cmdFrame)
			break;
		return QString::number(cmdFrame->frameTimeRemainder, 'e', 3);
	case SharedSeedHeader:
		if (!cmdFrame)
			break;
		return cmdFrame->sharedSeed;
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
	static const QFont boldFont = QFont(QString(), -1, QFont::Bold);

	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	getFrameData(row, phyFrame, &cmdFrame, &pmState);

	switch (column) {
	case HorizontalSpeedHeader:
	case VelocityAngleHeader:
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
	case ClientStateHeader:
		if (phyFrame.clientState == 5)
			break;
		return boldFont;
	}

	return QVariant();
}

QVariant LogTableModel::dataAlignment(int, int column) const
{
	switch (column) {
	case ForwardMoveHeader:
	case SideMoveHeader:
	case UpMoveHeader:
		return Qt::AlignCenter;
	}

	return QVariant();
}

QVariant LogTableModel::data(const QModelIndex &index, int role) const
{
	switch (role) {
	case Qt::DisplayRole:
		return dataDisplay(index.row(), index.column());
	case Qt::BackgroundRole:
		return dataBackground(index.row(), index.column());
	case Qt::ForegroundRole:
		return dataForeground(index.row(), index.column());
	case Qt::FontRole:
		return dataFont(index.row(), index.column());
	case Qt::TextAlignmentRole:
		return dataAlignment(index.row(), index.column());
	}

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
