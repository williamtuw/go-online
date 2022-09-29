#include "Connserver.h"

Connserver::Connserver():connflag(false), addr("0.0.0.0", 0)
{
}


Connserver::~Connserver()
{
}
Connserver::Connserver(const char* ip, unsigned short port) :connflag(false)
{
	addr.SetIp(ip);
	addr.SetPortW(port);
}
bool Connserver::Connnect()
{
	wsocket = new OS_TcpSocket;
	wsocket->SetOpt_RecvTimeout(5000);
	wsocket->SetOpt_SendTimeout(5000);
	wsocket->Open(true);
		if (wsocket->Connect(addr) < 0)
		{
			return false;
		}
		connflag = true;
		return true;
}
bool Connserver::isconnected()
{
	mutex.Lock();
	return connflag;
	mutex.Unlock();
}
bool Connserver::Disconnect()
{
	mutex.Lock();
	wsocket->Send("exit",128);
	wsocket->Close();
	delete wsocket;
	connflag = false;
	mutex.Unlock();
	return true;
}
bool Connserver::Checkconn()
{
	char req[128], rep[128];
	strcpy(req, "checkconn");
	Sendrequest(req, rep);
	if (memcmp(rep, "checkright",10) == 0)
	{
		return true;
	}
	connflag = false;
	return false;
}
bool Connserver::Sendrequest(const char* req)
{
	char respon[128];
	mutex.Lock();
	if (connflag)
	{	
		wsocket->Send(req, 128);
		mutex.Unlock();
		return true;
	}
	mutex.Unlock();
	return false;
}

bool Connserver::Sendrequest(const char* req,char* rep)
{
	char respon[128] = { 0 };
	mutex.Lock();
	//统一发送接收字节数
	if (connflag)
	{	
		wsocket->Send(req,128);
		wsocket->Recv(respon, 128);
		mutex.Unlock();
		strcpy(rep, respon);
		return true;
	}
	 mutex.Unlock();
	 return false;
}
void Connserver::SetTcpAddr(const char* ip, unsigned short port)
{
	mutex.Lock();
	addr.SetIp(ip);
	addr.SetPortW(port);
	mutex.Unlock();
}