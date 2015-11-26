#ifndef __OPTIONS_SCENE_H__
#define __OPTIONS_SCENE_H__

#include <cocos2d.h>

USING_NS_CC;

class OptionsScene : public Layer {
private:
	static const std::string pathToBackgroundImg;
	Menu *_optionsMenu;

public:
	CREATE_FUNC(OptionsScene);
	static Scene* createScene();
	virtual bool init() override;
};

#endif