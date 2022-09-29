#include "Gamemodel.h"
#include "GBK.h"

Gamemodel::Gamemodel(QWidget *parent)
	: QWidget(parent), who(false), turn(false),isconn(false),ismatched(false)
{
	connsv = new Connserver("127.0.0.1", 8888);

	connect(invt.ui.btninvt, &QPushButton::clicked, this, &Gamemodel::Invate);
	Clean();
}

Gamemodel::~Gamemodel()
{
}
bool Gamemodel::addNewFriends(const char* name)
{
	FILE* fp1, *fp2;
	char* rst;
	char friname[64];
	char buffer[64];
	sprintf(buffer, "%s\n", name);
	fopen_s(&fp1, "data/myfriends.txt", "r");
	if (fp1) {
		do
		{
			rst = fgets(friname, 64, fp1);
			if (strcmp(buffer, friname) == 0) {
				fclose(fp1);
				return false;
			}
		} while (rst != NULL);
		fclose(fp1);
	}

	fopen_s(&fp2, "data/myfriends.txt", "a");
	fputs(buffer, fp2);
	fclose(fp2);
	return true;

}
bool Gamemodel::isfinished(char r, char c)
{
	char bang;
	if (who)
	{
		bang = 1;
	}
	else
	{
		bang = 0;
	}
	//横
	int flag = 0;
	int i = 1;
	while (1)
	{
		if ((c - i) < 0)break;
		if (jz[r][c - i] == bang)
		{
			flag++;
		}
		else
		{
			break;
		}
		i++;
	}
	i = 1;
	while (1)
	{
		if ((c + i) > 14)break;
		if (jz[r][c + i] == bang)
		{
			flag++;
		}
		else
		{
			break;
		}
		i++;
	}
	if (flag >= 4)
	{
		return true;
	}
	//竖
	flag = 0;
	i = 1;
	while (1)
	{
		if ((r - i) < 0)break;
		if (jz[r - i][c] == bang)
		{
			flag++;
		}
		else
		{
			break;
		}
		i++;
	}
	i = 1;
	while (1)
	{
		if ((r + i) > 14)break;
		if (jz[r + i][c] == bang)
		{
			flag++;
		}
		else
		{
			break;
		}
		i++;
	}
	if (flag >= 4)
	{
		return true;
	}
	//对角线
	flag = 0;
	i = 1;
	while (1)
	{
		if (((r - i) < 0) || ((c - i) < 0))break;
		if (jz[r - i][c - i] == bang)
		{
			flag++;
		}
		else
		{
			break;
		}
		i++;
	}
	i = 1;
	while (1)
	{
		if (((r + i) > 14) || ((c + i) > 14))break;
		if (jz[r + i][c + i] == bang)
		{
			flag++;
		}
		else
		{
			break;
		}
		i++;
	}
	if (flag >= 4)
	{
		return true;
	}
	//反对角线
	flag = 0;
	i = 1;
	while (1)
	{
		if (((r - i) < 0) || ((c + i) > 14))break;
		if (jz[r - i][c + i] == bang)
		{
			flag++;
		}
		else
		{
			break;
		}
		i++;
	}
	i = 1;
	while (1)
	{
		if (((r + i) > 14) || ((c - i) < 0))break;
		if (jz[r + i][c - i] == bang)
		{
			flag++;
		}
		else
		{
			break;
		}
		i++;
	}
	if (flag >= 4)
	{
		return true;
	}
	return false;
}
void Gamemodel::Clean()
{
	units.clear();
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			jz[i][j] = -1;
		}
	}
	start.setHMS(0, 0, 0);
	rest.setHMS(0, 0, 0);
}
int Gamemodel::Login()
{
	FILE* pp;
	char buffer[128];
	fopen_s(&pp, "data/logininfo", "rb");
	char user[20];
	char passwd[20];
	char ip[20];
	if (pp)
	{
		fgets(user, 20, pp);
		fgets(passwd, 20, pp);
		fgets(ip, 20, pp);
		fclose(pp);
	}
	else
	{
		return -1;
	}
	connsv->SetTcpAddr(ip, 8888);
	sprintf(buffer, "gamelogin+%s+%s", user, passwd);
	char rep[128];
	if (connsv->Connnect())
	{
		connsv->Sendrequest(buffer, rep);
	}
	else
	{
		//服务器连接失败
		return -1;
	}
	if (memcmp(rep, "login successful", strlen("login successful")) == 0)
	{
		//成功登陆
		isconn = true;
		sscanf_s(rep + 17, "%d+%d+%d", &players[0].wins, &players[0].fails, &players[0].winsp);
		sscanf_s(user, "%s", players[0].user, 20);
		sscanf_s(passwd, "%s", players[0].passwd, 20);

	}
	else if (memcmp(rep, "error", 5) == 0)
	{
		//用户已经在线
		connsv->Disconnect();
		return 0;
	}
	else
	{
		//登陆失败
		connsv->Disconnect();
		return -1;
	}
	return 1;
}
bool Gamemodel::Invate()
{
	if (ismatched)
	{
		//已经在游戏中
		invt.ui.labtip->setStyleSheet("color: rgb(255,0,0);");
		invt.ui.labtip->setText(GBK::ToUnicode("您已经在游戏中"));
		return false;
	}
	char buffer[128] = { 0 };
	char rep[128];
	sprintf(buffer, "invate+%s",GBK::FromUnicode(invt.ui.invateedit->text()).c_str());
	if (memcmp(buffer+7,players[0].user,strlen(buffer+7)) == 0)
	{
		//邀请了自己
		invt.ui.labtip->setStyleSheet("color: rgb(255,0,0);");
		invt.ui.labtip->setText(GBK::ToUnicode("不能邀请自己"));
		return false;
	}
	connsv->Sendrequest(buffer, rep);
	if (memcmp(rep, "invate successful", strlen("invate successful")) == 0)
	{
		//邀请成功
		invt.ui.labtip->setStyleSheet("color: rgb(0,255,0);");
		invt.ui.labtip->setText(GBK::ToUnicode("邀请成功"));		
		emit Invtsuc(GBK::FromUnicode(invt.ui.invateedit->text()).c_str());
		addNewFriends(GBK::FromUnicode(invt.ui.invateedit->text()).c_str());
		invt.close();
	}
	else if (memcmp(rep, "error",5) == 0)
	{
		//对方在游戏中
		invt.ui.labtip->setStyleSheet("color: rgb(255,0,0);");
		invt.ui.labtip->setText(GBK::ToUnicode("玩家正在游戏中"));
		return false;
	}
	else
	{
		//邀请失败
		invt.ui.labtip->setStyleSheet("color: rgb(255,0,0);");
		invt.ui.labtip->setText(GBK::ToUnicode("玩家不存在或者不在线"));
		return false;
	}
	return true;
}
void Gamemodel::exit()
{
	char buffer[128];
	sprintf(buffer, "exit");
	connsv->Sendrequest(buffer);
}

