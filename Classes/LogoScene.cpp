#include "CommHeader.h"

#include "LogoScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "ActionForCC.h"
#include "BattleScene.h"
#include "HeroRoomScene.h"
#include "MainMenuScene.h"


// CCLogoSceneLayer
CCLogoSceneLayer::CCLogoSceneLayer()
    : m_cur(0)
{
}

CCLogoSceneLayer::~CCLogoSceneLayer()
{
}

CCScene* CCLogoSceneLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene* pScene = CCScene::create();

    CCLogoSceneLayer* layer = CCLogoSceneLayer::create();

    // add layer as a child to scene
    if (layer != NULL)
    {
        M_DEF_GC(gc);
        gc->preloadSound("sounds/Effect/archmage_attack_heavy.mp3");
        gc->preloadSound("sounds/Effect/hero_priest_consecrate.mp3");
        //gc->preloadMusic("sounds/Background/Logo.mp3");

        layer->setOpacity(0);
        pScene->addChild(layer);
        layer->runAction(CCSequence::createWithTwoActions(CCFadeIn::create(0.5f), CCCallFuncN::create(layer, callfuncN_selector(CCLogoSceneLayer::onEffectUpdate))));
        gc->playMusic("sounds/Background/Logo.mp3", false);
    }

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool CCLogoSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        return false;
    }

    return true;
}

void CCLogoSceneLayer::onEffectUpdate(CCNode* pNode)
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    CCSprite* sp = NULL;
    float scale = 0.0f;
    M_DEF_GC(gc);

    switch (m_cur)
    {
    case 0:
        sp = CCSprite::create("UI/TorchWood.png");
        addChild(sp, 1, 1001);
        sp->setOpacity(0);
        scale = wsz.width / sp->getContentSize().width * 0.75 * 0.6;
        sp->setScale(scale * 6);
        sp->setPosition(ccp(wsz.width * 0.5, wsz.height * 0.5));
        sp->runAction(
            CCSpawn::create(
            CCScaleTo::create(0.2f, scale),
            CCFadeIn::create(0.2f),
            NULL));

        sp = CCSprite::create("UI/TorchWood.png");
        addChild(sp);
        sp->setOpacity(20);
        sp->setScale(scale * 8);
        sp->setPosition(ccp(wsz.width * 0.5, wsz.height * 0.5));
        sp->runAction(
            CCSequence::create(
                CCDelayTime::create(0.2f),
                CCSpawn::create(
                    CCScaleTo::create(0.2f, scale),
                    CCFadeTo::create(0.2f, 32),
                    NULL),
                CCRemoveSelf::create(),
                NULL));

        runAction(
            CCSequence::create(
                CCDelayTime::create(1.0f),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoSceneLayer::onEffectUpdate)),
                NULL));

        gc->playSound("sounds/Effect/archmage_attack_heavy.mp3");
        break;

    case 1:
        getChildByTag(1001)->runAction(
            CCSequence::create(
                CCDelayTime::create(1.6f),
                CCEaseExponentialOut::create(CCMoveBy::create(0.5f, ccp(-wsz.width * 0.75 * 0.2, 0.0f))),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoSceneLayer::onEffectUpdate)),
                NULL));
        break;

    case 2:
        sp = CCSprite::create("UI/Studio.png");
        addChild(sp, 0, 1002);
        sp->setPosition(ccp(wsz.width * 0.8 * 0.91, wsz.height * 0.5));
        sp->setOpacity(0);
        scale = getChildByTag(1001)->getScale();
        sp->setScale(scale);
        sp->runAction(
            CCSequence::create(
                CCEaseExponentialOut::create(CCFadeIn::create(1.0f)),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoSceneLayer::onEffectUpdate)),
                NULL));
        break;

    case 3:
        runAction(
            CCSequence::create(
                CCDelayTime::create(2.0f),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoSceneLayer::onEffectUpdate)),
                NULL));
        break;

    case 4:
        getChildByTag(1001)->runAction(CCFadeOut::create(0.7f));
        getChildByTag(1002)->runAction(CCFadeOut::create(0.7f));

        runAction(
            CCSequence::create(
                CCFadeOut::create(0.7f),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoSceneLayer::onEffectUpdate)),
                NULL));

        gc->playSound("sounds/Effect/GUITransitionOpen.mp3");
        break;

    case 5:
        sp = CCSprite::create("UI/Loading.png");
        addChild(sp);
        sp->setScale(wsz.width / sp->getContentSize().width * 0.3f);
        sp->setPosition(ccp(wsz.width * 0.5, wsz.height * 0.5));

        runAction(
            CCSequence::create(
                CCTintTo::create(0.5f, 0, 0, 0),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoSceneLayer::onEffectUpdate)),
                NULL));

        break;

    case 6:
        gc->loadFrames("Global0");
        gc->loadFrames("Global1");
        gc->loadFrames("Heroes0");
        gc->loadFrames("Heroes1");
        gc->loadFrames("Heroes2");
        gc->loadFrames("Heroes3");
        gc->loadFrames("Heroes4");
        gc->loadFrames("Heroes5");
        gc->loadFrames("Projectiles0");
        gc->preloadSound("sounds/Effect/Sound_Sheep.mp3");
        gc->preloadSound("sounds/Effect/inapp_atfreezeend.mp3");
        gc->preloadSound("sounds/Effect/hero_dragon_spit.mp3");
        gc->preloadSound("sounds/Effect/hero_dragon_birth.mp3");
        gc->preloadSound("sounds/Effect/ThunderCap.mp3");
        gc->preloadSound("sounds/Effect/KRF_sfx_minotauro_grito.mp3");
        gc->preloadSound("sounds/Effect/KRF_sfx_minotauro_carga.mp3");
        gc->preloadSound("sounds/Effect/HammerThrow.mp3");
        gc->preloadSound("sounds/Effect/HeadlessHorseman-03a_WET.mp3");
        gc->preloadSound("sounds/Effect/KidnapGrab.mp3");
        gc->preloadSound("sounds/Effect/Sound_FireballHit.mp3");
        gc->preloadSound("sounds/Effect/GUITransitionClose.mp3");
        gc->preloadSound("sounds/Effect/Sound_GUIButtonCommon.mp3");
        gc->preloadSound("sounds/Effect/GUITransitionOpen.mp3");
        gc->preloadMusic("sounds/Background/savage_music_desert_preparation.mp3");
        gc->preloadMusic("sounds/Background/Desert_Battle.mp3");
        
        runAction(
            CCSequence::create(
                CCDelayTime::create(2.0f),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoSceneLayer::onEffectUpdate)),
                NULL));

        break;

    case 7:
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, CCLogoScene2Layer::scene()));
        break;

    }
    ++m_cur;
    
}

// CCLogoScene2Layer
CCLogoScene2Layer::CCLogoScene2Layer()
    : m_ctrl(NULL)
    , m_cur(0)
{
}

CCLogoScene2Layer::~CCLogoScene2Layer()
{
}

CCScene* CCLogoScene2Layer::scene()
{
    // 'scene' is an autorelease object
    CCScene* pScene = CCScene::create();

    CCLogoScene2Layer* layer = CCLogoScene2Layer::create();

    // add layer as a child to scene
    if (layer != NULL)
    {
        layer->setOpacity(0);
        pScene->addChild(layer);
        pScene->addChild(layer->m_ctrl, 20);
    }

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool CCLogoScene2Layer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 255)))
    {
        return false;
    }

    setAnchorPoint(ccp(0.5f, 0.5f));
    setScale(1.0f);

    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    m_cur = 0;
    
    CCSprite* sp = CCSprite::create("backgrounds/BackgroundHD00.png");
    sp->setScale(1.5f);
    addChild(sp);
    sp->setPosition(ccp(wsz.width * 0.5, wsz.height - sp->getContentSize().height * 0.5 - (sp->getScale() - 1.0) * 0.5 * sp->getContentSize().height));
    
    CCEffect* eff = CCEffect::create("Effects/Kidnap/WomanMove", 0.1f);
    addChild(eff);
    eff->setTag(1006);
    eff->setPosition(ccp(wsz.width * 0.5, wsz.height * 0.5));

    eff->playForever();
    eff->runAction(
        CCSequence::create(
            CCMoveBy::create(2.0f, ccp(100.0f, 0.0f)),
            CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
            NULL));

    runAction(CCScaleTo::create(15.0f, 2.0f));

    eff = CCEffect::create("Effects/Sheep/Move", 0.1f);
    eff->addAnimation("Effects/Sheep/Die", 0.1f);
    addChild(eff);
    eff->setTag(1000);
    eff->setPosition(ccp(wsz.width * 0.9, wsz.height * 0.7 + 20));
    eff->playForever();
    eff->setFlipX(true);
    eff->runAction(CCMoveTo::create(10.0f, ccp(wsz.width * 0.5, wsz.height * 0.6)));

    M_DEF_GC(gc);
    gc->playMusic("sounds/Background/savage_music_desert_preparation.mp3");

    m_ctrl = CCLayer::create();
    CCMenu* mn = CCMenu::create();
    m_ctrl->addChild(mn);
    mn->setPosition(CCPointZero);
    CCMenuItemLabel* lbl = CCMenuItemLabel::create(CCLabelTTF::create("Skip", FONT_COMIC_BOOK, 48), this, menu_selector(CCLogoScene2Layer::onClickSkip));
    mn->addChild(lbl);
    lbl->setColor(ccc3(41, 57, 85));
    lbl->setPosition(ccp(wsz.width - lbl->getContentSize().width * 0.5 - 50, lbl->getContentSize().height + 50));

    return true;
}

void CCLogoScene2Layer::onEffectUpdate( CCNode* pNode )
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    CCEffect* eff = NULL;
    CCEffect* eff2 = NULL;
    CCSprite* sp = NULL;
    M_DEF_GC(gc);

    switch (m_cur)
    {
    case 0:  // 女人站住
        eff = (CCEffect*)pNode;
        eff->stopAllActions();
        eff->runAction(
            CCSequence::create(
                CCDelayTime::create(3.0f),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                CCMoveBy::create(4.0f, ccp(-200.0f, 0.0f)),
                NULL));
        break;

    case 1:  // 女人左转
        eff = (CCEffect*)pNode;
        eff->setFlipX(true);
        eff->playForever();
        // 生成绑匪
        eff = CCEffect::create("Effects/Kidnap/Move", 0.05f);
        eff->addAnimation("Effects/Kidnap/Grab", 0.1f);
        eff->addAnimation("Effects/Kidnap/KidnapMove", 0.05f);
        eff->addAnimation("Effects/Kidnap/Die", 0.1f);
        addChild(eff, 1);
        eff->setTag(1004);
        eff->setPosition(ccp(wsz.width + eff->getContentSize().width, wsz.height * 0.5 + 20));
        eff->setFlipX(true);
        eff->playForever(0, 2.0f);
        eff->runAction(
            CCSequence::create(
                CCMoveTo::create(2.0f, ccp(((CCEffect*)pNode)->getPositionX() - 50.0f, eff->getPositionY())),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                NULL));

        break;
        
    case 2:  // 抓取女人
        getChildByTag(1006)->stopAllActions();
        ((CCSprite*)getChildByTag(1006))->setFlipX(false);
        eff = (CCEffect*)pNode;
        eff->play(1, 1.0f, 1, this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate));
        getChildByTag(1006)->runAction(
            CCSequence::create(
                CCDelayTime::create(0.6f),
                CCRemoveSelf::create(),
                NULL
            ));

        eff = (CCEffect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Effects/Sheep/Move/03.png"));

        stopAllActions();
        runAction(CCScaleTo::create(0.3f, 2.0f));
        gc->playSound("sounds/Effect/KidnapGrab.mp3");
        break;

    case 3:
        eff = (CCEffect*)pNode;
        gc->playMusic("sounds/Background/Desert_Battle.mp3");
        eff->runAction(
            CCSequence::create(
            CCDelayTime::create(1.0f),
            CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
            NULL));
        gc->playSound("sounds/Effect/HeadlessHorseman-03a_WET.mp3");
        break;

    case 4:  // 扛女人跑
        eff = (CCEffect*)pNode;
        eff->playForever(2, 1.5f);
        eff->runAction(
            CCSequence::create(
                CCMoveTo::create(2.0f, ccp(-eff->getContentSize().width, eff->getPositionY())),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                NULL));

        runAction(CCEaseExponentialOut::create(CCScaleTo::create(2.0f, 1.0f)));
        gc->playSound("sounds/Effect/Sound_Sheep.mp3");
        break;

    case 5:  // 牛出现
        // Help
        eff = CCEffect::create("Effects/Help", 0.1f);
        addChild(eff);
        eff->setScale(2.0f);
        eff->setPosition(ccp(eff->getContentSize().width, wsz.height * 0.5 + 100));
        eff->playRelease(0, 2.0f, 6);

        // 生成牛
        eff = CCEffect::create("Effects/Rush/Move", 0.1f);
        eff->addAnimation("Effects/Rush/Cry", 0.1f);
        eff->addAnimation("Effects/Rush/Rush", 0.1f);
        addChild(eff);
        eff->setPosition(ccp(800 + wsz.width + eff->getContentSize().width, wsz.height * 0.5));
        eff->setFlipX(true);

        eff->playForever(0, 2.0f);
        eff->runAction(
            CCSequence::create(
                CCMoveTo::create(5.0f, ccp(wsz.width * 0.8, wsz.height * 0.5)),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                NULL));

        eff = CCEffect::create("Effects/Soldier/Move", 0.08f);
        addChild(eff);
        eff->setFlipX(true);
        eff->setTag(1101);
        eff->setPosition(ccp(wsz.width * 0.5, wsz.height + 400));
        eff->playForever();
        eff->runAction(CCMoveTo::create(10.0f, ccp(wsz.width * 0.5 - 100, wsz.height * 0.6)));

        eff = CCEffect::create("Units/Thor/move", 0.08f);
        eff->addAnimation("Units/Thor/act3", 0.1f);
        addChild(eff, 5);
        eff->setTag(1102);
        eff->setPosition(ccp(wsz.width * 0.5, wsz.height + 400));
        eff->playForever();
        eff->runAction(CCMoveTo::create(10.0f, ccp(wsz.width * 0.5, wsz.height * 0.6)));

        eff = CCEffect::create("Effects/Soldier/Move", 0.08f);
        addChild(eff);
        eff->setTag(1103);
        eff->setPosition(ccp(wsz.width * 0.5, wsz.height + 400));
        eff->playForever();
        eff->runAction(CCMoveTo::create(10.0f, ccp(wsz.width * 0.5 + 100, wsz.height * 0.6)));

        eff = (CCEffect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->playForever();
        eff->setFlipX(false);
        eff->runAction(CCMoveTo::create(5.0f, ccp(wsz.width * 0.7, wsz.height * 0.5)));
        break;

    case 6:  // 牛咆哮
        eff = (CCEffect*)pNode;
        eff->play(1, 1.0f, 1, this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate));

        eff = (CCEffect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Effects/Sheep/Move/03.png"));
        
        gc->playSound("sounds/Effect/KRF_sfx_minotauro_grito.mp3");
        break;

    case 7:  // 牛冲刺
        eff = (CCEffect*)pNode;
        eff2 = CCEffect::create("Effects/Rush/RushEffect", 0.1f);
        eff->addChild(eff2);
        eff2->setPosition(ccp(eff->getContentSize().width - 60, eff->getContentSize().height * 0.5 - 55));
        eff2->setFlipX(true);

        eff->playForever(2, 1.5f);
        eff->runAction(
            CCSpawn::create(
                CCSequence::create(
                    CCEaseExponentialIn::create(CCMoveTo::create(2.0f, ccp(-eff->getContentSize().width - 2000, wsz.height * 0.5))),
                    CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                    NULL),
                CCSequence::create(
                    CCDelayTime::create(1.5f),
                    CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onSoldierTurn)),
                    NULL),
                NULL));

        eff2->playForever();

        eff = (CCEffect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->playForever();
        eff->setFlipX(false);
        eff->runAction(CCMoveTo::create(2.0f, ccp(wsz.width * 0.8, wsz.height * 0.6)));
        gc->playSound("sounds/Effect/KRF_sfx_minotauro_carga.mp3");
        break;

    case 8:  // 撞上
        runAction(
            CCSequence::create(
                CCShake::create(0.2f, 5, 30.0f),
                NULL));

        eff = (CCEffect*)getChildByTag(1004);
        eff->stopAllActions();
        eff->setFlipX(false);
        eff->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("Effects/Kidnap/Fly.png"));
        eff->runAction(
            CCSpawn::create(
                CCSequence::create(
                    CCJumpTo::create(2.0, ccp(wsz.width * 0.9, wsz.height * 0.5 + 20), 500.0f, 1),
                    CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                    NULL),
                CCSequence::create(
                    CCDelayTime::create(1.3f),
                    CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onSoldierTurn)),
                    NULL),
                NULL));
        eff->runAction(CCRepeatForever::create(CCRepeatForever::create(CCRotateBy::create(1.0f, 1800.0f))));

        getChildByTag(1101)->stopAllActions();
        getChildByTag(1102)->stopAllActions();
        getChildByTag(1103)->stopAllActions();

        eff = (CCEffect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->playForever();
        eff->setFlipX(false);
        eff->runAction(CCMoveTo::create(2.0f, ccp(wsz.width * 0.9, wsz.height * 0.5 - 50)));
        gc->playSound("sounds/Effect/inapp_atfreezeend.mp3");
        break;

    case 9:  // 落地
        runAction(
            CCSequence::create(
            CCShake::create(0.2f, 5, 30.0f),
            NULL));

        eff = (CCEffect*)getChildByTag(1004);
        eff->stopAllActions();
        eff->play(3, 1.0f, 1, this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate));
        eff->setRotation(0.0f);
        
        sp = CCSprite::createWithSpriteFrameName("Effects/Kidnap/Bang.png");
        addChild(sp);
        sp->setScale(4.0f);
        sp->setPosition(ccpAdd(eff->getPosition(), ccp(30.0f, -60.0f)));
        sp->runAction(CCFadeOut::create(3.0f));

        eff = CCEffect::create("Effects/Bang", 0.1f);
        addChild(eff);
        eff->setScale(1.0f);
        eff->setPosition(ccpAdd(sp->getPosition(), ccp(0.0f, 0.0f)));
        eff->playRelease();

        eff = (CCEffect*)getChildByTag(1000);
        eff->play(1);

        gc->playSound("sounds/Effect/Sound_FireballHit.mp3");
        break;

    case 10:  // 军官向前
        eff = ((CCEffect*)getChildByTag(1102));
        eff->playForever(0);
        eff->runAction(
            CCSequence::create(
                CCMoveTo::create(3.0f, ccp(wsz.width * 0.7, wsz.height * 0.5f)),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                NULL));
        break;

    case 11:  // 投掷锤子
        eff = (CCEffect*)pNode;
        eff->setFlipX(true);
        eff->play(1);
        eff->runAction(
            CCSequence::create(
                CCDelayTime::create(0.5f),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                NULL));
        
        break;

    case 12:
        eff = (CCEffect*)getChildByTag(1102);
        sp = CCSprite::createWithSpriteFrameName("Units/ThorHammer/default.png");
        addChild(sp, 6);
        sp->setPosition(ccpAdd(eff->getPosition(), ccp(-100.0f, 0.0f)));
        sp->setFlipX(true);

        sp->runAction(
            CCSequence::create(
                CCEaseExponentialIn::create(
                CCSpawn::create(
                    CCMoveTo::create(0.3f, ccp(wsz.width * 0.5, wsz.height * 0.5)),
                    CCScaleTo::create(0.3f, 100.f),
                    NULL)),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                NULL));

        gc->playSound("sounds/Effect/HammerThrow.mp3");
        break;

    case 13:
        runAction(
            CCSequence::create(
                CCShake::create(0.6f, 10, 100.0f),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                NULL));
        gc->playSound("sounds/Effect/ThunderCap.mp3");
        break;

    case 14:
        pNode = CCLayerColor::create(ccc4(0, 0, 0, 0));
        getParent()->addChild(pNode, 10);
        sp = CCSprite::create("UI/Loading.png");
        pNode->addChild(sp);
        sp->setScale(wsz.width / sp->getContentSize().width * 0.3f);
        sp->setPosition(ccp(wsz.width * 0.5, wsz.height * 0.5));
        pNode->runAction(
            CCSequence::create(
                CCFadeIn::create(0.3f),
                CCCallFuncN::create(this, callfuncN_selector(CCLogoScene2Layer::onEffectUpdate)),
                NULL));

        gc->playSound("sounds/Effect/GUITransitionOpen.mp3");
        m_ctrl->setVisible(false);
        break;

    case 15:
        //CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, CCHeroRoomSceneLayer::scene(), ccWHITE));
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, CCMainMenuSceneLayer::scene(), ccWHITE));
        break;
    }

    ++m_cur;
}

void CCLogoScene2Layer::onSoldierTurn( CCNode* pNode )
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    static bool first = true;
    M_DEF_GC(gc);

    if (first)
    {
        first = false;
        ((CCSprite*)getChildByTag(1101))->setFlipX(true);
        ((CCSprite*)getChildByTag(1102))->setFlipX(true);
        ((CCSprite*)getChildByTag(1103))->setFlipX(true);
        gc->playSound("sounds/Effect/hero_dragon_birth.mp3");
        
    }
    else
    {
        ((CCSprite*)getChildByTag(1101))->setFlipX(false);
        ((CCSprite*)getChildByTag(1102))->setFlipX(false);
        ((CCSprite*)getChildByTag(1103))->setFlipX(false);
        gc->playSound("sounds/Effect/hero_dragon_spit.mp3");
    }
    
}

void CCLogoScene2Layer::onClickSkip( CCObject* pObject )
{
    m_cur = 14;
    m_ctrl->setVisible(false);
}

