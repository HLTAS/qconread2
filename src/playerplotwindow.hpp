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

	PlayerPlotView *planView;
	PlayerPlotView *frontView;
	PlayerPlotView *sideView;

	QGraphicsScene *planScene;
	QGraphicsScene *frontScene;
	QGraphicsScene *sideScene;

	QList<QGraphicsItem *> planLineList;
	QList<QGraphicsItem *> frontLineList;
	QList<QGraphicsItem *> sideLineList;

	void setupUi();
	void setupPens();
	void drawAxes(QGraphicsScene *scene, const QString &xLabel, const QString &yLabel);
	void drawLinesOnScenes(const double line[3], const QPen &pen);
	void clearPlot(QGraphicsScene *scene, QList<QGraphicsItem *> &lineList);
};
