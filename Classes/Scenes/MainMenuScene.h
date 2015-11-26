#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include <cocos2d.h>
#include <SimpleAudioEngine.h>

USING_NS_CC;

class MainMenuScene : public Layer {
private:
	static const std::string pathToBackgroundImg;
	Menu *_mainMenu;

public:
	CREATE_FUNC(MainMenuScene);
	static Scene* createScene();
	virtual bool init() override;
};

#endif