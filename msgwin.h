#pragma once

#include <QWidget>
#include "ui_msgwin.h"

class msgwin : public QWidget
{
	Q_OBJECT

public:
	msgwin(QWidget *parent = Q_NULLPTR);
	~msgwin();
	QPixmap pm;
	void paintEvent(QPaintEvent *event);
	Ui::msgwin ui;
};
