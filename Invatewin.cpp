#include "Invatewin.h"
#include"QPaintEvent"
#include"QPainter"
Invatewin::Invatewin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	bg.load("Resources/bg.jpg");
}

Invatewin::~Invatewin()
{
}
void Invatewin::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(QRect(0, 0, width(), height()), bg);
}