#include "CommHeader.h"

#include "StageScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "Stage.h"



// CCStageSceneLayer
CCStageSceneLayer::CCStageSceneLayer()
    : m_pName(NULL)
    , m_pDesc(NULL)
    , m_iSelIndex(0)
    , m_pBattle(NULL)
{
}

CCStageSceneLayer::~CCStageSceneLayer()
{
}

CCScene* CCStageSceneLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene* pScene = CCScene::create();

    CCStageSceneLayer* layer = CCStageSceneLayer::create();

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
bool CCStageSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!CCLayerColor::initWithColor(ccc4(173, 138, 90, 255)))
    {
        return false;
    }
    
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);
    CCSprite* sp = NULL;
    gc->loadTexture("Global0");
    gc->loadTexture("Global1");
    
    CCMenu* mn = CCMenu::create();
    addChild(mn, 10);
    mn->setPosition(CCPointZero);

    m_oStageMap.setPathName("UI/Stage/Path.png");
    m_oStageMap.setPanel(mn, this, menu_selector(CCStageSceneLayer::onClickStage));

    static const char* nor[] = {"UI/Stage/Stage00Nor.png", "UI/Stage/Stage01Nor.png", "UI/Stage/Stage02Nor.png", "UI/Stage/Stage03Nor.png", "UI/Stage/Stage04Nor.png", "UI/Stage/Stage05Nor.png"};
    //static const char* sel[] = {"UI/Stage/Stage00Sel.png", "UI/Stage/Stage01Sel.png", "UI/Stage/Stage02Sel.png", "UI/Stage/Stage03Sel.png", "UI/Stage/Stage04Sel.png", "UI/Stage/Stage05Sel.png"};
    static const char* dis[] = {"UI/Stage/Stage00Dis.png", "UI/Stage/Stage01Dis.png", "UI/Stage/Stage02Dis.png", "UI/Stage/Stage03Dis.png", "UI/Stage/Stage04Dis.png", "UI/Stage/Stage05Dis.png"};
    static const CCPoint ps[] = {ccp(0, 0), ccp(1, 0), ccp(2, 0), ccp(1, 1), ccp(3, 0), ccp(3, 1)};
    static const int prev[][10] = {{}, {1}, {2}, {2}, {3}, {4, 5}};
    static const CStage::STAGE_STATUS status[] = {CStage::kUnlocked, CStage::kLocked, CStage::kLocked, CStage::kLocked, CStage::kLocked, CStage::kLocked};
    static const char* name[] = {"the Sunwell", "The Death of the Captain", "Strike Back", "Doubt", "Siege", "The End"};
    static const char* desc[] = {"The Sunwell is a fount of mystical power located in Quel\'Thalas", "It was created from a vial of water from the Well of Eternity", "It empowered the high elves for approximately nine thousand years, until Arthas used it to resurrect Kel'Thuzad as a lich", "His resurrection tainted the Sunwell in the process, severing the high elves from their source of power", "Kael\'thas attempted to summon Kil\'jaeden into Azeroth through the Sunwell using Anveena\'s powers", "In World of Warcraft, the Sunwell appears in the level 70 raid dungeon, Sunwell Plateau, on the Isle of Quel\'Danas"};

    for (int i = 0; i < 6; ++i)
    {
        CStage* stage = new CStage;
        stage->setNormalName(nor[i]);
        stage->setSelectedName("UI/Stage/StageSel.png");
        stage->setDisabledName(dis[i]);
        stage->setStarName("UI/Stage/Star.png");
        stage->setUnstarName("UI/Stage/Unstar.png");
        stage->setName(name[i]);
        stage->setDescribe(desc[i]);
        stage->setPosition(ccp(ps[i].x * 300 + 300 + rand() % 150 - 75, ps[i].y * 400 + 300 + rand() % 150 - 75));

        CStageMap::VEC_INDEXES vecPrev;
        for (int j = 0; prev[i][j] != 0; ++j)
        {
            vecPrev.push_back(prev[i][j] - 1);
        }
        m_oStageMap.addStage(stage, vecPrev);
    }
    
    for (int i = 0; i < 6; ++i)
    {
        m_oStageMap.changeStageStatus(i, status[i]);
    }

    m_pBattle = CCButtonNormal::create(
        CCSprite::create("UI/Stage/BtnBattleNor.png"),
        CCSprite::create("UI/Stage/BtnBattleSel.png"),
        CCSprite::create("UI/Stage/BtnBattleDis.png"),
        NULL,
        NULL,
        0.0f,
        this,
        menu_selector(CCStageSceneLayer::onClickBattle),
        NULL);

    mn->addChild(m_pBattle);
    m_pBattle->setPosition(ccp(wsz.width - m_pBattle->getContentSize().width * 0.5 - 50, wsz.height * 0.5));

    return true;
}

void CCStageSceneLayer::onClickStage( CCObject* pObj )
{
    CCMenuItemImage* mi = DCAST(pObj, CCMenuItemImage*);
    m_iSelIndex = mi->getTag();
    static CCSprite* sp = NULL;
    if (sp == NULL)
    {
        sp = CCSprite::create("UI/Stage/StageCur.png");
        addChild(sp);
        sp->setPosition(mi->getPosition());
        sp->setVisible(false);
    }
    if (!sp->isVisible())
    {
        sp->setVisible(true);
    }
    //sp->stopAllActions();
    sp->setPosition(mi->getPosition());
    //sp->setScale(1.5f);
    //sp->runAction(CCEaseExponentialOut::create(CCScaleTo::create(0.2f, 1.0f)));
}

void CCStageSceneLayer::onClickBattle( CCObject* pObj )
{
    m_oStageMap.changeStageStatus(m_iSelIndex, CStage::kConquered);
}


