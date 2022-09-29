#pragma once
#include "osapi/osapi.h"
#include "osapi/Socket.h"
#include "osapi/Mutex.h"
class Connserver
{
public:
	Connserver();
	~Connserver();
	Connserver(const char* ip, unsigned short port);
	bool Connnect();
	bool Disconnect();
	bool isconnected();
	bool Sendrequest(const char* req, char* rep);
	bool Sendrequest(const char* req);
	void SetTcpAddr(const char* ip, unsigned short port);
	bool Checkconn();
private:
	bool connflag;
	OS_SockAddr addr;
	OS_TcpSocket* wsocket;
	OS_Mutex mutex;
};

