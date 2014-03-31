#include "CommHeader.h"

#include "HeroRoomScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"



// CCHeroRoomSceneLayer
CCHeroRoomSceneLayer::CCHeroRoomSceneLayer()
{
}

CCHeroRoomSceneLayer::~CCHeroRoomSceneLayer()
{
}

CCScene* CCHeroRoomSceneLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene* pScene = CCScene::create();

    CCHeroRoomSceneLayer* layer = CCHeroRoomSceneLayer::create();

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
bool CCHeroRoomSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    //if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 255)))
    {
        return false;
    }

    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);
    CCSprite* sp = NULL;

    gc->loadTexture("Global0");
    gc->loadTexture("Global1");
    gc->preloadSound("sounds/Effect/UIMove.mp3");
    gc->playMusic("sounds/Background/MainBackground.mp3");

    m_panel = CCSprite::create("UI/PanelBig.png");
    addChild(m_panel);
    CCSize sz = m_panel->getContentSize();
    m_panel->setPosition(ccp(wsz.width * 0.5, wsz.height + sz.height * m_panel->getScaleY() * 0.5));
    
    // Í·Ïñ¿ò
    CCButtonPanel* bp = CCButtonPanel::create(1, 3, 156, 164, 18, 11, CCSprite::create("UI/HeroesBorder.png"), 0.0f, 38.0f);
    m_panel->addChild(bp);
    bp->setPosition(ccp(sz.width * 0.05 + bp->getContentSize().width * 0.5, sz.height * 0.5));
    bp->getBackground()->setZOrder(10);

    CCButtonNormal* btn = CCButtonNormal::createWithFrameName("UI/Portrait/WarriorSmall.png", "UI/Portrait/WarriorSmall.png", NULL, NULL, NULL, 0.0f, this, callfuncN_selector(CCHeroRoomSceneLayer::onClickHeroPortrait), NULL);
    bp->addButton(btn, 0, 0);

    m_sel = CCSprite::createWithSpriteFrameName("UI/Portrait/SelectSmall.png");
    bp->addChild(m_sel);
    m_sel->setPosition(btn->getPosition());

    btn = CCButtonNormal::createWithFrameName("UI/Portrait/ArcherSmall.png", "UI/Portrait/ArcherSmall.png", NULL, NULL, NULL, 0.0f, this, callfuncN_selector(CCHeroRoomSceneLayer::onClickHeroPortrait), NULL);
    bp->addButton(btn, 1, 0);

    btn = CCButtonNormal::createWithFrameName("UI/Portrait/WizardSmall.png", "UI/Portrait/WizardSmall.png", NULL, NULL, NULL, 0.0f, this, callfuncN_selector(CCHeroRoomSceneLayer::onClickHeroPortrait), NULL);
    bp->addButton(btn, 2, 0);

    // ÐÅÏ¢¿ò
    sp = CCSprite::create("UI/HeroInfo.png");
    m_panel->addChild(sp, 10);
    sp->setPosition(ccp(sz.width * 0.65, sz.height * 0.5));

    m_selBig = CCSprite::createWithSpriteFrameName("UI/Portrait/WarriorBig.png");
    m_panel->addChild(m_selBig);
    m_selBig->setPosition(ccpAdd(sp->getPosition(), ccp(-275, -30)));

    CCMenu* mn = CCMenu::create();
    m_panel->addChild(mn);
    mn->setPosition(CCPointZero);

    btn = CCButtonNormal::createWithFile("UI/Button/DoneNormal.png", "UI/Button/DoneOn.png", NULL, NULL, NULL, 0.0f, this, callfuncN_selector(CCHeroRoomSceneLayer::onClickButtonDone), NULL);
    mn->addChild(btn);
    btn->setPosition(ccp(sz.width - btn->getContentSize().width * 0.5 - 40, btn->getContentSize().height * 0.5 + 40));

    m_panel->runAction(CCMoveTo::create(0.5f, ccp(wsz.width * 0.5, wsz.height * 0.5)));

    return true;
}

void CCHeroRoomSceneLayer::onClickHeroPortrait( CCNode* pNode )
{
    M_DEF_GC(gc);
    static const char* bigPortraits[] = {"UI/Portrait/WarriorBig.png", "UI/Portrait/ArcherBig.png", "UI/Portrait/WizardBig.png"};
    gc->playSound("sounds/Effect/UIMove.mp3");
    m_sel->setPosition(pNode->getPosition());
    CCButtonNormal* btn = DCAST(pNode, CCButtonNormal*);
    int index = btn->getButtonIndex();
    m_selBig->setDisplayFrame(gc->getfc()->spriteFrameByName(bigPortraits[index]));

    //btn->runAction(CCBlink::create(0.5f, 3));
    //m_selBig->runAction(CCBlink::create(0.5f, 3));
}

void CCHeroRoomSceneLayer::onClickButtonDone( CCNode* pNode )
{
    M_DEF_GC(gc);
    gc->playSound("sounds/Effect/UIMove.mp3");
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    m_panel->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.5f, ccp(wsz.width * 0.5, wsz.height + m_panel->getContentSize().height * m_panel->getScaleY() * 0.5)), CCCallFuncN::create(this, callfuncN_selector(CCHeroRoomSceneLayer::onHideDone))));
}

void CCHeroRoomSceneLayer::onHideDone( CCNode* pNode )
{
}



