#pragma once

#include <QtWidgets>

class LogTableView : public QTableView
{
	Q_OBJECT

public:
	LogTableView(QWidget *parent = nullptr);

signals:
	void currentChanged_(const QModelIndex &current, const QModelIndex &previous);

protected:
	void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;
};
