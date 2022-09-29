#pragma once
#include<qstring.h>
#include<string>
using namespace std;
class GBK
{
public:
	//QString=>std::string(GBK)
	static std::string FromUnicode(const QString& qstr);
	GBK();
	~GBK();
	static  QString ToUnicode(const char* str);

};

