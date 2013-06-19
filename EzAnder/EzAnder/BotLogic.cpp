#include "StdAfx.h"
#include "BotLogic.h"

BotLogic* BotLogic::_instance = NULL;

BotLogic& BotLogic::instance()
{
	if (!_instance) {
		_instance = new BotLogic();
	}
	return *_instance;
}

BotLogic::BotLogic(void)
{
}


BotLogic::~BotLogic(void)
{
}
