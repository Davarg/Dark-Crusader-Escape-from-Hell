#ifndef __USER_INTERFACE_MAIN_GAME_H__
#define __USER_INTERFACE_MAIN_GAME_H__

#include <cocos2d.h>
#include "UI\AbstractElementUI.h"
#include "UI\AbstractUserInterface.h"

USING_NS_CC;

class UserInterfaceMainGame : public AbstractUserInterface {
private:
	UserInterfaceMainGame& operator=(const UserInterfaceMainGame&);
	UserInterfaceMainGame(const UserInterfaceMainGame&);
	std::vector<AbstractElementUI*> _childrens;
	Layer *_layerParent;
	

public:
	UserInterfaceMainGame(Layer*, Size);
	~UserInterfaceMainGame();

	virtual void show() override;
	virtual AbstractElementUI* getChildrenByName(const std::string) override;
};
#endif