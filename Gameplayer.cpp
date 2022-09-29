#include "Gameplayer.h"

Gameplayer::Gameplayer(QObject *parent)
	: QObject(parent),wins(0),rank(0),fails(0),winsp(0)
{
	strcpy(user, "Òì´ÎÔªÍæ¼Ò");
	strcpy(passwd, "");
}

Gameplayer::~Gameplayer()
{
}
