#pragma once

#include "cocos2d.h"

class NetUpdater : public cocos2d::Node
{
public:

	NetUpdater();
	virtual ~NetUpdater();

	static void startUpdateNet();
	virtual void update(float delta);
};
