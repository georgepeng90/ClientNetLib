#include "NetUpdater.h"

#include "../NetMgr.h"

using namespace cocos2d;

NetUpdater::NetUpdater()
{

}

NetUpdater::~NetUpdater()
{

}

void NetUpdater::startUpdateNet()
{
	static NetUpdater *netUpdator = NULL;
	if (netUpdator == NULL)
	{
		netUpdator = new NetUpdater();
		Scheduler *scheduler = Director::getInstance()->getScheduler();
		scheduler->scheduleUpdate(netUpdator, 0, false);
	}
}

void NetUpdater::update(float delta)
{
	CNetManager::GetInstance().Update();
}
