#include "MessageCenter.h"

MessageCenter* MessageCenter::getInstance()
{
	static MessageCenter *center = NULL;
	if (center == NULL)
	{
		center = new MessageCenter();
	}
	return center;
}

MessageCenter::MessageCenter()
{

}

MessageCenter::~MessageCenter()
{

}

void MessageCenter::registerNormalHandler(uint16 aMessageID, MessageHandler aHandler)
{
	_normalHandlerMap[aMessageID] = aHandler;
}

void MessageCenter::registerExtraHandler(uint16 aMessageID, MessageExtraHandler aHandler)
{
	_oneTimeExtraHandlerMap[aMessageID] = aHandler;
}

void MessageCenter::receiveMessage(uint16 aMessageID, Stream& aStream)
{
	MessageHandlerMap::const_iterator normalHandlerItr = _normalHandlerMap.find(aMessageID);
	if (normalHandlerItr != _normalHandlerMap.end())
	{
		normalHandlerItr->second(aStream);
	}

	MessageExtraHandlerMap::const_iterator extraHandlerItr = _oneTimeExtraHandlerMap.find(aMessageID);
	if (extraHandlerItr != _oneTimeExtraHandlerMap.end())
	{
		extraHandlerItr->second();
		_oneTimeExtraHandlerMap.erase(extraHandlerItr);
	}

	if (_unblockMessageID == aMessageID)
	{
		unblock();
	}
}

void MessageCenter::block(uint16 aUnBlockMessageID)
{
	if (_isBlocked)
	{
		return;
	}
	_unblockMessageID = aUnBlockMessageID;
	_isBlocked = true;
}

void MessageCenter::unblock()
{
	if (!_isBlocked)
	{
		return;
	}

	_isBlocked = false;
}

void MessageCenter::removeExtraHandler(uint16 aMessageID)
{
	for (MessageExtraHandlerMap::iterator itr = _oneTimeExtraHandlerMap.begin(); itr != _oneTimeExtraHandlerMap.end(); ++itr)
	{
		if (itr->first == aMessageID)
		{
			_oneTimeExtraHandlerMap.erase(itr);
			return;
		}
	}
}
