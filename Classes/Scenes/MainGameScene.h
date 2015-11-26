#ifndef __MAIN_GAME_SCENE_H__
#define __MAIN_GAME_SCENE_H___

#include <cocos2d.h>
#include <Box2D\Box2D.h>
#include <SimpleAudioEngine.h>
#include "Common\GLES-Render.h"
#include "Common\Player\Player.h"
#include "Common\MessagesQueue.h"
#include "Common\PlatformManager.h"
#include "UI\UserInterfaceMainGame.h"
#include "Common\Box2DContactFilter.h"
#include "Common\Player\PlayerInput.h"
#include "Common\Box2DContactListener.h"

USING_NS_CC;

class MainGameScene : public Layer {
private:
	MessagesQueue::WrapperMessageQueueCallback_1 *_playerJumpCallback;
	Box2DContactListener _contactListener;
	Box2DContactFilter _contactFilter;
	PlatformManager *_platformManager;
	static MainGameScene *gameScene;
	UserInterfaceMainGame *_ui;
	PlayerInput *_playerInput;
	Player *_player;
	b2World *_world;

#ifdef _DEBUG
	GLESDebugDraw *_debugDraw;
	virtual void draw(Renderer*, const Mat4&, uint32_t) override;
#endif

public:
	~MainGameScene();

	b2World* getWorld();
	CREATE_FUNC(MainGameScene);
	static Scene* createScene();
	virtual bool init() override;
	virtual void update(float dt) override;
	Player* getPlayer() { return _player; }
	UserInterfaceMainGame * getUI() { return _ui; }
	static MainGameScene* getInstance() { return gameScene; }
	PlatformManager* getPlatformManager() { return _platformManager; }
};

#endif
