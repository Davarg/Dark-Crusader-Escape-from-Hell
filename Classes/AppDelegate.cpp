#include "AppDelegate.h"
#include "Scenes\SplashScene.h"
#include "Common\MessagesQueue.h"
#include "Common\ConstantsRegistry.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::ROTATION_X_3D, -84);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::ROTATION_Y_3D, -20);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::ROTATION_Z_3D, 0);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::SPRITE_SCALE, 25);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::SCALE_RATIO_BOX2D, 32);
	ConstantsRegistry::setValueForKey(ConstantsRegistry::constants::GRAVITY_SCALE_Y, -6.8f);
	ConstantsRegistry::lock();

    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	auto sizeFrame = new Size(1024, 768);

    if(!glview) {
		glview = GLViewImpl::create("Dark Crusader: Hell Escape");
		glview->setFrameSize(sizeFrame->width, sizeFrame->height);
        director->setOpenGLView(glview);
		glview->setDesignResolutionSize(sizeFrame->width, sizeFrame->height, ResolutionPolicy::EXACT_FIT);
    }

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);

	Director::getInstance()->getScheduler()->scheduleUpdate(MessagesQueue::getInstance(), 1, false);
	_scenesManager = ScenesManager::getInstance();
	MessagesQueue::WrapperMessageQueueCallback_1 callback(CC_CALLBACK_1(ScenesManager::startMenuCallback, _scenesManager)
		, "ScenesManagerPosInitScenes");
	MessagesQueue::getInstance()->addListener(MessagesQueue::MessageType::INIT_SCENES, callback);
	auto scene = _scenesManager->getStartScene();

    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
