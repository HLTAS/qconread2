#include <cstdio>
#include <ctime>
#include <QDebug>
#include "logtablemodel.hpp"

LogTableModel::LogTableModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

void LogTableModel::openLogFile(const QString &fileName)
{
	const QByteArray nameBytes = fileName.toLatin1();
	FILE *file = fopen(nameBytes.data(), "rb");
	fseek(file, 0, SEEK_END);
	size_t fileSize = static_cast<size_t>(ftell(file));
	fseek(file, 0, SEEK_SET);
	char *fileData = new char[fileSize];
	size_t n = fread(fileData, 1, fileSize, file);
	fclose(file);

	rapidjson::ParseResult res = TASLogger::ParseString(fileData, tasLog);
	delete[] fileData;

	logLoaded = true;
	insertRows(0, static_cast<int>(tasLog.physicsFrameList.size()));
}

bool LogTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row + count);
	endInsertRows();
}

int LogTableModel::rowCount(const QModelIndex &parent) const
{
	if (logLoaded)
		return static_cast<int>(tasLog.physicsFrameList.size());
	else
		return 0;
}

int LogTableModel::columnCount(const QModelIndex &parent) const
{
	return 10;
}

QVariant LogTableModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case 0:
			return tasLog.physicsFrameList.at(index.row()).frameTime;
		case 1:
			return tasLog.physicsFrameList.at(index.row()).clientState;
		}
	}
	return QVariant();
}
