#include "CommHeader.h"

#include "MainMenuScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "HeroRoomScene.h"
#include "StageScene.h"
#include "GameData.h"
#include "UserData.h"


// MainMenuScene
MainMenuScene::MainMenuScene()
{
}
MainMenuScene::~MainMenuScene()
{
}
bool MainMenuScene::init()
{
    return Scene::init();
}


//Layer
MainMenuSceneLayer::MainMenuSceneLayer()
{

}

MainMenuSceneLayer::~MainMenuSceneLayer()
{

}

Scene* MainMenuSceneLayer::scene()
{
    MainMenuScene *pScene = MainMenuScene::create();
    MainMenuSceneLayer *pLayer = MainMenuSceneLayer::create();
    if (pLayer != nullptr)
    {
        pScene->addChild(pLayer);
    }
    return pScene;
}

bool MainMenuSceneLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Sprite* pSprite = Sprite::create("backgrounds/MainMenuBackground.png");
    static Size wsz = Director::getInstance()->getWinSize();
    Size orgSize = pSprite->getContentSize();
    pSprite->setScale(max(wsz.width/orgSize.width, wsz.height/orgSize.height));
    addChild(pSprite);
    pSprite->setAnchorPoint(Point::ZERO);
    pSprite->setPosition(Point::ZERO);

    
    m_pItemStart = MenuItemImage::create(
        "UI/mainmenu/menu_startchain_0001.png",
        "UI/mainmenu/menu_startchain_0002.png",
        CC_CALLBACK_1(MainMenuSceneLayer::MenuItemStartCallback, this));

    m_pItemStart->setPosition(wsz.width/2,wsz.height * 3/4);
    
    MenuItemImage* pItemLogo = MenuItemImage::create("UI/mainmenu/logo.png", "", CC_CALLBACK_1(MainMenuSceneLayer::MenuItemLogoCallback, this));
    pItemLogo->setPosition(wsz.width/2, wsz.height * 3/4);
    
    
    Menu* pMenu = Menu::create();
    addChild(pMenu, 1);
    pMenu->setPosition(Point::ZERO);
    pMenu->addChild(m_pItemStart);

    Menu* pMenu2 = Menu::create();
    addChild(pMenu2, 2);
    pMenu2->setPosition(Point::ZERO);
    pMenu2->addChild(pItemLogo);

    return true;
}

void MainMenuSceneLayer::MenuItemLogoCallback(Ref* sender)
{
    CCLOG("Touch Logo");
}
void MainMenuSceneLayer::onEnter()
{
    Layer::onEnter();
    CCRunStartAction();
}

void MainMenuSceneLayer::CCRunStartAction()
{
    Size wsz = Director::getInstance()->getWinSize();
    m_pItemStart->runAction(EaseExponentialOut::create(MoveTo::create(0.5f, Point(wsz.width/2, wsz.height/2))));
    m_pItemStart->setEnabled(true);
}

void MainMenuSceneLayer::MenuItemStartCallback(Ref* sender)
{
    Size wsz = Director::getInstance()->getWinSize();
    Sprite* pSprite = (Sprite*)sender;
    pSprite->runAction(
        Sequence::createWithTwoActions(EaseExponentialOut::create(MoveTo::create(0.5f, Point(wsz.width / 2, wsz.height * 3 / 4))),
        CallFunc::create(CC_CALLBACK_0(MainMenuSceneLayer::CCCallSelectArchiveLayer, this))));
}

void MainMenuSceneLayer::CCCallSelectArchiveLayer()
{
    SelectArchiveLayer *pSelectLayer = SelectArchiveLayer::create();
    addChild(pSelectLayer, 3);
    pSelectLayer->setPosition(Point::ZERO);
    m_pItemStart->setEnabled(false);
}


SelectArchiveLayer::SelectArchiveLayer()
{
}

SelectArchiveLayer::~SelectArchiveLayer()
{
}

bool SelectArchiveLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    Sprite* pSprite = Sprite::create("UI/mainmenu/mainmenu_saveslot_bg.png");
    Size wsz = Director::getInstance()->getWinSize();
    //Size orgSize = pSprite->getContentSize();
    addChild(pSprite);
    pSprite->setPosition(Point(wsz.width/2,wsz.height/2));

    Menu* pMenu = Menu::create();
    m_pItem1 = MenuItemImage::create(
        "UI/mainmenu/mainmenu_saveslot_0001.png",
        "UI/mainmenu/mainmenu_saveslot_0002.png",
        CC_CALLBACK_1(SelectArchiveLayer::MenuItemCallback1, this));
    m_pItem2 = MenuItemImage::create(
        "UI/mainmenu/mainmenu_saveslot_0001.png",
        "UI/mainmenu/mainmenu_saveslot_0002.png",
        CC_CALLBACK_1(SelectArchiveLayer::MenuItemCallback2, this));
    m_pItem3 = MenuItemImage::create(
        "UI/mainmenu/mainmenu_saveslot_0001.png",
        "UI/mainmenu/mainmenu_saveslot_0002.png",
        CC_CALLBACK_1(SelectArchiveLayer::MenuItemCallback3, this));

    pMenu->addChild(m_pItem1);
    m_pItem1->setPosition(wsz.width/2, wsz.height/2 + 200);
    m_pLabel1 = Label::createWithTTF("Hello World", FONT_ARIAL, 48);
    m_pItem1->addChild(m_pLabel1);
    m_pLabel1->setPosition(Point(m_pItem1->getContentSize().width/2, m_pItem1->getContentSize().height/2));
    pMenu->addChild(m_pItem2);
    m_pItem2->setPosition(wsz.width/2, wsz.height/2);
    m_pLabel2 = Label::createWithTTF("Goodbye World", FONT_ARIAL, 48);
    m_pItem2->addChild(m_pLabel2);
    m_pLabel2->setPosition(Point(m_pItem2->getContentSize().width/2, m_pItem2->getContentSize().height/2));
    pMenu->addChild(m_pItem3);
    m_pItem3->setPosition(wsz.width/2, wsz.height/2 - 200);
    m_pLabel3 = Label::createWithTTF("Hello Goodbye", FONT_ARIAL, 48);
    m_pItem3->addChild(m_pLabel3);
    m_pLabel3->setPosition(Point(m_pItem3->getContentSize().width/2, m_pItem3->getContentSize().height/2));

    addChild(pMenu);
    pMenu->setPosition(Point::ZERO);

    MenuItemImage* pItemClose = MenuItemImage::create(
        "UI/mainmenu/mainmenu_saveslot_close_0001.png",
        "UI/mainmenu/mainmenu_saveslot_close_0002.png",
        CC_CALLBACK_1(SelectArchiveLayer::MenuItemCloseCallback, this));
    pItemClose->setPosition(wsz.width/2, wsz.height/2 - pSprite->getContentSize().height/2 - pItemClose->getContentSize().height/2);
    pMenu->addChild(pItemClose);
    //MenuItemImage* 

    return true;
}

void SelectArchiveLayer::MenuItemCallback1(Ref* sender)
{
    CUserData* udt = CUserData::instance();
    CUserData::HERO_INFO* hi = udt->getHeroSelected();

    M_DEF_GC(gc);
    //gc->replaceSceneWithLoading(hi == nullptr ? &HeroRoomSceneLayer::scene : &StageSceneLayer::scene);
    gc->replaceSceneWithLoading(&StageSceneLayer::scene);
}

void SelectArchiveLayer::MenuItemCallback2(Ref* sender)
{

}

void SelectArchiveLayer::MenuItemCallback3(Ref* sender)
{

}

void SelectArchiveLayer::MenuItemCloseCallback(Ref* sender)
{
    Node* pNode = getParent();
    MainMenuSceneLayer* pMainLayer = (MainMenuSceneLayer*)pNode;
    pMainLayer->CCRunStartAction();
    removeFromParent();
}

void SelectArchiveLayer::onEnter()
{
    Layer::onEnter();
    return;

    string path = FileUtils::getInstance()->getWritablePath() + "save00";
    CCLOG("path = %s", path.c_str());
    if(FileUtils::getInstance()->isFileExist(path))
    {
        Data data = FileUtils::getInstance()->getDataFromFile(path);
        CCLOG("%ld", data.getSize());
        m_pLabel1->setString((const char*)data.getBytes());
    }
    else
    {
        CCLOG("File not Exist");
        FILE* file = fopen(path.c_str(), "wb"); 

        if (file)
        { 
            fwrite("input sth", sizeof(unsigned char), 10, file);
            fclose(file);  
        }
    }
}
/*

SaveFile::SaveFile()
{

}

SaveFile::~SaveFile()
{

}

bool SaveFile::init(const char* pFileName, const char* pMode)
{
    string path = FileUtils::getInstance()->getWritablePath() + pFileName;
    unsigned long len = 0;
    unsigned char* data = FileUtils::getInstance()->getFileData(path.c_str(), "rb", &len);

}
*/