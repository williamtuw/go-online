#pragma once
#include "QVector"
#include <QObject>
#include"Gameunit.h"
#include"Gameplayer.h"
#include "QTime"
#include "Connserver.h"
#include "osapi/osapi.h"
#include "Gameinvate.h"

class Gamemodel : public QWidget
{
	Q_OBJECT
signals:
	void Invtsuc(const char* invtname);
public:
	Gamemodel(QWidget *parent);
	~Gamemodel();
	char jz[15][15];
	QVector<Gameunit> units;
	int desknum;
	Gameplayer players[2];
	QTime start, rest;
	Gameinvate invt;
	bool isfinished(char r, char c);
	void Clean();
	int Login();
	void exit();
	bool addNewFriends(const char* name);
	bool isconn,ismatched, turn, who;
	Connserver* connsv;
public slots:
	bool Invate();
};
