#ifndef __PLATFORM_MANAGER_H__
#define __PLATFORM_MANAGER_H__

#include <cocos2d.h>
#include <Box2D\Box2D.h>
#include "Common\MessagesQueue.h"
#include "Common\PlatformBuilder.h"

USING_NS_CC;

class PlatformManager {
private:
	MessagesQueue::WrapperMessageQueueCallback_1 *_deletePlatformCallback;
	PlatformBuilder::Platform *_lastPlatform;
	Size _minSizeBetweenPlatforms; //Pxl
	Size _maxSizeBetweenPlatforms; //Pxl
	int _platformRequestCount;
	int _minGenPlatformCount;
	int _platformCount;
	Layer *_lastLayer;

public:
	PlatformManager();
	~PlatformManager();

	void addPlatforms(Layer*, int);
	void playerJumpCallback(void*);
	void platformDestroyCallback(void*);
};

#endif