#include "CommHeader.h"

#include "StageScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "Stage.h"
#include "UserData.h"
#include "GameData.h"
#include "BattleScene.h"


// CCStageSceneLayer
CCStageSceneLayer::CCStageSceneLayer()
    : m_panel(NULL)
    , m_name(NULL)
    , m_desc(NULL)
    , m_selIndex(0)
    , m_btnBattle(NULL)
    , m_stageCur(NULL)
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

// on "init" you need to initialize your instance
bool CCStageSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!CCWinLayer::init())
    {
        return false;
    }

    // ctrl layer
    m_ctrlLayer = CCTouchMaskLayer::create(ccc4(0, 0, 0, 0), 80, -5);

    M_DEF_GC(gc);
    gc->playMusic("sounds/Background/Map_Theme.mp3");
    addChild(gc->defaultLoadingLayer(), 100, 100);

    vector<string> ts;
    ts.push_back("Global0");
    ts.push_back("Global1");

    vector<string> ot;
    ot.push_back("UI/Stage/MapBackground.png");
    ot.push_back("UI/PanelBig.png");
    gc->loadTexturesAsync(ts, ot, this, callfuncO_selector(CCStageSceneLayer::onLoadingProgress), callfuncO_selector(CCStageSceneLayer::onLoadingDone));

    return true;
}

void CCStageSceneLayer::onLoadingProgress( CCObject* pObj )
{

}

void CCStageSceneLayer::onLoadingDone( CCObject* pObj )
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);
    CCSprite* sp = NULL;
    CCLabelTTF* lbl = NULL;

    removeChildByTag(100);

    setBackgroundSprite(CCSprite::create("UI/Stage/MapBackground.png"));
    setPosition(ccp(0.0f, 0.0f));

    gc->preloadSound("sounds/Effect/UIMove.mp3");

    // 布置pannel
    m_panel = CCPopPanel::create(CCSprite::create("UI/PanelBig.png"));
    m_ctrlLayer->addChild(m_panel);
    CCSize psz = m_panel->getContentSize();

    m_panel->getMenu()->setTouchPriority(-10);
    m_panel->getMenu()->setPosition(CCPointZero);

    sp = CCSprite::create("UI/Chain.png");
    m_panel->addChild(sp, -1);
    sp->setPosition(ccp(psz.width * 0.2, psz.height + sp->getContentSize().height * 0.2));

    sp = CCSprite::create("UI/Chain.png");
    m_panel->addChild(sp, -1);
    sp->setPosition(ccp(psz.width * 0.8, psz.height + sp->getContentSize().height * 0.2));

    sp = CCSprite::create("UI/Title.png");
    m_panel->addChild(sp, 1, 2);
    sp->setPosition(ccp(psz.width * 0.5, psz.height));

    lbl = CCLabelTTF::create("STAGES", FONT_YIKES, 72);
    lbl->setColor(ccc3(80, 60, 50));
    sp->addChild(lbl, 1, 2);
    lbl->setPosition(ccpAdd(sp->getAnchorPointInPoints(), ccp(0.0f, 20)));

    CCMenu* mn = CCMenu::create();
    addChild(mn, 10);
    mn->setTouchPriority(-1);
    mn->setPosition(CCPointZero);

    m_stageMap.setPathName("UI/Stage/Path.png");
    m_stageMap.setPanel(mn, this, menu_selector(CCStageSceneLayer::onClickStage));

    // 关卡数据，用户数据
    CGameData::VEC_STAGES& si = CGameData::instance()->m_stages;
    vector<int>& sg = CUserData::instance()->m_stageGrades;

    for (int i = 0; i < (int)si.size(); ++i)
    {
        CStage* stage = new CStage;
        stage->setNormalName(si[i].btnNorName.c_str());
        stage->setSelectedName("UI/Stage/StageSel.png");
        stage->setDisabledName(si[i].btnDisName.c_str());
        stage->setStarName("UI/Stage/Star.png");
        stage->setUnstarName("UI/Stage/Unstar.png");
        stage->setName(si[i].name.c_str());
        stage->setDescribe(si[i].desc.c_str());
        stage->setPosition(si[i].pos);

        m_stageMap.addStage(stage, si[i].prevIndex);
    }

    for (int i = 0; i < (int)sg.size(); ++i)
    {
        m_stageMap.getStage(i)->setGrade(sg[i]);
        if (sg[i] > 0)
        {
            m_stageMap.setStageStatus(i, CStage::kConquered);
        }
    }

    if (m_stageMap.getStage(0)->getStatus() == CStage::kLocked)
    {
        m_stageMap.setStageStatus(0, CStage::kUnlocked);
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

    m_panel->addButton(m_btnBattle);
    m_btnBattle->setPosition(ccp(psz.width - m_btnBattle->getContentSize().width * 0.5 - 50, m_btnBattle->getContentSize().height * 0.5 + 50));

    CCButtonNormal* btnClose = CCButtonNormal::create(
        CCSprite::create("UI/Button/BtnCloseNor.png"),
        CCSprite::create("UI/Button/BtnCloseSel.png"),
        CCSprite::create("UI/Button/BtnCloseNor.png"),
        NULL,
        NULL,
        0.0f,
        this,
        menu_selector(CCStageSceneLayer::onClickPanelClose),
        NULL);

    m_panel->addButton(btnClose);
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
    m_panel->setScale(min(wsz.width * 0.8 / psz.width, wsz.height * 0.8 / psz.height));
    psz = psz * m_panel->getScale();
    m_panel->setPosition(ccp(wsz.width * 0.5, wsz.height + psz.height * 0.5));
}

void CCStageSceneLayer::onClickStage( CCObject* pObj )
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);

    CGameData::VEC_STAGES& si = CGameData::instance()->m_stages;

    CCMenuItemImage* mi = DCAST(pObj, CCMenuItemImage*);
    m_selIndex = mi->getTag();
    CStage* stage = m_stageMap.getStage(m_selIndex);

    if (m_stageCur == NULL)
    {
        m_stageCur = CCSprite::create("UI/Stage/StageCur.png");
        addChild(m_stageCur);
        m_stageCur->setPosition(mi->getPosition());
        m_stageCur->setVisible(false);
    }
    if (!m_stageCur->isVisible())
    {
        m_stageCur->setVisible(true);
    }

    m_stageCur->setPosition(mi->getPosition());

    m_name->setString(si[m_selIndex].name.c_str());
    m_desc->setString(si[m_selIndex].desc.c_str());
    for (int i = 0; i < 3; ++i)
    {
        DCAST(m_stars[i], CCSprite*)->setDisplayFrame(gc->getfc()->spriteFrameByName(i < stage->getGrade() ? "UI/Stage/Star.png" : "UI/Stage/Unstar.png"));
    }

    // 弹出panel
    gc->playSound("sounds/Effect/GUITransitionOpen.mp3");
    m_panel->show();
    
    m_ctrlLayer->setMaskEnabled(true);
}

void CCStageSceneLayer::onClickPanelBattle( CCObject* pObj )
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);

    CUserData::instance()->m_stageSel = m_selIndex;
    
    //gc->replaceSceneWithLoading(&CCBattleSceneLayer::scene);
    CCDirector::sharedDirector()->replaceScene(CCBattleSceneLayer::scene());
    //CCDirector::sharedDirector()->replaceScene(CCHeroRoomSceneLayer::scene());
}

void CCStageSceneLayer::onClickPanelClose( CCObject* pObj )
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);
    gc->playSound("sounds/Effect/GUITransitionOpen.mp3");
    m_panel->hide();

    m_ctrlLayer->setMaskEnabled(false);
}



