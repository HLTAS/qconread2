#include "fileinfodialog.hpp"

FileInfoDialog::FileInfoDialog(QWidget *parent, const LogTableModel *model)
	: QDialog(parent), logTableModel(model)
{
	setupUi();
}

void FileInfoDialog::setupUi()
{
	QGridLayout *gl = new QGridLayout(this);
	gl->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(gl);

	QLabel *lblToolVersion = new QLabel("Tool version:", this);
	gl->addWidget(lblToolVersion, 0, 0, Qt::AlignRight);
	txtToolVersion = new QLabel(this);
	txtToolVersion->setTextInteractionFlags(Qt::TextSelectableByMouse);
	gl->addWidget(txtToolVersion, 0, 1);

	QLabel *lblBuildNumber = new QLabel("Game build:", this);
	gl->addWidget(lblBuildNumber, 1, 0, Qt::AlignRight);
	txtBuildNumber = new QLabel(this);
	txtBuildNumber->setTextInteractionFlags(Qt::TextSelectableByMouse);
	gl->addWidget(txtBuildNumber, 1, 1);

	QLabel *lblGameMod = new QLabel("Game mod:", this);
	gl->addWidget(lblGameMod, 2, 0, Qt::AlignRight);
	txtGameMod = new QLabel(this);
	txtGameMod->setTextInteractionFlags(Qt::TextSelectableByMouse);
	gl->addWidget(txtGameMod, 2, 1);

	setWindowTitle("Log File Info");
}

void FileInfoDialog::updateFileInfo(bool validFile)
{
	if (validFile) {
		txtToolVersion->setText(logTableModel->toolVersion());
		txtBuildNumber->setText(QString::number(logTableModel->buildNumber()));
		txtGameMod->setText(logTableModel->gameMod());
	} else {
		txtToolVersion->setText(QStringLiteral(""));
		txtBuildNumber->setText(QStringLiteral(""));
		txtGameMod->setText(QStringLiteral(""));
	}
}
