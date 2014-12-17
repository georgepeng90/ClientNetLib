#pragma once

#include <vector>
#include <functional>

#include "Stream.h"

class MessageCenter
{
public:

	typedef std::function<void(Stream&)> MessageHandler;
	typedef std::map<int, MessageHandler> MessageHandlerMap;
	typedef std::function<void(void)> MessageExtraHandler;
	typedef std::map<int, MessageExtraHandler> MessageExtraHandlerMap;

	static MessageCenter* getInstance();

	MessageCenter();
	virtual ~MessageCenter();

	void registerNormalHandler(uint16 aMessageID, MessageHandler aHandler);
	void registerExtraHandler(uint16 aMessageID, MessageExtraHandler aHandler);
	void removeExtraHandler(uint16 aMessageID);

	void receiveMessage(uint16 aMessageID, Stream& aStream);

	void block(uint16 aUnBlockMessageID);
	void unblock();

protected:

	bool _isBlocked = false;
	int _unblockMessageID = 0;
	MessageHandlerMap _normalHandlerMap;
	MessageExtraHandlerMap _oneTimeExtraHandlerMap;
};