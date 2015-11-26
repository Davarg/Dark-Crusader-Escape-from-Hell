#ifndef __BACKGROUND_ELEMENT_MAIN_GAME_H__
#define __BACKGROUND_ELEMENT_MAIN_GAME_H__

#include <cocos2d.h>
#include <Box2D\Box2D.h>
#include "UI\AbstractElementUI.h"

USING_NS_CC;

class BackgroundElementMainGame : public AbstractElementUI {
private:
	BackgroundElementMainGame& operator=(const BackgroundElementMainGame&);
	BackgroundElementMainGame(const BackgroundElementMainGame&);
	static const std::string backgroundImgPath;
	static const uint16 bottomCategoryBits;
	static const std::string elementName;
	Layer *_layerParent;
	b2Body *_bodyGlass;
	Layer *_layerBack;
	bool _isDisabled;

	void createGlass();

public:
	BackgroundElementMainGame(Layer*, Size);

	virtual void show() override;
	virtual void disable(bool) override;
	Layer* getBackLayer() { return _layerBack; }
	virtual bool isDisabled() const override { return _isDisabled; }
	static uint16 getBottomCategoryBits() { return bottomCategoryBits; }
	static const std::string getElementName() { return BackgroundElementMainGame::elementName; }
	virtual const std::string getName() const override { return BackgroundElementMainGame::elementName;	}
};

#endif