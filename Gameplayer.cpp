#include "Gameplayer.h"

Gameplayer::Gameplayer(QObject *parent)
	: QObject(parent),wins(0),rank(0),fails(0),winsp(0)
{
	strcpy(user, "���Ԫ���");
	strcpy(passwd, "");
}

Gameplayer::~Gameplayer()
{
}
