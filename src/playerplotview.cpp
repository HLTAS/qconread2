#include "playerplotview.hpp"

PlayerPlotView::PlayerPlotView(QWidget *parent)
	: QGraphicsView(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setRenderHints(QPainter::Antialiasing);
}

void PlayerPlotView::resizeEvent(QResizeEvent *event)
{
	fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(event);
}
