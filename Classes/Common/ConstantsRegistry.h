#ifndef __CONSTANTS_REGISTRY_H__
#define __CONSTANTS_REGISTRY_H__

#include <cocos2d.h>
#include <Box2D\Box2D.h>

USING_NS_CC;

class ConstantsRegistry {
private:
	bool _locked;
	std::map<int, float32> _values;
	static ConstantsRegistry _registry;

public:
	enum constants {
		SPRITE_SCALE,
		ROTATION_X_3D,
		ROTATION_Y_3D,
		ROTATION_Z_3D,
		GRAVITY_SCALE_Y,
		SCALE_RATIO_BOX2D
	};

	static float32 getValueForKey(constants);
	static void setValueForKey(constants, float32);
	static void lock() { _registry._locked = true; }
	static bool isLocked() { return _registry._locked; }
};

#endif