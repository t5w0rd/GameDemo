#include "CommHeader.h"

#include "AppDelegate.h"
#include "BattleScene.h"
#include "LuaBinding.h"
#include "LuaBindingForCC.h"
#include "TestScene.h"
#include "LogoScene.h"
#include "MainMenuScene.h"


// AppDelegate
AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    pDirector->setContentScaleFactor(1.0);

    // create a scene. it's an autorelease object
#if GD_UNPACK
    CCScene* pScene = CCTestSceneLayer::scene();
#else
    CCScene* pScene = CCMainMenuSceneLayer::scene();
    //CCScene* pScene = CCBattleSceneLayer::scene();
    //CCScene* pScene = CCLogoSceneLayer::scene();
    
#endif
    if (pScene == NULL)
    {
        exit(1);
        return false;
    }

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
