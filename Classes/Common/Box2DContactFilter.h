#ifndef __BOX2D_CONTACT_FILTER_H__
#define __BOX2D_CONTACT_FILTER_H__

#include <cocos2d.h>
#include <Box2D\Box2D.h>

USING_NS_CC;

class Box2DContactFilter : public b2ContactFilter{
public:
	virtual bool ShouldCollide(b2Fixture*, b2Fixture*) override;
};

#endif