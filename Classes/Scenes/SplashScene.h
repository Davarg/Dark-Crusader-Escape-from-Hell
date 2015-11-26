#ifndef __SPLASH_SCREEN_H__
#define __SPLASH_SCREEN_H__

#include <cocos2d.h>

USING_NS_CC;

class SplashScene : public Layer {
private:
	static const std::string pathToBack;

public:
	CREATE_FUNC(SplashScene);
	static Scene* createScene();
	virtual bool init() override;
	virtual void onEnter() override;
};
#endif 