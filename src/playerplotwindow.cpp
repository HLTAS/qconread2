#include "playerplotwindow.hpp"
#include "settings.hpp"

static const double AxisLength = 150;
static const double CircleDistance = 100;
static const double AxisLabelOffset = 5;
static const double LineLength = AxisLength / 2;
static const double LinePenWidth = 4;
static const double ImagLinePenWidth = 1;

static const QPointF PlanViewPos(-CircleDistance, CircleDistance);
static const QPointF FrontViewPos(-CircleDistance, -CircleDistance);
static const QPointF SideViewPos(CircleDistance, -CircleDistance);
static const QPointF FictitiousViewPos(CircleDistance, CircleDistance);

PlayerPlotWindow::PlayerPlotWindow(QWidget *parent, const LogTableModel *model)
	: QWidget(parent), logTableModel(model)
{
	setupPens();
	setupUi();
}

void PlayerPlotWindow::setupPens()
{
	yawPen.setBrush(Qt::magenta);
	yawImagPen.setColor(QColor(211, 169, 228));
	velocityPen.setBrush(Qt::blue);
	velocityImagPen.setColor(QColor(177, 178, 231));
	damagePen.setBrush(Qt::red);
	damageImagPen.setColor(QColor(255, 200, 186));
	collisionPen.setBrush(Qt::darkYellow);
	collisionImagPen.setColor(Qt::darkYellow);

	yawPen.setWidth(LinePenWidth);
	yawImagPen.setWidth(ImagLinePenWidth);
	velocityPen.setWidth(LinePenWidth);
	velocityImagPen.setWidth(ImagLinePenWidth);
	damagePen.setWidth(LinePenWidth);
	damageImagPen.setWidth(ImagLinePenWidth);
	collisionPen.setWidth(LinePenWidth);
	collisionImagPen.setWidth(ImagLinePenWidth);

	yawPen.setCosmetic(true);
	yawImagPen.setCosmetic(true);
	velocityPen.setCosmetic(true);
	velocityImagPen.setCosmetic(true);
	damagePen.setCosmetic(true);
	damageImagPen.setCosmetic(true);
	collisionPen.setCosmetic(true);
	collisionImagPen.setCosmetic(true);
}

void PlayerPlotWindow::setupUi()
{
	setWindowTitle("Player Plot");
	setWindowFlags(Qt::Tool);

	QGridLayout *lay = new QGridLayout(this);
	setLayout(lay);

	plotView = new PlayerPlotView(this);
	lay->addWidget(plotView, 0, 0);

	plotScene = new QGraphicsScene(this);
	plotView->setScene(plotScene);

	drawAxes(PlanViewPos, "X", "Y", true);
	drawAxes(FrontViewPos, "X", "Z", true);
	drawAxes(SideViewPos, "Y", "Z", true);
	drawAxes(FictitiousViewPos, "", "", false);
}

void PlayerPlotWindow::closeEvent(QCloseEvent *event)
{
	emit aboutToClose();
	event->ignore();
	hide();
}

void PlayerPlotWindow::hideEvent(QHideEvent *event)
{
	QSettings settings;
	settings.setValue(PlayerPlotGeometryKey, saveGeometry());

	event->accept();
}

void PlayerPlotWindow::showEvent(QShowEvent *event)
{
	QSettings settings;
	restoreGeometry(settings.value(PlayerPlotGeometryKey).toByteArray());

	event->accept();
}


void PlayerPlotWindow::drawAxes(const QPointF &pos, const QString &xLabel,
	const QString &yLabel, bool drawCircle)
{
	QPen axesPen;
	axesPen.setCosmetic(true);
	axesPen.setColor(QColor(Qt::gray));

	QGraphicsLineItem *xAxisItem = plotScene->addLine(
		-AxisLength / 2, 0, AxisLength / 2, 0, axesPen);
	QGraphicsLineItem *yAxisItem = plotScene->addLine(
		0, -AxisLength / 2, 0, AxisLength / 2, axesPen);

	QGraphicsSimpleTextItem *xLabelItem = plotScene->addSimpleText(xLabel);
	xLabelItem->setPos(AxisLength / 2 + AxisLabelOffset, -xLabelItem->boundingRect().height() / 2);

	QGraphicsSimpleTextItem *yLabelItem = plotScene->addSimpleText(yLabel);
	yLabelItem->setPos(-yLabelItem->boundingRect().width() / 2,
		-AxisLength / 2 - yLabelItem->boundingRect().height() - AxisLabelOffset);

	QList<QGraphicsItem *> items({
		xAxisItem, yAxisItem, xLabelItem, yLabelItem
	});

	if (drawCircle) {
		QGraphicsEllipseItem *circleItem = plotScene->addEllipse(
			-AxisLength / 2, -AxisLength / 2, AxisLength, AxisLength, axesPen);
		items.append(circleItem);
	}

	QGraphicsItemGroup *group = plotScene->createItemGroup(items);
	group->setPos(pos);
}

void PlayerPlotWindow::clearPlot()
{
	for (QGraphicsItem *item : plotItemList) {
		plotScene->removeItem(item);
		delete item;
	}
	plotItemList.clear();
}

void PlayerPlotWindow::drawLinesOnScenes(const double line[3],
	const QPen &pen, const QPen &lightPen)
{
	const QPointF planPoint(line[0] + PlanViewPos.x(), -line[1] + PlanViewPos.y());
	const QPointF frontPoint(line[0] + FrontViewPos.x(), -line[2] + FrontViewPos.y());
	const QPointF sidePoint(line[1] + SideViewPos.x(), -line[2] + SideViewPos.y());
	const QPointF planFicPoint(FictitiousViewPos.x(), -line[1] + FictitiousViewPos.y());
	const QPointF sideFicPoint(line[1] + FictitiousViewPos.x(), FictitiousViewPos.y());

	QGraphicsLineItem *planLineItem = plotScene->addLine(QLineF(PlanViewPos, planPoint), pen);
	QGraphicsLineItem *frontLineItem = plotScene->addLine(QLineF(FrontViewPos, frontPoint), pen);
	QGraphicsLineItem *sideLineItem = plotScene->addLine(QLineF(SideViewPos, sidePoint), pen);

	QGraphicsLineItem *pfLineItem = plotScene->addLine(QLineF(planPoint, frontPoint), lightPen);
	QGraphicsLineItem *fsLineItem = plotScene->addLine(QLineF(frontPoint, sidePoint), lightPen);

	QGraphicsLineItem *planImgLineItem = plotScene->addLine(
		QLineF(planPoint, planFicPoint), lightPen);
	QGraphicsLineItem *sideImgLineItem = plotScene->addLine(
		QLineF(sidePoint, sideFicPoint), lightPen);
	QGraphicsEllipseItem *imgArc = plotScene->addEllipse(
		FictitiousViewPos.x() - line[1], FictitiousViewPos.y() - line[1],
		line[1] * 2, line[1] * 2,
		lightPen
	);

	plotItemList.append(planLineItem);
	plotItemList.append(frontLineItem);
	plotItemList.append(sideLineItem);
	plotItemList.append(pfLineItem);
	plotItemList.append(fsLineItem);
	plotItemList.append(planImgLineItem);
	plotItemList.append(sideImgLineItem);
	plotItemList.append(imgArc);
}

void PlayerPlotWindow::plotFrame(int row)
{
	clearPlot();

	if (row == -1)
		return;

	TASLogger::ReaderPhysicsFrame phyFrame;
	TASLogger::ReaderCommandFrame *cmdFrame;
	TASLogger::ReaderPlayerState *pmState;
	logTableModel->getFrameData(row, phyFrame, &cmdFrame, &pmState);

	if (!cmdFrame)
		return;

	{
		const double yaw = cmdFrame->viewangles[0] * M_PI / 180;
		const double line[3] = {
			std::cos(yaw) * LineLength,
			std::sin(yaw) * LineLength,
			0
		};

		drawLinesOnScenes(line, yawPen, yawImagPen);
	}

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

		drawLinesOnScenes(line, velocityPen, velocityImagPen);
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

		drawLinesOnScenes(line, damagePen, damageImagPen);
	}

	for (const TASLogger::ReaderCollision &col : cmdFrame->collisionList) {
		if (col.normal[0] == 0.0 && col.normal[1] == 0.0 && col.normal[2] == 0.0)
			continue;

		const double line[3] = {
			col.normal[0] * LineLength,
			col.normal[1] * LineLength,
			col.normal[2] * LineLength
		};

		drawLinesOnScenes(line, collisionPen, collisionImagPen);
	}
}
