#pragma once
#include <QWidget>
#include "ui_Gameuiview.h"
#include "Gamemodel.h"
#include"msgwin.h"
#include <windows.h>
#include "Invatewin.h"
#include "osapi/Mutex.h"
#include "QMediaPlayer"

class Gameuiview : public QWidget
{
	Q_OBJECT

public:
	Gameuiview(QWidget *parent = Q_NULLPTR);
	void paintEvent(QPaintEvent *event);
	int r_, c_;
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	bool ispressed;
	bool isfastmode;
	void Oncheckright();
	void Login();
	void Reset();

	void Starecvinvt()
	{
		tm_recvinvt = startTimer(500);
	}


	void Stprecvinvt()
	{
		killTimer(tm_recvinvt);
		tm_recvinvt = -1;
	}
	void Stpgetinvtans()
	{
		killTimer(tm_ansinvt);
		tm_ansinvt = -1;
	}


	void recvinvt();

	void getinvtans();

	void startnewgame(int n);

	void bangdown(int r,int c);

	void refreshpeer();

	void refreshself();

	Invatewin invtw;
	msgwin gmsg;
	OS_Mutex mutex_msg;
	char state;
	int tm_ansinvt, tm_recvinvt,tm_start,tm_rest;
private:
	Ui::Gameuiview ui;
	QPixmap pm_bg, black, white, red,green;
	QMediaPlayer* bangdownmu;
	Gamemodel* model;
	void timerEvent(QTimerEvent *event);
public slots:
	void Onexitlogin();
	void Reconn();
	void Invate();
	void acptinvt();
	void rjtinvt();
	void Newgame();
	void qtrm();
	void msgwinok();
	void msgwincancel();
	void surrend();
	void chat();
	void gameover();
	void Ongameover(bool iswin);
	void Onchangefastmode();
	void onFriendSelected(QListWidgetItem *item);
	void Stagetinvtans(const char* invtname);
};
