#include "GBK.h"

#include<QTextcodec>
#include<string>
GBK::GBK()
{
	
}


GBK::~GBK()
{
	
}
 QString GBK::ToUnicode(const char* str)
{
	QTextCodec* pcodec = QTextCodec::codecForName("gb2312");
	if (!pcodec)return NULL;
	QString qstring =pcodec->toUnicode(str,strlen(str));
	return qstring;
	
}
std::string GBK::FromUnicode(const QString& qstr)
{
	
	QTextCodec* pcodec = QTextCodec::codecForName("gb2312");
	if (!pcodec)return"";
	QByteArray buf = pcodec->fromUnicode(qstr);

	std::string str = buf.data();
	return str;
}