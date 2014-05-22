#include "CommHeader.h"

#include "AppDelegate.h"
#include "BattleScene.h"
#include "MainMenuScene.h"
#include "LogoScene.h"
#include "StageScene.h"
#include "AbilityScene.h"
#include "EmptyScene.h"
#include "GameData.h"


// AppDelegate
AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    str_normalize_init();

    // initialize director
    auto director = Director::getInstance();
    //
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    director->setContentScaleFactor(1.0);

    Director::getInstance()->getScheduler()->schedule([](float)
    {
        CAutoReleasePool::instance()->releaseObjects();
    }, this, 0.1f, false, "AutoRelease");

    // create a scene. it's an autorelease object
#if GD_UNPACK
    auto scene = UnpackSceneLayer::scene();
#else
    CGameData::instance();
    CUserData::instance()->load("");
    //auto scene = BattleSceneLayer::scene();
    //auto scene = LogoSceneLayer::scene();
    //auto scene = HeroRoomSceneLayer::scene();
    //auto scene = StageSceneLayer::scene();
    //auto scene = MainMenuSceneLayer::scene();
    auto scene = AbilitySceneLayer::scene();
    //auto scene = EmptySceneLayer::scene();

#endif

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
}
