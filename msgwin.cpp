#include "msgwin.h"
#include"QPainter"
msgwin::msgwin(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	pm.load("Resources/bg.jpg");;
}

msgwin::~msgwin()
{
}
void msgwin::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawPixmap(QRect(0, 0, width(), height()), pm);
}