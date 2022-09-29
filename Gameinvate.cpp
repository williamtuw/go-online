#include "Gameinvate.h"
#include"QPaintEvent"
#include"QPainter"
Gameinvate::Gameinvate(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	bg.load("Resources/bg.jpg");
}

Gameinvate::~Gameinvate()
{
}
void Gameinvate::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(QRect(0, 0, width(), height()),bg);
}