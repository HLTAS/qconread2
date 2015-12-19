#pragma once

#include <QtWidgets>
#include "logtablemodel.hpp"
#include "playerplotview.hpp"

class PlayerPlotWindow : public QWidget
{
	Q_OBJECT

public:
	PlayerPlotWindow(QWidget *parent, const LogTableModel *model);

	void plotFrame(int row);

signals:
	void aboutToClose();

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	const LogTableModel *logTableModel;

	QPen yawPen;
	QPen velocityPen;
	QPen damagePen;
	QPen collisionPen;

	QPen yawImagPen;
	QPen velocityImagPen;
	QPen damageImagPen;
	QPen collisionImagPen;

	PlayerPlotView *plotView;
	QGraphicsScene *plotScene;

	QList<QGraphicsItem *> plotItemList;

	void setupUi();
	void setupPens();
	void drawAxes(const QPointF &pos, const QString &xLabel,
		const QString &yLabel, bool drawCircle);
	void drawLinesOnScenes(const double line[3], const QPen &pen, const QPen &lightPen);
	void clearPlot();
};
