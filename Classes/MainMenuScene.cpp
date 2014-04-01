#include "CommHeader.h"

#include "MainMenuScene.h"
#include "GameControl.h"
#include "platform/CCFileUtils.h"

CCMainMenuScene::CCMainMenuScene()
{
}
CCMainMenuScene::~CCMainMenuScene()
{
}
bool CCMainMenuScene::init()
{
    return CCScene::init();
}


//Layer
CCMainMenuSceneLayer::CCMainMenuSceneLayer()
{

}

CCMainMenuSceneLayer::~CCMainMenuSceneLayer()
{

}

CCScene* CCMainMenuSceneLayer::scene()
{
    CCMainMenuScene *pScene = CCMainMenuScene::create();
    CCMainMenuSceneLayer *pLayer = CCMainMenuSceneLayer::create();
    if (pLayer != NULL)
    {
        pScene->addChild(pLayer);
    }
    return pScene;
}

bool CCMainMenuSceneLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    char sz[1024];
    sprintf(sz, "backgrounds/BackgroundHD%02d.png", rand() % 2);
    CCSprite* pSprite = CCSprite::create(sz);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize orgSize = pSprite->getContentSize();
    pSprite->setScaleX(winSize.width/orgSize.width);
    pSprite->setScaleY(winSize.height/orgSize.height);
    addChild(pSprite);
    pSprite->setAnchorPoint(ccp(0,0));
    pSprite->setPosition(ccp(0,0));
    pSprite->setZOrder(1);

    CCMenu* pMenu = CCMenu::create();
    m_pItemStart = CCMenuItemImage::create("mainmenu/menu_startchain_0001.png"
        , "mainmenu/menu_startchain_0002.png"
        , this
        , menu_selector(CCMainMenuSceneLayer::CCMenuItemStartCallback));
    addChild(pMenu);
    pMenu->setPosition(0,0);
    pMenu->setZOrder(2);
    m_pItemStart->setPosition(winSize.width/2,winSize.height * 3/4);
    pMenu->addChild(m_pItemStart);
    

    return true;
}

void CCMainMenuSceneLayer::onEnter()
{
    CCLayer::onEnter();
    CCRunStartAction();
}

void CCMainMenuSceneLayer::CCRunStartAction()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_pItemStart->runAction(CCMoveTo::create(1, ccp(winSize.width/2, winSize.height/2)));
}

void CCMainMenuSceneLayer::CCMenuItemStartCallback(CCObject* sender)
{
    //CCActionInterval* effect = CCRotateTo::create(3, 30, 40);
    //runAction(effect);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* pSprite = (CCSprite*)sender;
    pSprite->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(1, ccp(winSize.width/2 , winSize.height *3/4))
        , CCCallFunc::create(this, callfunc_selector(CCMainMenuSceneLayer::CCCallSelectArchiveLayer))));
    
}

void CCMainMenuSceneLayer::CCCallSelectArchiveLayer()
{
    CCSelectArchiveLayer *pSelectLayer = CCSelectArchiveLayer::create();
    addChild(pSelectLayer);
    pSelectLayer->setPosition(0,0);
    pSelectLayer->setZOrder(3);
}


CCSelectArchiveLayer::CCSelectArchiveLayer()
{

}
CCSelectArchiveLayer::~CCSelectArchiveLayer()
{

}
bool CCSelectArchiveLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    CCSprite* pSprite = CCSprite::create("mainmenu/mainmenu_saveslot_bg.png");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //CCSize orgSize = pSprite->getContentSize();
    addChild(pSprite);
    pSprite->setPosition(ccp(winSize.width/2,winSize.height/2));

    CCMenu* pMenu = CCMenu::create();
    m_pItem1 = CCMenuItemImage::create("mainmenu/mainmenu_saveslot_0001.png"
        , "mainmenu/mainmenu_saveslot_0002.png"
        , this
        , menu_selector(CCSelectArchiveLayer::CCMenuItemCallback1));
    m_pItem2 = CCMenuItemImage::create("mainmenu/mainmenu_saveslot_0001.png"
        , "mainmenu/mainmenu_saveslot_0002.png"
        , this, menu_selector(CCSelectArchiveLayer::CCMenuItemCallback2));
    m_pItem3 = CCMenuItemImage::create("mainmenu/mainmenu_saveslot_0001.png"
        , "mainmenu/mainmenu_saveslot_0002.png"
        , this
        , menu_selector(CCSelectArchiveLayer::CCMenuItemCallback3));

    pMenu->addChild(m_pItem1);
    m_pItem1->setPosition(winSize.width/2, winSize.height/2 + 200);
    m_pLabel1 = CCLabelTTF::create("Hello World", "Arial", 48);
    m_pItem1->addChild(m_pLabel1);
    m_pLabel1->setPosition(ccp(m_pItem1->getContentSize().width/2, m_pItem1->getContentSize().height/2));
    pMenu->addChild(m_pItem2);
    m_pItem2->setPosition(winSize.width/2, winSize.height/2);
    m_pLabel2 = CCLabelTTF::create("Goodbye World", "Arial", 48);
    m_pItem2->addChild(m_pLabel2);
    m_pLabel2->setPosition(ccp(m_pItem2->getContentSize().width/2, m_pItem2->getContentSize().height/2));
    pMenu->addChild(m_pItem3);
    m_pItem3->setPosition(winSize.width/2, winSize.height/2 - 200);
    m_pLabel3 = CCLabelTTF::create("Hello Goodbye", "Arial", 48);
    m_pItem3->addChild(m_pLabel3);
    m_pLabel3->setPosition(ccp(m_pItem3->getContentSize().width/2, m_pItem3->getContentSize().height/2));

    addChild(pMenu);
    pMenu->setPosition(0, 0);

    CCMenuItemImage* pItemClose = CCMenuItemImage::create("mainmenu/mainmenu_saveslot_close_0001.png"
        , "mainmenu/mainmenu_saveslot_close_0002.png"
        , this
        , menu_selector(CCSelectArchiveLayer::CCMenuItemCloseCallback));
    pItemClose->setPosition(winSize.width/2, winSize.height/2 - pSprite->getContentSize().height/2 - pItemClose->getContentSize().height/2);
    pMenu->addChild(pItemClose);
    //CCMenuItemImage* 

    return true;
}

void CCSelectArchiveLayer::CCMenuItemCallback1(CCObject* sender)
{
    CCLog("~~~~~","~~~~~~~~");
}

void CCSelectArchiveLayer::CCMenuItemCallback2(CCObject* sender)
{

}
void CCSelectArchiveLayer::CCMenuItemCallback3(CCObject* sender)
{

}
void CCSelectArchiveLayer::CCMenuItemCloseCallback(CCObject* sender)
{
    CCNode* pNode = getParent();
    CCMainMenuSceneLayer* pMainLayer = (CCMainMenuSceneLayer*)pNode;
    pMainLayer->CCRunStartAction();
    removeFromParent();
}
void CCSelectArchiveLayer::onEnter()
{
    CCLayer::onEnter();
    if(CCFileUtils::sharedFileUtils()->isFileExist("config.txt"))
    {
        CCLog("----","++++");
    }
}