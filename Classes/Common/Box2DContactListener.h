#ifndef __BOX2D_CONTACT_LISTENER_H__
#define __BOX2D_CONTACT_LISTENER_H__

#include <cocos2d.h>
#include <Box2D\Box2D.h>
#include "Common\MessagesQueue.h"

USING_NS_CC;

class Box2DContactListener : public b2ContactListener {
private:
	std::vector<b2Contact*> _contacts;
	
	void contactPlayerPlatform(b2Contact*, b2Fixture*, b2Fixture*);

public:
	virtual void PreSolve(b2Contact*, const b2Manifold*) override;
	virtual void BeginContact(b2Contact*) override;
	virtual void EndContact(b2Contact*) override;
};

#endif