#pragma once

#include <QAbstractTableModel>
#include "taslogger/reader.hpp"

class LogTableModel : public QAbstractTableModel
{
public:
	LogTableModel(QObject *parent = nullptr);

	void openLogFile(const QString &fileName);

	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
	TASLogger::TASLog tasLog;
	bool logLoaded;
};
