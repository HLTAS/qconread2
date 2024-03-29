#pragma once

#include <QtWidgets>
#include "taslogger/reader.hpp"

enum LogFileError
{
	LFErrorNone,
	LFErrorCannotOpen,
	LFErrorInvalidLogFile
};

const int IN_ATTACK = 1 << 0;
const int IN_JUMP = 1 << 1;
const int IN_DUCK = 1 << 2;
const int IN_FORWARD = 1 << 3;
const int IN_BACK = 1 << 4;
const int IN_USE = 1 << 5;
const int IN_LEFT = 1 << 7;
const int IN_RIGHT = 1 << 8;
const int IN_MOVELEFT = 1 << 9;
const int IN_MOVERIGHT = 1 << 10;
const int IN_ATTACK2 = 1 << 11;
const int IN_RELOAD = 1 << 13;

enum HorizontalHeaderIndex {
	PhysicsFrameTimeHeader = 0,
	CommandFrameTimeHeader,
	FramebulkIdHeader,
	HorizontalSpeedHeader,
	VelocityAngleHeader,
	VerticalSpeedHeader,
	OnGroundHeader,
	DuckStateHeader,
	JumpHeader,
	DuckHeader,
	ForwardMoveHeader,
	SideMoveHeader,
	UpMoveHeader,
	YawHeader,
	PitchHeader,
	HealthHeader,
	ArmorHeader,
	UseHeader,
	AttackHeader,
	Attack2Header,
	ReloadHeader,
	OnLadderHeader,
	WaterLevelHeader,
	ClientStateHeader,
	FrameTimeRemainderHeader,
	EntityFrictionHeader,
	EntityGravityHeader,
	SharedSeedHeader,
	NonSharedRNGParameterHeader,
	PositionZHeader,
	PositionXHeader,
	PositionYHeader,
};

static const QString HorizontalHeaderList[][2] = {
	{"ft", "Physics frametime"},
	{"ms", "Command frametime"},
	{"bid", "Framebulk ID"},
	{"hspd", "Horizontal speed"},
	{"vyaw", "Velocity yaw"},
	{"vspd", "Vertical speed"},
	{"g", "Is on ground?"},
	{"k", "Duck state"},
	{"j", "Jump key"},
	{"d", "Duck key"},
	{"f", "Forward move"},
	{"s", "Side move"},
	{"u", "Up move"},
	{"y", "Yaw"},
	{"p", "Pitch"},
	{"hp", "Health"},
	{"ap", "Armor"},
	{"e", "Use key"},
	{"1", "Primary attack key"},
	{"2", "Second attack key"},
	{"r", "Reload key"},
	{"l", "Is on ladder?"},
	{"w", "Water level"},
	{"c", "Client state"},
	{"rem", "Frametime remainder"},
	{"ef", "Entity friction"},
	{"eg", "Entity gravity"},
	{"ss", "Shared seed"},
	{"idum", "Non-shared RNG parameter"},
	{"posz", "z position"},
	{"posx", "x position"},
	{"posy", "y position"},
};

static const int HorizontalHeaderCount =
	sizeof(HorizontalHeaderList) / sizeof(HorizontalHeaderList[0]);

class LogTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	LogTableModel(QObject *parent = nullptr);

	inline const TASLogger::TASLog &getTASLog() const { return _tasLog; }
	inline QString logFileName() const { return _logFileName; }
	inline QString toolVersion() const { return QString::fromStdString(_tasLog.toolVersion); }
	inline int buildNumber() const { return _tasLog.buildNumber; }
	inline QString gameMod() const { return QString::fromStdString(_tasLog.gameMod); }

	LogFileError openLogFile(const QString &fileName);
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

	void getFrameData(int row,
		TASLogger::ReaderPhysicsFrame &phyFrame,
		TASLogger::ReaderCommandFrame **cmdFrame,
		TASLogger::ReaderPlayerState **pmState) const;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const override;

	void setShowPlayerMove(bool pre);

	void setShowAnglemodUnit(bool enable);
	inline bool showAnglemodUnit() const { return _showAnglemodUnit; }

	void setShowFSUValues(bool enable);
	inline bool showFSUValues() const { return _showFSUValues; }

	void setHideMostCommonFrameTimes(bool enable);
	inline bool hideMostCommonFrameTimes() const { return _hideMostCommonFrameTimes; }
	inline float mostCommonFrameTimes() const { return _mostCommonFrameTimes; }
	inline float mostCommonMsec() const { return _mostCommonMsec; }

	inline const TASLogger::TASLog &tasLog() const { return _tasLog; }

signals:
	void logFileLoaded(bool loaded);

private:
	TASLogger::TASLog _tasLog;
	QVector<int> commandToPhysicsIndex;
	bool logLoaded = false;
	bool showPrePlayerMove = false;
	bool _showAnglemodUnit = false;
	bool _showFSUValues = false;
	bool _hideMostCommonFrameTimes = false;

	bool mostCommonFrameTimesOutdated = true;
	float _mostCommonFrameTimes;
	int _mostCommonMsec;
	QString _logFileName;

	void signalAllDataChanged();

	int searchBaseCommandRow(int phyIndex, int row) const;
	void findMostCommonFrameTimes();

	void populateCommandToPhysicsIndex();
	QVariant dataForeground(int row, int column) const;
	QVariant dataBackground(int row, int column) const;
	QVariant dataDisplay(int row, int column) const;
	QVariant dataFont(int row, int column) const;
	QVariant dataAlignment(int row, int column) const;
};
