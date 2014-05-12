#include "CommHeader.h"

#include "EmptyScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"


// EmptySceneLayer
EmptySceneLayer::EmptySceneLayer()
{
}

EmptySceneLayer::~EmptySceneLayer()
{
}

Scene* EmptySceneLayer::scene()
{
    // 'scene' is an autorelease object
    Scene* pScene = Scene::create();

    EmptySceneLayer* layer = EmptySceneLayer::create();

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
bool EmptySceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }
    
    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);
    Sprite* sp = NULL;

    return true;
}


