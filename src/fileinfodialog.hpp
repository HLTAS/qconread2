#pragma once

#include <QtWidgets>
#include "logtablemodel.hpp"

class FileInfoDialog : public QDialog
{
	Q_OBJECT

public:
	FileInfoDialog(QWidget *parent, const LogTableModel *model);

public slots:
	void updateFileInfo(bool validFile);

private:
	const LogTableModel *logTableModel;

	QLabel *toolVersionText;
	QLabel *buildNumberText;
	QLabel *gameModText;

	void setupUi();
};
