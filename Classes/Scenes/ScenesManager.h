#ifndef __SCENES_MANAGER_H__
#define __SCENES_MANAGER_H__

#include <cocos2d.h>
#include <SimpleAudioEngine.h>
#include "Common\MessagesQueue.h"

USING_NS_CC;

class ScenesManager {
private:
	MessagesQueue::WrapperMessageQueueCallback_1 *_callBackPlayerDie;
	static const std::string pathToBackgroundMscMainGame;
	static const std::string pathToBackgroundMscMenu;
	static ScenesManager *scenesManager;
	void *_sharedCallbackArgs;

	Scene* createStartMenu();

public:
	ScenesManager() : _sharedCallbackArgs(nullptr) {}

	~ScenesManager();

	Scene* getStartScene();
	void startMenuCallback(void*);
	void buttonPlayCallback(Ref*);
	void buttonExitCallback(Ref*);
	void playerDiedCallback(void*);
	void buttonScoresCallback(Ref*);
	void releaseSceneCallback(void*);
	void buttonOptionsCallback(Ref*);
	void buttonMusicOnCallback(Ref*);
	void buttonSoundOnCallback(Ref*);
	void buttonReturnOptionsCallback(Ref*);
	void buttonReturnGameOverCallback(Ref*);
	static ScenesManager* getInstance() { return scenesManager; }
};

#endif