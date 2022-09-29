#include "Gameuiview.h"
#include"GBK.h"
#include"QPainter"
#include"QMouseEvent"
#include "QTimerEvent"
Gameuiview::Gameuiview(QWidget *parent)
	: QWidget(parent), ispressed(false),isfastmode(true)
{
	ui.setupUi(this);
	model = new Gamemodel(this);
	Login();
	pm_bg.load("Resources/bg.jpg");
	black.load("Resources/black.png");
	white.load("Resources/white.png");
	green.load("Resources/green.png");
	red.load("Resources/red.png");

	bangdownmu = new QMediaPlayer(this);
	bangdownmu->setMedia(QUrl::fromLocalFile(GBK::ToUnicode("Resources/bangdownmu.mp3")));
	bangdownmu->setVolume(80);

	connect(ui.checkright, &QPushButton::clicked, this, &Gameuiview::Oncheckright);
	connect(ui.btnexit, &QPushButton::clicked, this, &Gameuiview::Onexitlogin);
	connect(ui.btnreconn, &QPushButton::clicked, this, &Gameuiview::Reconn);
	connect(ui.btninvate, &QPushButton::clicked, this, &Gameuiview::Invate);
	connect(invtw.ui.btnrecv, &QPushButton::clicked, this, &Gameuiview::acptinvt);
	connect(invtw.ui.btnreject, &QPushButton::clicked, this, &Gameuiview::rjtinvt);
	connect(model, &Gamemodel::Invtsuc, this, &Gameuiview::Stagetinvtans);
	connect(ui.btnnewgame, &QPushButton::clicked, this, &Gameuiview::Newgame);
	connect(ui.btnqtrm, &QPushButton::clicked, this, &Gameuiview::qtrm);
	connect(gmsg.ui.btnok, &QPushButton::clicked, this, &Gameuiview::msgwinok);
	connect(gmsg.ui.btncancel, &QPushButton::clicked, this, &Gameuiview::msgwincancel);
	connect(ui.btnsurrender, &QPushButton::clicked, this, &Gameuiview::surrend);
	connect(ui.btnsend, &QPushButton::clicked, this, &Gameuiview::chat);
	connect(ui.chatedit, &QLineEdit::returnPressed, this, &Gameuiview::chat);
	connect(ui.fastdownbtn, &QPushButton::clicked, this, &Gameuiview::Onchangefastmode);
	connect(model->invt.ui.friends, &QListWidget::itemClicked, this, &Gameuiview::onFriendSelected);
}
void Gameuiview::onFriendSelected(QListWidgetItem *item) {
	model->invt.ui.invateedit->setText(item->text());
}
//painterEvent
void Gameuiview::paintEvent(QPaintEvent *event)
{
	this->setMouseTracking(true);
	QPainter painter(this);
	//����
	painter.drawPixmap(QRect(0, 0, width(), height()), pm_bg);
	for (int i = 0; i < 15; i++)
	{
		painter.drawLine(50, 70 + 50 * i, 750, 70 + 50 * i);
	}
	for (int i = 0; i < 15; i++)
	{
		painter.drawLine(50 + 50 * i, 70, 50 + 50 * i, 770);
	}
	char buf[3];
	painter.setFont(QFont(GBK::ToUnicode("����")));
	for (int i = 0; i < 15; i++)
	{
		sprintf(buf, "%c", 'A' + i);
		painter.drawText(QPoint(10, 70 + i * 50), buf);
	}
	for (int i = 0; i < 15; i++)
	{
		if (i >= 9)
		{
			sprintf_s(buf, "1%c", '0' + i - 9);
		}
		else
		{
			sprintf_s(buf, "%c", '1' + i);
		}
		painter.drawText(QPoint(50 + 50 * i, 35), buf);
	}
	painter.setBrush(QBrush(QColor(0, 0, 0)));
	//���
	painter.drawEllipse(QRect(397, 417, 6, 6));
	painter.drawEllipse(QRect(197, 617, 6, 6));
	painter.drawEllipse(QRect(597, 217, 6, 6));
	painter.drawEllipse(QRect(197, 217, 6, 6));
	painter.drawEllipse(QRect(597, 617, 6, 6));
	//����
	if (model->isconn)
	{
		for (int i = 0; i < model->units.size(); i++)
		{
			if (model->units[i].flag)
			{
				painter.drawPixmap(QRect(25 + 50 * model->units[i].c, 45 + 50 * model->units[i].r, 48, 48), white);
			}
			else
			{
				painter.drawPixmap(QRect(25 + 50 * model->units[i].c, 45 + 50 * model->units[i].r, 48, 48), black);

			}
		}
	}
	//��Ч
	if (model->turn&&model->isconn&&model->ismatched)
	{
		if (ispressed)
		{
			if (model->who)
			{
				painter.drawPixmap(QRect(25 + 50 * c_, 45 + 50 * r_, 48, 48), white);
			}
			else
			{
				painter.drawPixmap(QRect(25 + 50 * c_, 45 + 50 * r_, 48, 48), black);

			}
		}
		else
		{
			painter.drawPixmap(QRect(25 + 50 * c_, 45 + 50 * r_, 48, 48), red);
		}
	}
}
//����¼�
void Gameuiview::mouseMoveEvent(QMouseEvent *event)
{
	if (model->turn&&model->isconn&&model->ismatched)
	{
		if (!ispressed)
		{
			QPoint p = event->pos();
			if (p.x() > 25 && p.x() < 775 && p.y() > 45 && p.y() < 795)
			{
				char r, c;
				r = (p.y() - 45) / 50;
				c = (p.x() - 25) / 50;
				if (model->jz[r][c] == -1)
				{
					r_ = r;
					c_ = c;
					update();
				}
			}
		}
	}
}
void Gameuiview::mousePressEvent(QMouseEvent *event)
{
	if (model->turn&&model->isconn&&model->ismatched)
	{
		QPoint p = event->pos();
		if (p.x() > 25 && p.x() < 775 && p.y() > 45 && p.y() < 795)
		{
			char r, c;
			r = (p.y() - 45) / 50;
			c = (p.x() - 25) / 50;
			if (model->jz[r][c] == -1)
			{
				r_ = r;
				c_ = c;
				ispressed = true;
				bangdownmu->play();
				if (isfastmode)Oncheckright();
				else ui.checkright->setEnabled(true);
				update();
			}
		}
	}
}


//����
void Gameuiview::refreshpeer()
{
	char wins[6], fails[6], winsp[6];
	sprintf(wins, "%d", model->players[1].wins);
	sprintf(winsp, "%d%%", model->players[1].winsp);
	sprintf(fails, "%d", model->players[1].fails);
	ui.player2->setText(GBK::ToUnicode(model->players[1].user));
	ui.wins2->setText(wins);
	ui.fail2->setText(fails);
	ui.wp2->setText(winsp);
	sprintf(wins, "%d", model->desknum);
	ui.desknum->setText(GBK::ToUnicode(wins));
	update();
}
void Gameuiview::refreshself()
{
	char wins[6], fails[6], winsp[6];
	sprintf(wins, "%d", model->players[0].wins);
	sprintf(winsp, "%d%%", model->players[0].winsp);
	sprintf(fails, "%d", model->players[0].fails);
	ui.player1->setText(GBK::ToUnicode(model->players[0].user));
	ui.wins1->setText(wins);
	ui.fail1->setText(fails);
	ui.wp1->setText(winsp);
	update();
}
void Gameuiview::Login()
{
	char r = model->Login();
	if (r == 1)
	{
		ui.connserver->setStyleSheet("color: rgb(0 ,255, 0);");
		ui.connserver->setText(GBK::ToUnicode("�ɹ���½^_^"));
		Starecvinvt();
	}
	else if (r == 0)
	{
		ui.connserver->setStyleSheet("color: rgb(255 ,0, 0);");
		ui.connserver->setText(GBK::ToUnicode("���˺��Ѿ�����¼"));
	}
	else
	{
		ui.connserver->setStyleSheet("color: rgb(255 ,0, 0);");
		ui.connserver->setText(GBK::ToUnicode("���ӷ�����ʧ�������µ�½@_@"));
	}
	Reset();
}
void Gameuiview::Reset()
{
	ispressed = false;
	ui.labtip->setStyleSheet("color: rgb(0,255, 0);");
	ui.labtip->setText(GBK::ToUnicode("����������*_*"));
	ui.btnsend->setEnabled(false);
	killTimer(tm_start);
	killTimer(tm_rest);
	char wins[6], fails[6], winsp[6];
	sprintf(wins, "%d", model->players[0].wins);
	sprintf(winsp, "%d%%", model->players[0].winsp);
	sprintf(fails, "%d", model->players[0].fails);
	ui.player1->setText(GBK::ToUnicode(model->players[0].user));
	ui.wins1->setText(wins);
	ui.fail1->setText(fails);
	ui.wp1->setText(winsp);
	ui.player2->setText(GBK::ToUnicode("���Ԫ���"));
	ui.wins2->setText("0");
	ui.fail2->setText("0");
	ui.wp2->setText("0%");
	ui.desknum->setText("0");
	ui.labstate->setStyleSheet(GBK::ToUnicode("color: rgb(255,0,0);font: 12pt \"����\";"));
	ui.labstate->setText(GBK::ToUnicode("��Ϸδ��ʼ"));
	ui.starttm->setText("0");
	ui.resttm->setText("0");
	ui.chatedit->setEnabled(false);
	ui.btnsurrender->setEnabled(false);
	ui.checkright->setEnabled(false);
	ui.chatwidget->clear();
	ui.rcdwgt->clear();
	model->Clean();
	model->ismatched = false;
	ui.btnnewgame->setEnabled(false);
	ui.btnqtrm->setEnabled(false);
	if (!model->isconn)
	{
		ui.btnmatch->setEnabled(false);
		ui.btninvate->setEnabled(false);
	}
	else
	{
		ui.btnmatch->setEnabled(true);
		ui.btninvate->setEnabled(true);
	}
	update();
}
char* Strcut(char* str)
{
	for (int i = strlen(str) - 1; i >= 0; i--)
	{
		if ((str[i]) == '\r')
		{
			str[i] = 0;
			break;
		}
	}
	return str;
}
//slots
void Gameuiview::msgwinok()
{
	////�����ִ��
	////
	if (state == 0)
	{
		//ƨ��û��
	}
	else if (state == 1)
	{
		//ͬ���˶Է�������
		if (model->ismatched )
		{
			//���ս�����һ�ֵ���Ϣ
			char req[128], rep[128];
			sprintf(req, "acpnewgame");
			model->connsv->Sendrequest(req, rep);
			if (memcmp(rep, "acp successful", 14) == 0)
			{
				//��һ�ֿ�ʼ��������Ϣ
				int n;
				sscanf_s(rep + 15, "%d", &n);
				startnewgame(n);
			}
			else
			{
				//�Է��Ѿ�����
				ui.labtip->setStyleSheet("color: rgb(255,0, 0);");
				ui.labtip->setText(GBK::ToUnicode("�Է��Ѿ�����"));
			}
		}
	}
	gmsg.close();
}
void Gameuiview::msgwincancel()
{
	////һ����close
	gmsg.close();
}
void Gameuiview::qtrm()
{
	if (!model->ismatched)return;
	char req[128];
	sprintf(req, "qtrm");
	model->connsv->Sendrequest(req);
	//�˳�����Ĵ���
	Reset();
	ui.chatwidget->addItem(GBK::ToUnicode("���˳��˷���"));

}
void Gameuiview::Newgame()
{
	if (!model->ismatched)return;

	char req[128], rep[128];
	sprintf(req, "newgame");
	model->connsv->Sendrequest(req, rep);
	//����
	if (memcmp(rep, "newgame successful", 18) == 0)
	{
		//����newgame�ɹ�

	}
	else
	{
		//�Է��Ѿ����߻��˳�����
		//�����˳������׼��
		Reset();
	}

}
void Gameuiview::Oncheckright()
{
	ispressed = false;
	ui.checkright->setEnabled(false);
	ui.labstate->setStyleSheet(GBK::ToUnicode("color: rgb(255,0,0);font: 12pt \"����\";"));
	ui.labstate->setText(GBK::ToUnicode("�ֵ��Է���"));
	model->turn = false;
	killTimer(tm_rest);
	model->rest.setHMS(0, 2, 0);
	tm_rest = startTimer(1000);
	bangdown(r_, c_);

	Gameunit u(r_, c_, model->who);
	model->units.push_back(u);
	if (model->who)
	{
		model->jz[r_][c_] = 1;
	}
	else
	{
		model->jz[r_][c_] = 0;
	}
	if (model->isfinished(r_, c_))
	{
		////////////////������Ϸ����
		Ongameover(true);
		gameover();
	}
	
	
}
void  Gameuiview::Onchangefastmode()
{
	string fastmode = isfastmode ? "�Ƚ�" : "���";
	isfastmode = isfastmode ? false : true;
	
	//ui.fastdownbtn->setStyleSheet(GBK::ToUnicode("color: rgb(255,0,0);font: 12pt \"����\";"));
	ui.fastdownbtn->setText(GBK::ToUnicode(fastmode.c_str()));
}
void Gameuiview::Onexitlogin()
{
	//////����һ������/////
	if (model->isconn)
	{
		model->exit();
	}
	TCHAR commandLine[] = TEXT("gamelogin.exe");
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	bool bRet = CreateProcess(
		NULL,
		commandLine,
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi);
	close();
}
void Gameuiview::Reconn()
{
	if (!model->connsv->Checkconn())
	{
		Login();
		ui.connserver->setStyleSheet("color: rgb(0 ,255, 0);");
		ui.connserver->setText(GBK::ToUnicode("�ɹ���½^_^"));
		Reset();
	}
}
void Gameuiview::Invate()
{
	//��friends ��ӵ�items
	if (model->invt.ui.friends->count() == 0) {
		FILE* fp;
		char *rst, buffer[64];
		fopen_s(&fp, "data/myfriends.txt", "r");
		if (fp) {
			while ((rst = fgets(buffer, 64, fp)) != NULL) {
				buffer[strlen(buffer) - 1] = 0;
				model->invt.ui.friends->addItem(GBK::ToUnicode(buffer));
			}
			fclose(fp);
		}
	}
	
	model->invt.showNormal();
}
void Gameuiview::acptinvt()
{
	invtw.close();
	if (model->ismatched)return;

	char req[128], rep[128];
	sprintf(req, "ansinvt+%s", GBK::FromUnicode(invtw.ui.labid->text()).c_str());
	model->connsv->Sendrequest(req, rep);

	if (memcmp(rep, "ansinvt successful", strlen("ansinvt successful")) == 0)
	{
		//ƥ��ɹ���������Ϣ
		ui.btnsend->setEnabled(true);
		ui.btnqtrm->setEnabled(true);
		ui.btnnewgame->setEnabled(true);
		strcpy(model->players[1].user, GBK::FromUnicode(invtw.ui.labid->text()).c_str());
		sscanf_s(rep + 19, "%d+%d+%d+%d", &model->desknum, &model->players[1].wins, &model->players[1].fails, &model->players[1].winsp);
		model->ismatched = true;
		model->turn = false;
		Stpgetinvtans();
		refreshpeer();
		ui.chatedit->setEnabled(true);
		ui.labtip->setStyleSheet("color: rgb(0,255, 0);");
		ui.labtip->setText(GBK::ToUnicode("�ɹ����뷿��*_*"));
		sprintf(req, "��������%s ������", model->players[1].user);
		ui.chatwidget->addItem(GBK::ToUnicode(req));
		sprintf(req, "%s: �����˷���(^_^)", model->players[0].user);
		ui.chatwidget->addItem(GBK::ToUnicode(req));
		sprintf(req, "%s: �����˷���(^_^)", model->players[1].user);
		ui.chatwidget->addItem(GBK::ToUnicode(req));
	}
	else if (memcmp(rep, "error", 5) == 0)
	{
		//�Է�����Ϸ��
		ui.labtip->setStyleSheet("color: rgb(255,0, 0);");
		ui.labtip->setText(GBK::ToUnicode("�Է��Ѿ���ʼ��Ϸ@_@"));
	}
	else
	{
		//�Է��Ѿ�����
		ui.labtip->setStyleSheet("color: rgb(255,0, 0);");
		ui.labtip->setText(GBK::ToUnicode("�Է��Ѿ�����"));
	}

}
void Gameuiview::rjtinvt()
{
	invtw.close();
}
//timeEvent������
void Gameuiview::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == tm_recvinvt)
	{
		recvinvt();
	}
	else if (event->timerId() == tm_ansinvt)
	{
		getinvtans();
	}
	else if (event->timerId() == tm_start)
	{
		model->start = model->start.addSecs(1);
		ui.starttm->setText(model->start.toString("mm:ss"));
	}
	else if (event->timerId() == tm_rest)
	{
		if (model->rest == QTime(0, 0, 0))
		{
			//��ʱ����
			killTimer(tm_rest);

		}
		else
		{
			model->rest = model->rest.addSecs(-1);
			ui.resttm->setText(model->rest.toString("mm:ss"));
		}
	}
}


void Gameuiview::recvinvt()
{
	char req[128], rep[128];
	sprintf(req, "recvinvt");
	model->connsv->Sendrequest(req, rep);
	//������Ϣ����������з���ִ��
	/////////////////////////////////////////////////
	if (memcmp(rep, "invate", 6) == 0)
	{
		//�յ�����
		invtw.ui.labid->setText(GBK::ToUnicode(Strcut(rep + 7)));
		invtw.show();
	}
	else if (memcmp(rep, "qtrm", 4) == 0)
	{
		//�˳�����Ĵ���
		Reset();
		sprintf(req, "%s: �˳��˷���(^_^)", model->players[1].user);
		ui.chatwidget->addItem(GBK::ToUnicode(req));
		ui.chatwidget->addItem(GBK::ToUnicode("�� �Ѿ�����������(^_^)"));
	}
	else if (memcmp(rep, "newgame", 7) == 0)
	{
		//���յ�newgame������
		state = 1;
		mutex_msg.Lock();
		gmsg.ui.btncancel->setEnabled(true);
		gmsg.ui.labmsg->setText(GBK::ToUnicode("�Է�����һ����Ϸ!"));
		gmsg.showNormal();
		mutex_msg.Unlock();
	}
	else if (memcmp(rep, "acpnewgame", 10) == 0)
	{
		//��һ�ֿ�ʼ
		int n;
		sscanf_s(rep + 11, "%d", &n);
		startnewgame(n);
	}
	else if (memcmp(rep, "surrend", 7) == 0)
	{
		//�Է�����
		sprintf(rep, "%s: ������Ͷ��", model->players[1].user);
		ui.chatwidget->addItem(GBK::ToUnicode(rep));
		Ongameover(true);
	}
	else if (memcmp(rep, "chat",4) == 0)
	{
		char str[128];
		sprintf_s(str, "%s: %s", model->players[1].user, rep + 5);
		ui.chatwidget->addItem(GBK::ToUnicode(str));
		ui.chatwidget->scrollToBottom();
	}
	else if (memcmp(rep, "bangdown", 8) == 0)
	{
		//�Է�������
		if (!model->ismatched)return;
		bangdownmu->play();
		killTimer(tm_rest);
		model->rest.setHMS(0, 2, 0);
		tm_rest = startTimer(1000);
		sscanf_s(rep + 9, "%d+%d", &r_, &c_);
		Gameunit u(r_, c_, !model->who);
		model->units.push_back(u);
		if (!model->who)
		{
			model->jz[r_][c_] = 1;
		}
		else
		{
			model->jz[r_][c_] = 0;
		}
		ui.labstate->setStyleSheet(GBK::ToUnicode("color: rgb(0,170,0);font: 12pt \"����\";"));
		ui.labstate->setText(GBK::ToUnicode("�ֵ�����"));
		model->turn = true;
		update();
	}
	else if (memcmp(rep, "gameover", 8) == 0)
	{
		//����������
		Ongameover(false);
	}
	else
	{
		//
	}
}
void Gameuiview::getinvtans()
{
	if (model->ismatched)return;
	ui.btnsend->setEnabled(true);
	model->turn = false;
	char req[128], rep[128];
	sprintf(req, "getinvtans");
	model->connsv->Sendrequest(req, rep);
	if (memcmp(rep, "getinvtans successful", 21) == 0)
	{
		//����ͬ��������,match�ɹ���������ʼ����Ϣ
		ui.btnsend->setEnabled(true);
		ui.btnqtrm->setEnabled(true);
		ui.btnnewgame->setEnabled(true);
		char info[20];
		sscanf_s(rep + 22, "%d+%d+%d+%d\r\n+%s\r\n", &model->desknum, &model->players[1].wins, &model->players[1].fails, &model->players[1].winsp, model->players[1].user, 20);
		model->ismatched = true;
		Stpgetinvtans();
		refreshpeer();
		ui.chatedit->setEnabled(true);
		////�ݷ�
		ui.labtip->setStyleSheet("color: rgb(0,255, 0);");
		ui.labtip->setText(GBK::ToUnicode("�ɹ����뷿��*_*"));
		sprintf(req, "%s ��������������!", model->players[1].user);
		ui.chatwidget->addItem(GBK::ToUnicode(req));
		sprintf(req, "%s: �����˷���(^_^)", model->players[0].user);
		ui.chatwidget->addItem(GBK::ToUnicode(req));
		
		sprintf(req, "%s: �����˷���(^_^)", model->players[1].user);
		ui.chatwidget->addItem(GBK::ToUnicode(req));
	}

}
void Gameuiview::Stagetinvtans(const char* invtname)
{
	char str[128];
	sprintf_s(str, "%s: %s", "�ɹ�����",invtname);
	ui.chatwidget->addItem(GBK::ToUnicode(str));
	ui.chatwidget->scrollToBottom();
	tm_ansinvt = startTimer(1000);
}
void Gameuiview::startnewgame(int n)
{
	model->Clean();
	tm_start=startTimer(1000);
	ui.chatwidget->addItem(GBK::ToUnicode("��ʼ��һ���±���!"));
	ui.labtip->setStyleSheet("color: rgb(0,255, 0);");
	ui.labtip->setText(GBK::ToUnicode("��Ϸ������*_*"));
	ui.btnnewgame->setEnabled(false);
	ui.btnsurrender->setEnabled(true);
	//////////////////////////////////////////////////////////////////
	if (n)
	{//��ɫ �Է�����
		ui.labstate->setStyleSheet(GBK::ToUnicode("color: rgb(255,0,0);font: 12pt \"����\";"));
		ui.labstate->setText(GBK::ToUnicode("�ֵ��Է���"));
		ui.label_2->setText(GBK::ToUnicode("��"));
		ui.label_3->setText(GBK::ToUnicode("��"));
		model->turn = false;
		model->who = true;
	}
	else
	{//��ɫ������
		ui.labstate->setStyleSheet(GBK::ToUnicode("color: rgb(0,170,0);font: 12pt \"����\";"));
		ui.label_2->setText(GBK::ToUnicode("��"));
		ui.label_3->setText(GBK::ToUnicode("��"));
		ui.labstate->setText(GBK::ToUnicode("�ֵ�����"));
		model->turn = true;
		model->who = false;
		model->rest.setHMS(0, 2, 0);
		tm_rest=startTimer(1000);
	}
	update();
}
void Gameuiview::bangdown(int r, int c)
{
	char req[128], rep[128];
	sprintf(req, "bangdown+%d+%d", r, c);
	model->connsv->Sendrequest(req, rep);
	//����
	if (memcmp(rep, "successful", 10) == 0)
	{
	}
	else
	{
		//�Է��Ѿ����߻��˳�����
		//�����˳������׼��
		//�Է�����message
		Reset();
	}

}
void Gameuiview::gameover()
{
	char req[128], rep[128];
	sprintf(req, "gameover");
	model->connsv->Sendrequest(req, rep);
	//����
	if (memcmp(rep, "successful", 10) == 0)
	{
	}
	else
	{
		//�Է��Ѿ����߻��˳�����
		//�����˳������׼��
		//�Է�����message
		Reset();
	}
}
void Gameuiview::surrend()
{
	Ongameover(false);
	char req[128], rep[128];
	sprintf(req, "surrend");
	model->connsv->Sendrequest(req, rep);
	ui.chatwidget->addItem(GBK::ToUnicode("��������Ͷ��~"));
	//����
	if (memcmp(rep, "successful", 10) == 0)
	{
	}
	else
	{
		//�Է��Ѿ����߻��˳�����
		//�����˳������׼��
		//�Է�����message
		Reset();
	}
}
void Gameuiview::chat()
{
	if (ui.chatedit->text() == "")return;
	char req[128], rep[128];
	sprintf(req, "%s: %s",model->players[0].user, GBK::FromUnicode(ui.chatedit->text()).c_str());
	ui.chatwidget->addItem(GBK::ToUnicode(req));
	ui.chatwidget->scrollToBottom();
	sprintf(req, "chat+%s", GBK::FromUnicode(ui.chatedit->text()).c_str());
	ui.chatedit->clear();
	model->connsv->Sendrequest(req, rep);
	//����
	if (memcmp(rep, "successful", 10) == 0)
	{
	}
	else
	{
		//�Է��Ѿ����߻��˳�����
		//�����˳������׼��
		//�Է�����message
		Reset();
	}
}
void Gameuiview::Ongameover(bool iswin)
{
	char buf[128];
	if (iswin)
	{
		model->players[0].wins++;
		model->players[1].fails++;
	}
	else
	{
		model->players[1].wins++;
		model->players[0].fails++;
	}
	model->players[0].winsp = (double)model->players[0].wins / (model->players[0].wins + model->players[0].fails)*100;
	model->players[1].winsp = (double)model->players[1].wins / (model->players[1].wins + model->players[1].fails)*100;
	refreshpeer();
	refreshself();
	//�����������ս��
	sprintf(buf,"changeinfo+%d+%d+%d\r\n",model->players[0].wins, model->players[0].fails, model->players[0].winsp);
	model->connsv->Sendrequest(buf);
	
	killTimer(tm_start);
	killTimer(tm_rest);
	ispressed = false;
	ui.checkright->setEnabled(false);
	ui.labstate->setStyleSheet(GBK::ToUnicode("color: rgb(255,0,0);font: 12pt \"����\";"));
	ui.labstate->setText(GBK::ToUnicode("����δ��ʼ"));

	if (iswin)sprintf(buf, "%d %sӮ����һ������", ui.rcdwgt->count() + 1, model->players[0].user);
	else 	sprintf(buf, "%d %sӮ����һ������", ui.rcdwgt->count() + 1, model->players[1].user);
	ui.rcdwgt->addItem(GBK::ToUnicode(buf));

	if (iswin)	ui.chatwidget->addItem(GBK::ToUnicode("��Ӯ����һ����Ϸ!"));
	else
	{
		sprintf(buf, "%s: Ӯ����һ����Ϸ!", model->players[1].user);
		ui.chatwidget->addItem(GBK::ToUnicode(buf));
	}

	ui.btnnewgame->setEnabled(true);
	ui.btnsurrender->setEnabled(false);
	model->turn = false;
	state = 0;
	mutex_msg.Lock();
	gmsg.ui.btncancel->setEnabled(false);
	if(iswin)gmsg.ui.labmsg->setText(GBK::ToUnicode("��Ӯ��,���^_^"));
	else gmsg.ui.labmsg->setText(GBK::ToUnicode("laji,������&_&"));
	gmsg.showNormal();
	mutex_msg.Unlock();
}