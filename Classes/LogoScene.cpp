#include "CommHeader.h"

#include "LogoScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "ActionForCC.h"
#include "BattleScene.h"
#include "HeroRoomScene.h"
#include "MainMenuScene.h"


// LogoSceneLayer
LogoSceneLayer::LogoSceneLayer()
: m_cur(0)
{
}

LogoSceneLayer::~LogoSceneLayer()
{
}

Scene* LogoSceneLayer::scene()
{
    // 'scene' is an autorelease object
    Scene* pScene = Scene::create();

    LogoSceneLayer* layer = LogoSceneLayer::create();

    // add layer as a child to scene
    if (layer != nullptr)
    {
        M_DEF_GC(gc);
        gc->preloadSound("sounds/Effects/archmage_attack_heavy.mp3");
        gc->preloadSound("sounds/Effects/hero_priest_consecrate.mp3");
        //gc->preloadMusic("sounds/Backgrounds/Logo.mp3");

        layer->setOpacity(0);
        pScene->addChild(layer);
        layer->runAction(Sequence::createWithTwoActions(FadeIn::create(0.5f), CallFuncN::create(CC_CALLBACK_1(LogoSceneLayer::onEffectUpdate, layer))));
        gc->playMusic("sounds/Backgrounds/Logo.mp3", false);
    }

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool LogoSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
    {
        return false;
    }

    return true;
}

void LogoSceneLayer::onEffectUpdate(Node* pNode)
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    Sprite* sp = nullptr;
    float scale = 0.0f;
    M_DEF_GC(gc);

    switch (m_cur)
    {
    case 0:
        sp = Sprite::create("UI/TorchWood.png");
        addChild(sp, 1, 1001);
        sp->setOpacity(0);
        scale = wsz.width / sp->getContentSize().width * 0.75 * 0.6;
        sp->setScale(scale * 6);
        sp->setPosition(Point(wsz.width * 0.5, wsz.height * 0.5));
        sp->runAction(
            Spawn::create(
            ScaleTo::create(0.2f, scale),
            FadeIn::create(0.2f),
            nullptr));

        sp = Sprite::create("UI/TorchWood.png");
        addChild(sp);
        sp->setOpacity(20);
        sp->setScale(scale * 8);
        sp->setPosition(Point(wsz.width * 0.5, wsz.height * 0.5));
        sp->runAction(
            Sequence::create(
                DelayTime::create(0.2f),
                Spawn::create(
                    ScaleTo::create(0.2f, scale),
                    FadeTo::create(0.2f, 32),
                    nullptr),
                RemoveSelf::create(),
                nullptr));

        runAction(
            Sequence::create(
                DelayTime::create(1.0f),
                CallFuncN::create(CC_CALLBACK_1(LogoSceneLayer::onEffectUpdate, this)),
                nullptr));

        gc->playSound("sounds/Effects/archmage_attack_heavy.mp3");
        break;

    case 1:
        getChildByTag(1001)->runAction(
            Sequence::create(
                DelayTime::create(1.6f),
                EaseExponentialOut::create(MoveBy::create(0.5f, Point(-wsz.width * 0.75 * 0.2, 0.0f))),
                CallFuncN::create(CC_CALLBACK_1(LogoSceneLayer::onEffectUpdate, this)),
                nullptr));
        break;

    case 2:
        sp = Sprite::create("UI/Studio.png");
        addChild(sp, 0, 1002);
        sp->setPosition(Point(wsz.width * 0.8 * 0.91, wsz.height * 0.5));
        sp->setOpacity(0);
        scale = getChildByTag(1001)->getScale();
        sp->setScale(scale);
        sp->runAction(
            Sequence::create(
                EaseExponentialOut::create(FadeIn::create(1.0f)),
                CallFuncN::create(CC_CALLBACK_1(LogoSceneLayer::onEffectUpdate, this)),
                nullptr));
        break;

    case 3:
        runAction(
            Sequence::create(
                DelayTime::create(2.0f),
                CallFuncN::create(CC_CALLBACK_1(LogoSceneLayer::onEffectUpdate, this)),
                nullptr));
        break;

    case 4:
        getChildByTag(1001)->runAction(FadeOut::create(0.7f));
        getChildByTag(1002)->runAction(FadeOut::create(0.7f));

        runAction(
            Sequence::create(
                FadeOut::create(0.7f),
                CallFuncN::create(CC_CALLBACK_1(LogoSceneLayer::onEffectUpdate, this)),
                nullptr));

        gc->playSound("sounds/Effects/GUITransitionOpen.mp3");
        break;

    case 5:
        sp = Sprite::create("UI/Loading.png");
        addChild(sp);
        sp->setScale(wsz.width / sp->getContentSize().width * 0.3f);
        sp->setPosition(Point(wsz.width * 0.5, wsz.height * 0.5));

        runAction(
            Sequence::create(
                TintTo::create(0.5f, 0, 0, 0),
                CallFuncN::create(CC_CALLBACK_1(LogoSceneLayer::onEffectUpdate, this)),
                nullptr));

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
        gc->preloadSound("sounds/Effects/Sound_Sheep.mp3");
        gc->preloadSound("sounds/Effects/inapp_atfreezeend.mp3");
        gc->preloadSound("sounds/Effects/hero_dragon_spit.mp3");
        gc->preloadSound("sounds/Effects/hero_dragon_birth.mp3");
        gc->preloadSound("sounds/Effects/ThunderCap.mp3");
        gc->preloadSound("sounds/Effects/KRF_sfx_minotauro_grito.mp3");
        gc->preloadSound("sounds/Effects/KRF_sfx_minotauro_carga.mp3");
        gc->preloadSound("sounds/Effects/HammerThrow.mp3");
        gc->preloadSound("sounds/Effects/HeadlessHorseman-03a_WET.mp3");
        gc->preloadSound("sounds/Effects/KidnapGrab.mp3");
        gc->preloadSound("sounds/Effects/Sound_FireballHit.mp3");
        gc->preloadSound("sounds/Effects/GUITransitionClose.mp3");
        gc->preloadSound("sounds/Effects/Sound_GUIButtonCommon.mp3");
        gc->preloadSound("sounds/Effects/GUITransitionOpen.mp3");
        gc->preloadMusic("sounds/Backgrounds/savage_music_desert_preparation.mp3");
        gc->preloadMusic("sounds/Backgrounds/Desert_Battle.mp3");
        
        runAction(
            Sequence::create(
                DelayTime::create(2.0f),
                CallFuncN::create(CC_CALLBACK_1(LogoSceneLayer::onEffectUpdate, this)),
                nullptr));

        break;

    case 7:
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, LogoScene2Layer::scene()));
        break;

    }
    ++m_cur;
    
}

// LogoScene2Layer
LogoScene2Layer::LogoScene2Layer()
: m_ctrl(nullptr)
, m_cur(0)
{
}

LogoScene2Layer::~LogoScene2Layer()
{
}

Scene* LogoScene2Layer::scene()
{
    // 'scene' is an autorelease object
    Scene* pScene = Scene::create();

    LogoScene2Layer* layer = LogoScene2Layer::create();

    // add layer as a child to scene
    if (layer != nullptr)
    {
        layer->setOpacity(0);
        pScene->addChild(layer);
        pScene->addChild(layer->m_ctrl, 20);
    }

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool LogoScene2Layer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
    {
        return false;
    }

    setAnchorPoint(Point(0.5f, 0.5f));
    setScale(1.0f);

    static Size wsz = Director::getInstance()->getVisibleSize();
    m_cur = 0;
    
    Sprite* sp = Sprite::create("backgrounds/BackgroundHD00.png");
    sp->setScale(1.5f);
    addChild(sp);
    sp->setPosition(Point(wsz.width * 0.5, wsz.height - sp->getContentSize().height * 0.5 - (sp->getScale() - 1.0) * 0.5 * sp->getContentSize().height));
    
    Effect* eff = Effect::create("Effects/Kidnap/WomanMove", 0.1f);
    addChild(eff);
    eff->setTag(1006);
    eff->setPosition(Point(wsz.width * 0.5, wsz.height * 0.5));

    eff->playForever();
    eff->runAction(
        Sequence::create(
            MoveBy::create(2.0f, Point(100.0f, 0.0f)),
            CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
            nullptr));

    runAction(ScaleTo::create(15.0f, 2.0f));

    eff = Effect::create("Effects/Sheep/Move", 0.1f);
    eff->addAnimation("Effects/Sheep/Die", 0.1f);
    addChild(eff);
    eff->setTag(1000);
    eff->setPosition(Point(wsz.width * 0.9, wsz.height * 0.7 + 20));
    eff->playForever();
    eff->setFlippedX(true);
    eff->runAction(MoveTo::create(10.0f, Point(wsz.width * 0.5, wsz.height * 0.6)));

    M_DEF_GC(gc);
    gc->playMusic("sounds/Backgrounds/savage_music_desert_preparation.mp3");

    m_ctrl = Layer::create();
    Menu* mn = Menu::create();
    m_ctrl->addChild(mn);
    mn->setPosition(Point::ZERO);
    MenuItemLabel* lbl = MenuItemLabel::create(Label::createWithTTF("Skip", FONT_COMIC_BOOK, 48), CC_CALLBACK_1(LogoScene2Layer::onClickSkip, this));
    mn->addChild(lbl);
    lbl->setColor(Color3B(41, 57, 85));
    lbl->setPosition(Point(wsz.width - lbl->getContentSize().width * 0.5 - 50, lbl->getContentSize().height + 50));

    return true;
}

void LogoScene2Layer::onEffectUpdate(Node* pNode)
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    Effect* eff = nullptr;
    Effect* eff2 = nullptr;
    Sprite* sp = nullptr;
    M_DEF_GC(gc);

    switch (m_cur)
    {
    case 0:  // 女人站住
        eff = (Effect*)pNode;
        eff->stopAllActions();
        eff->runAction(
            Sequence::create(
                DelayTime::create(3.0f),
                CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                MoveBy::create(4.0f, Point(-200.0f, 0.0f)),
                nullptr));
        break;

    case 1:  // 女人左转
        eff = (Effect*)pNode;
        eff->setFlippedX(true);
        eff->playForever();
        // 生成绑匪
        eff = Effect::create("Effects/Kidnap/Move", 0.05f);
        eff->addAnimation("Effects/Kidnap/Grab", 0.1f);
        eff->addAnimation("Effects/Kidnap/KidnapMove", 0.05f);
        eff->addAnimation("Effects/Kidnap/Die", 0.1f);
        addChild(eff, 1);
        eff->setTag(1004);
        eff->setPosition(Point(wsz.width + eff->getContentSize().width, wsz.height * 0.5 + 20));
        eff->setFlippedX(true);
        eff->playForever(0, 2.0f);
        eff->runAction(
            Sequence::create(
                MoveTo::create(2.0f, Point(((Effect*)pNode)->getPositionX() - 50.0f, eff->getPositionY())),
                CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                nullptr));

        break;
        
    case 2:  // 抓取女人
        getChildByTag(1006)->stopAllActions();
        ((Sprite*)getChildByTag(1006))->setFlippedX(false);
        eff = (Effect*)pNode;
        eff->play(1, 1.0f, 1, CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)));
        getChildByTag(1006)->runAction(
            Sequence::create(
                DelayTime::create(0.6f),
                RemoveSelf::create(),
                nullptr));

        eff = (Effect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Effects/Sheep/Move/03.png"));

        stopAllActions();
        runAction(ScaleTo::create(0.3f, 2.0f));
        gc->playSound("sounds/Effects/KidnapGrab.mp3");
        break;

    case 3:
        eff = (Effect*)pNode;
        gc->playMusic("sounds/Backgrounds/Desert_Battle.mp3");
        eff->runAction(
            Sequence::create(
            DelayTime::create(1.0f),
            CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
            nullptr));
        gc->playSound("sounds/Effects/HeadlessHorseman-03a_WET.mp3");
        break;

    case 4:  // 扛女人跑
        eff = (Effect*)pNode;
        eff->playForever(2, 1.5f);
        eff->runAction(
            Sequence::create(
                MoveTo::create(2.0f, Point(-eff->getContentSize().width, eff->getPositionY())),
                CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                nullptr));

        runAction(EaseExponentialOut::create(ScaleTo::create(2.0f, 1.0f)));
        gc->playSound("sounds/Effects/Sound_Sheep.mp3");
        break;

    case 5:  // 牛出现
        // Help
        eff = Effect::create("Effects/Help", 0.1f);
        addChild(eff);
        eff->setScale(2.0f);
        eff->setPosition(Point(eff->getContentSize().width, wsz.height * 0.5 + 100));
        eff->playRelease(0, 2.0f, 6);

        // 生成牛
        eff = Effect::create("Effects/Rush/Move", 0.1f);
        eff->addAnimation("Effects/Rush/Cry", 0.1f);
        eff->addAnimation("Effects/Rush/Rush", 0.1f);
        addChild(eff);
        eff->setPosition(Point(800 + wsz.width + eff->getContentSize().width, wsz.height * 0.5));
        eff->setFlippedX(true);

        eff->playForever(0, 2.0f);
        eff->runAction(
            Sequence::create(
                MoveTo::create(5.0f, Point(wsz.width * 0.8, wsz.height * 0.5)),
                CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                nullptr));

        eff = Effect::create("Effects/Soldier/Move", 0.08f);
        addChild(eff);
        eff->setFlippedX(true);
        eff->setTag(1101);
        eff->setPosition(Point(wsz.width * 0.5, wsz.height + 400));
        eff->playForever();
        eff->runAction(MoveTo::create(10.0f, Point(wsz.width * 0.5 - 100, wsz.height * 0.6)));

        eff = Effect::create("Sprites/Thor/move", 0.08f);
        eff->addAnimation("Sprites/Thor/act3", 0.1f);
        addChild(eff, 5);
        eff->setTag(1102);
        eff->setPosition(Point(wsz.width * 0.5, wsz.height + 400));
        eff->playForever();
        eff->runAction(MoveTo::create(10.0f, Point(wsz.width * 0.5, wsz.height * 0.6)));

        eff = Effect::create("Effects/Soldier/Move", 0.08f);
        addChild(eff);
        eff->setTag(1103);
        eff->setPosition(Point(wsz.width * 0.5, wsz.height + 400));
        eff->playForever();
        eff->runAction(MoveTo::create(10.0f, Point(wsz.width * 0.5 + 100, wsz.height * 0.6)));

        eff = (Effect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->playForever();
        eff->setFlippedX(false);
        eff->runAction(MoveTo::create(5.0f, Point(wsz.width * 0.7, wsz.height * 0.5)));
        break;

    case 6:  // 牛咆哮
        eff = (Effect*)pNode;
        eff->play(1, 1.0f, 1, CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)));

        eff = (Effect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Effects/Sheep/Move/03.png"));
        
        gc->playSound("sounds/Effects/KRF_sfx_minotauro_grito.mp3");
        break;

    case 7:  // 牛冲刺
        eff = (Effect*)pNode;
        eff2 = Effect::create("Effects/Rush/RushEffect", 0.1f);
        eff->addChild(eff2);
        eff2->setPosition(Point(eff->getContentSize().width - 60, eff->getContentSize().height * 0.5 - 55));
        eff2->setFlippedX(true);

        eff->playForever(2, 1.5f);
        eff->runAction(
            Spawn::create(
                Sequence::create(
                    EaseExponentialIn::create(MoveTo::create(2.0f, Point(-eff->getContentSize().width - 2000, wsz.height * 0.5))),
                    CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                    nullptr),
                Sequence::create(
                    DelayTime::create(1.5f),
                    CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onSoldierTurn, this)),
                    nullptr),
                nullptr));

        eff2->playForever();

        eff = (Effect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->playForever();
        eff->setFlippedX(false);
        eff->runAction(MoveTo::create(2.0f, Point(wsz.width * 0.8, wsz.height * 0.6)));
        gc->playSound("sounds/Effects/KRF_sfx_minotauro_carga.mp3");
        break;

    case 8:  // 撞上
        runAction(
            Sequence::create(
                Shake::create(0.2f, 5, 30.0f),
                nullptr));

        eff = (Effect*)getChildByTag(1004);
        eff->stopAllActions();
        eff->setFlippedX(false);
        eff->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Effects/Kidnap/Fly.png"));
        eff->runAction(
            Spawn::create(
                Sequence::create(
                    JumpTo::create(2.0, Point(wsz.width * 0.9, wsz.height * 0.5 + 20), 500.0f, 1),
                    CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                    nullptr),
                Sequence::create(
                    DelayTime::create(1.3f),
                    CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onSoldierTurn, this)),
                    nullptr),
                nullptr));
        eff->runAction(RepeatForever::create(RepeatForever::create(RotateBy::create(1.0f, 1800.0f))));

        getChildByTag(1101)->stopAllActions();
        getChildByTag(1102)->stopAllActions();
        getChildByTag(1103)->stopAllActions();

        eff = (Effect*)getChildByTag(1000);
        eff->stopAllActions();
        eff->playForever();
        eff->setFlippedX(false);
        eff->runAction(MoveTo::create(2.0f, Point(wsz.width * 0.9, wsz.height * 0.5 - 50)));
        gc->playSound("sounds/Effects/inapp_atfreezeend.mp3");
        break;

    case 9:  // 落地
        runAction(
            Sequence::create(
            Shake::create(0.2f, 5, 30.0f),
            nullptr));

        eff = (Effect*)getChildByTag(1004);
        eff->stopAllActions();
        eff->play(3, 1.0f, 1, CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)));
        eff->setRotation(0.0f);
        
        sp = Sprite::createWithSpriteFrameName("Effects/Kidnap/Bang.png");
        addChild(sp);
        sp->setScale(4.0f);
        sp->setPosition(eff->getPosition() + Point(30.0f, -60.0f));
        sp->runAction(FadeOut::create(3.0f));

        eff = Effect::create("Effects/Bang", 0.1f);
        addChild(eff);
        eff->setScale(1.0f);
        eff->setPosition(sp->getPosition() + Point(0.0f, 0.0f));
        eff->playRelease();

        eff = (Effect*)getChildByTag(1000);
        eff->play(1);

        gc->playSound("sounds/Effects/Sound_FireballHit.mp3");
        break;

    case 10:  // 军官向前
        eff = ((Effect*)getChildByTag(1102));
        eff->playForever(0);
        eff->runAction(
            Sequence::create(
                MoveTo::create(3.0f, Point(wsz.width * 0.7, wsz.height * 0.5f)),
                CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                nullptr));
        break;

    case 11:  // 投掷锤子
        eff = (Effect*)pNode;
        eff->setFlippedX(true);
        eff->play(1);
        eff->runAction(
            Sequence::create(
                DelayTime::create(0.5f),
                CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                nullptr));
        
        break;

    case 12:
        eff = (Effect*)getChildByTag(1102);
        sp = Sprite::createWithSpriteFrameName("Sprites/ThorHammer/default.png");
        addChild(sp, 6);
        sp->setPosition(eff->getPosition() + Point(-100.0f, 0.0f));
        sp->setFlippedX(true);

        sp->runAction(
            Sequence::create(
                EaseExponentialIn::create(
                Spawn::create(
                    MoveTo::create(0.3f, Point(wsz.width * 0.5, wsz.height * 0.5)),
                    ScaleTo::create(0.3f, 100.f),
                    nullptr)),
                CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                nullptr));

        gc->playSound("sounds/Effects/HammerThrow.mp3");
        break;

    case 13:
        runAction(
            Sequence::create(
                Shake::create(0.6f, 10, 100.0f),
                CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                nullptr));
        gc->playSound("sounds/Effects/ThunderCap.mp3");
        break;

    case 14:
        pNode = LayerColor::create(Color4B(0, 0, 0, 0));
        getParent()->addChild(pNode, 10);
        sp = Sprite::create("UI/Loading.png");
        pNode->addChild(sp);
        sp->setScale(wsz.width / sp->getContentSize().width * 0.3f);
        sp->setPosition(Point(wsz.width * 0.5, wsz.height * 0.5));
        pNode->runAction(
            Sequence::create(
                FadeIn::create(0.3f),
                CallFuncN::create(CC_CALLBACK_1(LogoScene2Layer::onEffectUpdate, this)),
                nullptr));

        gc->playSound("sounds/Effects/GUITransitionOpen.mp3");
        m_ctrl->setVisible(false);
        break;

    case 15:
        //Director::getInstance()->replaceScene(TransitionFade::create(1.0f, HeroRoomSceneLayer::scene(), Color3B::WHITE));
        Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainMenuSceneLayer::scene(), Color3B::WHITE));
        break;
    }

    ++m_cur;
}

void LogoScene2Layer::onSoldierTurn(Node* pNode)
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    static bool first = true;
    M_DEF_GC(gc);

    if (first)
    {
        first = false;
        ((Sprite*)getChildByTag(1101))->setFlippedX(true);
        ((Sprite*)getChildByTag(1102))->setFlippedX(true);
        ((Sprite*)getChildByTag(1103))->setFlippedX(true);
        gc->playSound("sounds/Effects/hero_dragon_birth.mp3");
        
    }
    else
    {
        ((Sprite*)getChildByTag(1101))->setFlippedX(false);
        ((Sprite*)getChildByTag(1102))->setFlippedX(false);
        ((Sprite*)getChildByTag(1103))->setFlippedX(false);
        gc->playSound("sounds/Effects/hero_dragon_spit.mp3");
    }
    
}

void LogoScene2Layer::onClickSkip(Ref* pObject)
{
    m_cur = 14;
    m_ctrl->setVisible(false);
}

