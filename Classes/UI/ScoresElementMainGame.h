#ifndef __SCORES_ELEMENT_MAIN_GAME_H__
#define __SCORES_ELEMENT_MAIN_GAME_H__

#include <cocos2d.h>
#include "AbstractElementUI.h"
#include "Common\MessagesQueue.h"

USING_NS_CC;

class ScoresElementMainGame : public AbstractElementUI {
private:
	MessagesQueue::WrapperMessageQueueCallback_1 *_updateScoresCallback;
	ScoresElementMainGame& operator=(const ScoresElementMainGame&);
	ScoresElementMainGame(const ScoresElementMainGame&);
	static const std::string backgroundImgPath;
	static const std::string elementName;
	Layer *_layerParent;
	Label *_labelScores;
	Layer *_layerBack;
	bool _isDisabled;
	int _scores;

public:
	ScoresElementMainGame(Layer*, Size);
	~ScoresElementMainGame();

	virtual void show() override;
	void updateScoresCallback(void*);
	int getScores() { return _scores; }
	virtual void disable(bool) override;
	virtual bool isDisabled() const override { return _isDisabled; }
	static const std::string getElementName() { return elementName; }
	virtual const std::string getName() const override { return elementName; }
};

#endif