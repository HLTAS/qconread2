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

	QLabel *toolVersionLabel = new QLabel("Tool version:", this);
	gl->addWidget(toolVersionLabel, 0, 0, Qt::AlignRight);
	toolVersionText = new QLabel(this);
	toolVersionText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	gl->addWidget(toolVersionText, 0, 1);

	QLabel *buildNumberLabel = new QLabel("Game build:", this);
	gl->addWidget(buildNumberLabel, 1, 0, Qt::AlignRight);
	buildNumberText = new QLabel(this);
	buildNumberText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	gl->addWidget(buildNumberText, 1, 1);

	QLabel *gameModLabel = new QLabel("Game mod:", this);
	gl->addWidget(gameModLabel, 2, 0, Qt::AlignRight);
	gameModText = new QLabel(this);
	gameModText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	gl->addWidget(gameModText, 2, 1);

	setWindowTitle("Log File Info");
}

void FileInfoDialog::updateFileInfo(bool validFile)
{
	if (validFile) {
		toolVersionText->setText(logTableModel->toolVersion());
		buildNumberText->setText(QString::number(logTableModel->buildNumber()));
		gameModText->setText(logTableModel->gameMod());
	} else {
		toolVersionText->setText(QString());
		buildNumberText->setText(QString());
		gameModText->setText(QString());
	}
}
