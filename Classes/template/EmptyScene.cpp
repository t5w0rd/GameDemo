#include "CommHeader.h"

#include "EmptyScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"



// CCEmptySceneLayer
CCEmptySceneLayer::CCEmptySceneLayer()
{
}

CCEmptySceneLayer::~CCEmptySceneLayer()
{
}

CCScene* CCEmptySceneLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene* pScene = CCScene::create();

    CCEmptySceneLayer* layer = CCEmptySceneLayer::create();

    // add layer as a child to scene
    if (layer != NULL)
    {
        M_DEF_GC(gc);
        //gc->preloadSound("sounds/Effect/xxxxxxx.mp3");

        pScene->addChild(layer);
    }

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool CCEmptySceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        return false;
    }
    
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);
    CCSprite* sp = NULL;

    return true;
}


