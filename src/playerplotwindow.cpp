#include "playerplotwindow.hpp"

static const double AxisLength = 300;
static const double AxisLabelOffset = 5;
static const double LineLength = AxisLength / 2;
static const double LinePenWidth = 4;

PlayerPlotWindow::PlayerPlotWindow(QWidget *parent, const LogTableModel *model)
	: QWidget(parent), logTableModel(model)
{
	setupPens();
	setupUi();
}

void PlayerPlotWindow::setupPens()
{
	yawPen.setBrush(Qt::magenta);
	yawPen.setWidth(LinePenWidth);
	yawPen.setCosmetic(true);

	velocityPen.setBrush(Qt::blue);
	velocityPen.setWidth(LinePenWidth);
	velocityPen.setCosmetic(true);

	damagePen.setBrush(Qt::red);
	damagePen.setWidth(LinePenWidth);
	damagePen.setCosmetic(true);

	collisionPen.setBrush(Qt::darkYellow);
	collisionPen.setWidth(LinePenWidth);
	collisionPen.setCosmetic(true);
}

void PlayerPlotWindow::setupUi()
{
	setWindowTitle("Player Plot");
	setWindowFlags(Qt::Tool);

	QGridLayout *lay = new QGridLayout(this);
	setLayout(lay);

	planView = new PlayerPlotView(this);
	frontView = new PlayerPlotView(this);
	sideView = new PlayerPlotView(this);

	planScene = new QGraphicsScene(this);
	frontScene = new QGraphicsScene(this);
	sideScene = new QGraphicsScene(this);

	drawAxes(planScene, "X", "Y");
	drawAxes(frontScene, "X", "Z");
	drawAxes(sideScene, "Y", "Z");

	planView->setScene(planScene);
	frontView->setScene(frontScene);
	sideView->setScene(sideScene);

	lay->addWidget(frontView, 0, 0);
	lay->addWidget(sideView, 0, 1);
	lay->addWidget(planView, 1, 0);
}

void PlayerPlotWindow::closeEvent(QCloseEvent *event)
{
	emit aboutToClose();
	event->accept();
}

void PlayerPlotWindow::drawAxes(QGraphicsScene *scene, const QString &xLabel, const QString &yLabel)
{
	QPen axesPen;
	axesPen.setCosmetic(true);
	axesPen.setColor(QColor(Qt::gray));
	scene->addLine(-AxisLength / 2, 0, AxisLength / 2, 0, axesPen);
	scene->addLine(0, -AxisLength / 2, 0, AxisLength / 2, axesPen);
	scene->addEllipse(-AxisLength / 2, -AxisLength / 2, AxisLength, AxisLength, axesPen);

	QGraphicsSimpleTextItem *textItem = scene->addSimpleText(xLabel);
	textItem->setPos(AxisLength / 2 + AxisLabelOffset, -textItem->boundingRect().height() / 2);

	textItem = scene->addSimpleText(yLabel);
	textItem->setPos(-textItem->boundingRect().width() / 2,
		-AxisLength / 2 - textItem->boundingRect().height() - AxisLabelOffset);
}

void PlayerPlotWindow::clearPlot(QGraphicsScene *scene, QList<QGraphicsItem *> &lineList)
{
	for (QGraphicsItem *item : lineList) {
		scene->removeItem(item);
		delete item;
	}
	lineList.clear();
}

void PlayerPlotWindow::drawLinesOnScenes(const double line[3], const QPen &pen)
{
	planLineList.append(planScene->addLine(0, 0, line[0], -line[1], pen));
	frontLineList.append(frontScene->addLine(0, 0, line[0], -line[2], pen));
	sideLineList.append(sideScene->addLine(0, 0, line[1], -line[2], pen));
}

void PlayerPlotWindow::plotFrame(int row)
{
	clearPlot(planScene, planLineList);
	clearPlot(frontScene, frontLineList);
	clearPlot(sideScene, sideLineList);

	if (row == -1)
		return;

	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	logTableModel->getFrameData(row, phyFrame, &cmdFrame, &pmState);

	if (!cmdFrame)
		return;

	if (pmState->velocity[0] != 0.0 || pmState->velocity[1] != 0.0 || pmState->velocity[2] != 0.0) {
		const double speed = std::sqrt(
			pmState->velocity[0] * pmState->velocity[0]
			+ pmState->velocity[1] * pmState->velocity[1]
			+ pmState->velocity[2] * pmState->velocity[2]
		);

		const double scale = LineLength / speed;
		const double line[3] = {
			pmState->velocity[0] * scale,
			pmState->velocity[1] * scale,
			pmState->velocity[2] * scale
		};

		drawLinesOnScenes(line, velocityPen);
	}

	{
		const double yaw = cmdFrame->viewangles[0] * M_PI / 180;
		const double line[3] = {
			std::cos(yaw) * LineLength,
			std::sin(yaw) * LineLength,
			0
		};

		drawLinesOnScenes(line, yawPen);
	}

	for (const TASLogger::ReaderDamage &dmg : phyFrame.damageList) {
		if (dmg.direction[0] == 0.0 && dmg.direction[1] == 0.0 && dmg.direction[2] == 0.0)
			continue;

		const double distance = std::sqrt(
			dmg.direction[0] * dmg.direction[0]
			+ dmg.direction[1] * dmg.direction[1]
			+ dmg.direction[2] * dmg.direction[2]
		);

		const double scale = LineLength / distance;
		const double line[3] = {
			dmg.direction[0] * scale,
			dmg.direction[1] * scale,
			dmg.direction[2] * scale
		};

		drawLinesOnScenes(line, damagePen);
	}

	for (const TASLogger::ReaderCollision &col : cmdFrame->collisionList) {
		if (col.normal[0] == 0.0 && col.normal[1] == 0.0 && col.normal[2] == 0.0)
			continue;

		const double line[3] = {
			col.normal[0] * LineLength,
			col.normal[1] * LineLength,
			col.normal[2] * LineLength
		};

		drawLinesOnScenes(line, collisionPen);
	}
}
