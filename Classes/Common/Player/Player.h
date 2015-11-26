#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include <cocos2d.h>
#include <Box2D\Box2D.h>

USING_NS_CC;

class Player {
private:
	static const std::string playerTexture;
	static const std::string playerC3B;
	Sprite3D *_sprite;
	b2Body *_physBody;
	bool _isJumped;

public:
	enum PlayerCategoryBits {
		PLAYER_CATEGORY_BITS = 0x0020
	};

	Player();

	void setPositionPxl(Vec3);
	bool isJumped() { return _isJumped; }
	Sprite3D* getSprite() { return _sprite; }
	b2Body* getPhysBody() { return _physBody; }
	void isJumped(bool flag) { _isJumped = flag; }
};

#endif