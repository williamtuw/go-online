#pragma once

#include <QWidget>
#include "ui_Invatewin.h"

class Invatewin : public QWidget
{
	Q_OBJECT

public:
	Invatewin(QWidget *parent = Q_NULLPTR);
	~Invatewin();
	QPixmap bg;
	void paintEvent(QPaintEvent *event);
	Ui::Invatewin ui;
};
