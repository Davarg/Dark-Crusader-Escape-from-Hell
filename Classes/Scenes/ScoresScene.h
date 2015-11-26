#ifndef __SCORES_SCENE_H__
#define __SCORES_SCENE_H__

#include <cocos2d.h>
#include "Common\MessagesQueue.h"
#include <ui\UIEditBox\UIEditBox.h>

USING_NS_CC;

class ScoresScene : public Layer {
private:
	class ScoresSceneDelegate : public ui::EditBoxDelegate {
	public:
		virtual void editBoxReturn(ui::EditBox* editBox) override;
	};

	static std::vector<std::tuple<int, bool, std::string, int> > dbRowsPreparedForDeletion;
	MessagesQueue::WrapperMessageQueueCallback_1 *_createTableCallback;
	static std::vector<std::tuple<int, bool, std::string, int> > dbRows;
	static const std::string labelHightScoresName;
	static const std::string pathToBackEditBox;
	ScoresSceneDelegate _editBoxDelegate;
	static const std::string pathToBack;
	static ScoresScene *scoresScene;
	ui::EditBox *_editBox;
	bool _isGameOver;
	int _tmpScores;

	void sortAndDeleteDbRows();

public:
	~ScoresScene();

	bool init();
	void writeScoresToDb();
	CREATE_FUNC(ScoresScene);
	void createScoresTableCallback(void*);
	static int selectDbCallback(void*, int, char**, char**);
	static ScoresScene *getInstance() { return scoresScene; }
	static Scene* createScene(int scores = 0, bool isGameOver = true);
};

#endif