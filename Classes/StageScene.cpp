#include "CommHeader.h"

#include "StageScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "Stage.h"



// CCStageSceneLayer
CCStageSceneLayer::CCStageSceneLayer()
    : m_panel(NULL)
    , m_name(NULL)
    , m_desc(NULL)
    , m_selIndex(0)
    , m_btnBattle(NULL)
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
        pScene->addChild(layer->m_ctrlLayer);
    }

    // return the scene
    return pScene;
}

// 后续改为用户数据，此处为测试
static char* nor[] = {"UI/Stage/Stage00Nor.png", "UI/Stage/Stage01Nor.png", "UI/Stage/Stage02Nor.png", "UI/Stage/Stage03Nor.png", "UI/Stage/Stage04Nor.png", "UI/Stage/Stage05Nor.png"};
static char* dis[] = {"UI/Stage/Stage00Dis.png", "UI/Stage/Stage01Dis.png", "UI/Stage/Stage02Dis.png", "UI/Stage/Stage03Dis.png", "UI/Stage/Stage04Dis.png", "UI/Stage/Stage05Dis.png"};
static CCPoint ps[] = {ccp(0, 0), ccp(1, 0), ccp(2, 0), ccp(1, 1), ccp(3, 0), ccp(3, 1)};
static int previndex[][10] = {{}, {1}, {2}, {2}, {3, 4}, {5}};
//static CStage::STAGE_STATUS status[] = {CStage::kUnlocked, CStage::kLocked, CStage::kLocked, CStage::kLocked, CStage::kLocked, CStage::kLocked};
static char* name[] = {"the Sunwell", "The Death of the Captain", "Strike Back", "Doubt", "Siege", "The End"};
static char* desc[] = {"The Sunwell is a fount of mystical power located in Quel\'Thalas", "It was created from a vial of water from the Well of Eternity", "It empowered the high elves for approximately nine thousand years, until Arthas used it to resurrect Kel'Thuzad as a lich", "His resurrection tainted the Sunwell in the process, severing the high elves from their source of power", "Kael\'thas attempted to summon Kil\'jaeden into Azeroth through the Sunwell using Anveena\'s powers", "In World of Warcraft, the Sunwell appears in the level 70 raid dungeon, Sunwell Plateau, on the Isle of Quel\'Danas"};
static int grade[] = {0, 0, 0, 0, 0, 0};

// on "init" you need to initialize your instance
bool CCStageSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!CCWinLayer::init())
    {
        return false;
    }

    setBackgroundSprite(CCSprite::create("UI/Stage/MapBackground.png"));

    setPosition(ccp(0.0f, 0.0f));
    
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);
    CCSprite* sp = NULL;
    CCLabelTTF* lbl = NULL;
    gc->loadTexture("Global0");
    gc->loadTexture("Global1");
    gc->preloadSound("sounds/Effect/UIMove.mp3");
    
    // ctrl layer
    m_ctrlLayer = CCTouchMaskLayer::create(ccc4(0, 0, 0, 0));
    m_ctrlLayer->setTouchMode(kCCTouchesOneByOne);
    m_ctrlLayer->setTouchEnabled(false);
    m_ctrlLayer->setTouchPriority(-5);

    // 布置pannel
    m_panel = CCSprite::create("UI/PanelBig.png");
    m_ctrlLayer->addChild(m_panel);
    CCSize psz = m_panel->getContentSize();
    
    CCMenu* panelMn = CCMenu::create();
    m_panel->addChild(panelMn);
    panelMn->setTouchPriority(-10);
    panelMn->setPosition(CCPointZero);
    
    // 标题
    //sp = CCSprite::create("UI/Title.png");
    //lbl = CCLabelTTF::create("Hero Room", "fonts/Yikes!.ttf", 72);
    //lbl->setColor(ccc3(80, 60, 50));
    //sp->addChild(lbl);
    //lbl->setPosition(ccpAdd(sp->getAnchorPointInPoints(), ccp(0.0f, 20)));
    //m_panel->addChild(sp);
    //sp->setPosition(ccp(sz.width * 0.5, sz.height - lbl->getContentSize().height - 100));

    CCMenu* mn = CCMenu::create();
    addChild(mn, 10);
    mn->setTouchPriority(-1);
    mn->setPosition(CCPointZero);

    m_stageMap.setPathName("UI/Stage/Path.png");
    m_stageMap.setPanel(mn, this, menu_selector(CCStageSceneLayer::onClickStage));

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
        stage->setPosition(ccp(ps[i].x * 300 + 200 + rand() % 100 - 50, ps[i].y * 400 + 200 + rand() % 100 - 50));

        CStageMap::VEC_INDEXES vecPrev;
        for (int j = 0; previndex[i][j] != 0; ++j)
        {
            vecPrev.push_back(previndex[i][j] - 1);
        }
        m_stageMap.addStage(stage, vecPrev);
    }
    
    for (int i = 0; i < 6; ++i)
    {
        m_stageMap.getStage(i)->setGrade(grade[i]);
        if (grade[i] > 0)
        {
            m_stageMap.setStageStatus(i, CStage::kConquered);
        }
        else if (i == 0)
        {
            m_stageMap.setStageStatus(0, CStage::kUnlocked);
        }
    }

    m_btnBattle = CCButtonNormal::create(
        CCSprite::create("UI/Stage/BtnBattleNor.png"),
        CCSprite::create("UI/Stage/BtnBattleSel.png"),
        CCSprite::create("UI/Stage/BtnBattleDis.png"),
        NULL,
        NULL,
        0.0f,
        this,
        menu_selector(CCStageSceneLayer::onClickPanelBattle),
        NULL);

    panelMn->addChild(m_btnBattle);
    m_btnBattle->setPosition(ccp(psz.width - m_btnBattle->getContentSize().width * 0.5 - 50, m_btnBattle->getContentSize().height * 0.5 + 50));

    CCButtonNormal* btnClose = CCButtonNormal::create(
        CCSprite::create("UI/BtnCloseNor.png"),
        CCSprite::create("UI/BtnCloseSel.png"),
        CCSprite::create("UI/BtnCloseNor.png"),
        NULL,
        NULL,
        0.0f,
        this,
        menu_selector(CCStageSceneLayer::onClickPanelClose),
        NULL);

    panelMn->addChild(btnClose);
    btnClose->setPosition(ccp(psz.width - btnClose->getContentSize().width * 0.5 - 50, psz.height - btnClose->getContentSize().height * 0.5 - 50));

    // 名字、简介
    m_name = CCLabelTTF::create(" ", FONT_COMIC_BOOK, 100, CCSize(psz.width * 0.6, 100 / 0.75f), kCCTextAlignmentLeft);
    m_name->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    m_name->setAnchorPoint(ccp(0.0f, 0.5f));
    m_panel->addChild(m_name);
    m_name->setPosition(ccp(100, psz.height - m_name->getContentSize().height * 0.5 - 100));

    m_desc = CCLabelTTF::create(" ", FONT_COMIC_BOOK, 60, CCSize(psz.width * 0.6, 60 / 0.75f * 5), kCCTextAlignmentLeft);
    m_desc->setVerticalAlignment(kCCVerticalTextAlignmentTop);
    m_desc->setAnchorPoint(ccp(0.0f, 0.5f));
    m_panel->addChild(m_desc);
    m_desc->setPosition(ccp(100, m_name->getPosition().y - m_name->getContentSize().height * 0.5 - m_desc->getContentSize().height * 0.5));

    for (int i = 0; i < 3; ++i)
    {
        m_stars[i] = CCSprite::createWithSpriteFrameName("UI/Stage/Unstar.png");
        m_panel->addChild(m_stars[i]);
        m_stars[i]->setScale(2.0f);
        m_stars[i]->setPosition(ccp(psz.width - m_stars[i]->getContentSize().width * m_stars[i]->getScale() * (3 - i) - 150, m_name->getPosition().y));
    }

    // panel缩放
    m_panel->setScale(min(wsz.width / psz.width, wsz.height / psz.height));
    psz = psz * m_panel->getScale();
    m_panel->setPosition(ccp(wsz.width * 0.5, wsz.height + psz.height * 0.5));

    return true;
}

void CCStageSceneLayer::onClickStage( CCObject* pObj )
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);

    CCMenuItemImage* mi = DCAST(pObj, CCMenuItemImage*);
    m_selIndex = mi->getTag();
    CStage* stage = m_stageMap.getStage(m_selIndex);

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
    
    sp->setPosition(mi->getPosition());

    m_name->setString(name[m_selIndex]);
    m_desc->setString(desc[m_selIndex]);
    for (int i = 0; i < 3; ++i)
    {
        DCAST(m_stars[i], CCSprite*)->setDisplayFrame(gc->getfc()->spriteFrameByName(i < stage->getGrade() ? "UI/Stage/Star.png" : "UI/Stage/Unstar.png"));
    }

    // 缩放panel
    gc->playSound("sounds/Effect/GUITransitionOpen.mp3");
    m_panel->stopAllActions();
    m_panel->runAction(CCEaseExponentialOut::create(CCMoveTo::create(0.5f, ccp(wsz.width * 0.5, wsz.height * 0.5))));

    //CCLayerColor* mask = CCLayerColor::create(ccc4(0, 0, 0, 50));
    //m_ctrlLayer->addChild(mask, -10, -10);
    //mask->setTouchPriority(-5);
    m_ctrlLayer->setOpacity(80);
    m_ctrlLayer->setTouchEnabled(true);
}

void CCStageSceneLayer::onClickPanelBattle( CCObject* pObj )
{
    m_stageMap.setStageStatus(m_selIndex, CStage::kConquered);
    m_stageMap.getStage(m_selIndex)->setGrade(rand() % 3 + 1);

    // for test
    onClickPanelClose(NULL);
}

void CCStageSceneLayer::onClickPanelClose( CCObject* pObj )
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);
    gc->playSound("sounds/Effect/GUITransitionOpen.mp3");
    m_panel->stopAllActions();
    m_panel->runAction(CCEaseExponentialOut::create(CCMoveTo::create(0.5f, ccp(wsz.width * 0.5, wsz.height + m_panel->getContentSize().height * m_panel->getScaleY() * 0.5))));

    //m_ctrlLayer->removeChildByTag(-10);
    m_ctrlLayer->setOpacity(0);
    m_ctrlLayer->setTouchEnabled(false);
}

