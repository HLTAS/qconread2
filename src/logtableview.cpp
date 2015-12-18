#include "logtableview.hpp"

LogTableView::LogTableView(QWidget *parent)
	: QTableView(parent)
{
}

void LogTableView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	emit currentChanged_(current, previous);
	QTableView::currentChanged(current, previous);
}
