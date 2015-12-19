#pragma once

#include <QtWidgets>

class PlayerPlotView : public QGraphicsView
{
public:
	PlayerPlotView(QWidget *parent);

protected:
	void resizeEvent(QResizeEvent *event) override;
};
