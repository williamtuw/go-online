#pragma once

#include <QWidget>
#include "ui_Gameinvate.h"

class Gameinvate : public QWidget
{
	Q_OBJECT

public:
	Gameinvate(QWidget *parent = Q_NULLPTR);
	~Gameinvate();
	Ui::Gameinvate ui;
	QPixmap bg;
	void paintEvent(QPaintEvent *event);
};
