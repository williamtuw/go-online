#pragma once

#include <QObject>
#include "QIcon"
class Gameplayer : public QObject
{
	Q_OBJECT

public:
	Gameplayer(QObject *parent=NULL);
	~Gameplayer();
	QIcon icon;
	char user[20];
	char passwd[20];
	int wins,fails,winsp;
	short rank;
};
