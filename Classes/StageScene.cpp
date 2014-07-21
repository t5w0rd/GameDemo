#include "CommHeader.h"

#include "StageScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "Stage.h"
#include "UserData.h"
#include "GameData.h"
#include "BattleScene.h"
#include "AbilityScene.h"


// StageSceneLayer
StageSceneLayer::StageSceneLayer()
: m_panel(nullptr)
, m_name(nullptr)
, m_desc(nullptr)
, m_selIndex(0)
, m_btnBattle(nullptr)
, m_stageCur(nullptr)
{
}

StageSceneLayer::~StageSceneLayer()
{
}

Scene* StageSceneLayer::scene()
{
    // 'scene' is an autorelease object
    Scene* pScene = Scene::create();

    StageSceneLayer* layer = StageSceneLayer::create();

    // add layer as a child to scene
    if (layer != nullptr)
    {
        M_DEF_GC(gc);
        //gc->preloadSound("sounds/Effects/xxxxxxx.mp3");

        pScene->addChild(layer);
        pScene->addChild(layer->m_ctrlLayer, 1);
    }

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool StageSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!WinLayer::init())
    {
        return false;
    }

    // ctrl layer
    m_ctrlLayer = TouchMaskLayer::create(Color4B(0, 0, 0, 0), 80);

    M_DEF_GC(gc);
    gc->playMusic("sounds/Backgrounds/Map_Theme.mp3");
    addChild(gc->defaultLoadingLayer(), 100, 100);

    vector<string> ts;
    ts.push_back("Global0");
    ts.push_back("Global1");
    ts.push_back("Global2");
    ts.push_back("Heroes0");
    ts.push_back("Heroes1");
    ts.push_back("Heroes2");
    ts.push_back("Heroes3");
    ts.push_back("Heroes4");
    ts.push_back("Heroes5");
    ts.push_back("Projectiles0");
    ts.push_back("Battle0");

    vector<string> ot;
    ot.push_back("UI/Stage/MapBackground.png");
    ot.push_back("UI/PanelBig.png");
    gc->loadTexturesAsync(ts, ot, CC_CALLBACK_0(StageSceneLayer::onLoadingProgress, this), CC_CALLBACK_0(StageSceneLayer::onLoadingDone, this));

    return true;
}

void StageSceneLayer::onLoadingProgress()
{
    M_DEF_GC(gc);
    if (gc->getLoaded() < 2)
    {
        return;
    }

    float per = gc->getLoaded() * 1.0f / gc->getLoadCount();
    auto pb = DCAST(getChildByTag(100)->getChildByTag(101), ProgressBar*);
    if (pb == nullptr)
    {
        pb = ProgressBar::create(Size(wsz().width * 0.5, 50.0f), Sprite::createWithSpriteFrameName("UI/status/HpBarFill.png"), Sprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
        getChildByTag(100)->addChild(pb, 1, 101);
        pb->setPosition(Point(wsz().width * 0.5, wsz().height * 0.3));
    }
    //pb->stopAllActions();
    //pb->runActionForTimer(EaseExponentialOut::create(pb->setPercentageAction(per, 0.1f, per < 100 ? nullptr : CallFunc::create(CC_CALLBACK_0(BattleSceneLayer::onLoadingDone, this)))));
    pb->setPercentage(per);
    if (per >= 1.0f)
    {
        pb->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create(CC_CALLBACK_0(StageSceneLayer::onLoadingDone, this))));
    }
}

void StageSceneLayer::onLoadingDone()
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);
    Sprite* sp = nullptr;
    Label* lbl = nullptr;

    removeChildByTag(100);

    setBackgroundSprite(Sprite::create("UI/Stage/MapBackground.png"));
    setPosition(Point(0.0f, 0.0f));

    gc->preloadSound("sounds/Effects/UIMove.mp3");

    // 布置pannel
    m_panel = PopPanel::create(Sprite::create("UI/PanelBig.png"));
    m_ctrlLayer->addChild(m_panel);
    Size psz = m_panel->getContentSize();

    //m_panel->getMenu()->setTouchPriority(-10);
    m_panel->getMenu()->setPosition(Point::ZERO);

    sp = Sprite::create("UI/Chain.png");
    m_panel->addChild(sp, -1);
    sp->setPosition(Point(psz.width * 0.2, psz.height + sp->getContentSize().height * 0.2));

    sp = Sprite::create("UI/Chain.png");
    m_panel->addChild(sp, -1);
    sp->setPosition(Point(psz.width * 0.8, psz.height + sp->getContentSize().height * 0.2));

    sp = Sprite::create("UI/Title.png");
    m_panel->addChild(sp, 1, 2);
    sp->setPosition(Point(psz.width * 0.5, psz.height));

    lbl = Label::createWithTTF("STAGES", FONT_YIKES, 72);
    lbl->setColor(Color3B(80, 60, 50));
    sp->addChild(lbl, 1, 2);
    lbl->setPosition(sp->getAnchorPointInPoints() + Point(0.0f, 20));

    Menu* mn = Menu::create();
    addChild(mn, 10);
    //mn->setTouchPriority(-1);
    mn->setPosition(Point::ZERO);

    m_stageMap.setPathName("UI/Stage/Path.png");
    m_stageMap.setPanel(mn, CC_CALLBACK_1(StageSceneLayer::onClickStage, this));

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

    m_btnBattle = ButtonNormal::create(
        Sprite::create("UI/Stage/BtnBattleNor.png"),
        Sprite::create("UI/Stage/BtnBattleSel.png"),
        Sprite::create("UI/Stage/BtnBattleDis.png"),
        nullptr,
        nullptr,
        0.0f,
        CC_CALLBACK_1(StageSceneLayer::onClickPanelBattle, this),
        nullptr);

    m_panel->addButton(m_btnBattle);
    m_btnBattle->setPosition(Point(psz.width - m_btnBattle->getContentSize().width * 0.5 - 50, m_btnBattle->getContentSize().height * 0.5 + 50));

    ButtonNormal* btnClose = ButtonNormal::create(
        Sprite::create("UI/Button/BtnCloseNor.png"),
        Sprite::create("UI/Button/BtnCloseSel.png"),
        Sprite::create("UI/Button/BtnCloseNor.png"),
        nullptr,
        nullptr,
        0.0f,
        CC_CALLBACK_1(StageSceneLayer::onClickPanelClose, this),
        nullptr);

    m_panel->addButton(btnClose);
    btnClose->setPosition(Point(psz.width - btnClose->getContentSize().width * 0.5 - 50, psz.height - btnClose->getContentSize().height * 0.5 - 50));

    // 名字、简介
    m_name = Label::createWithTTF(" ", FONT_COMIC_BOOK, 100, Size(psz.width * 0.6, 100 / 0.75f), TextHAlignment::LEFT);
    m_name->setVerticalAlignment(TextVAlignment::TOP);
    m_name->setAnchorPoint(Point(0.0f, 0.5f));
    m_panel->addChild(m_name);
    m_name->setPosition(Point(100, psz.height - m_name->getContentSize().height * 0.5 - 100));

    m_desc = Label::createWithTTF(" ", FONT_COMIC_BOOK, 60, Size(psz.width * 0.6, 60 / 0.75f * 5), TextHAlignment::LEFT);
    m_desc->setVerticalAlignment(TextVAlignment::TOP);
    m_desc->setAnchorPoint(Point(0.0f, 0.5f));
    m_panel->addChild(m_desc);
    m_desc->setPosition(Point(100, m_name->getPosition().y - m_name->getContentSize().height * 0.5 - m_desc->getContentSize().height * 0.5));

    for (int i = 0; i < 3; ++i)
    {
        m_stars[i] = Sprite::createWithSpriteFrameName("UI/Stage/Unstar.png");
        m_panel->addChild(m_stars[i]);
        m_stars[i]->setScale(2.0f);
        m_stars[i]->setPosition(Point(psz.width - m_stars[i]->getContentSize().width * m_stars[i]->getScale() * (3 - i) - 150, m_name->getPosition().y));
    }

    // panel缩放
    m_panel->setScale(min(wsz.width * 0.8 / psz.width, wsz.height * 0.8 / psz.height));
    psz = psz * m_panel->getScale();
    m_panel->setPosition(Point(wsz.width * 0.5, wsz.height + psz.height * 0.5));
}

void StageSceneLayer::onClickStage(Ref* pObj)
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);

    CGameData::VEC_STAGES& si = CGameData::instance()->m_stages;

    MenuItemImage* mi = DCAST(pObj, MenuItemImage*);
    m_selIndex = mi->getTag();
    CStage* stage = m_stageMap.getStage(m_selIndex);

    if (m_stageCur == nullptr)
    {
        m_stageCur = Sprite::create("UI/Stage/StageCur.png");
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
        DCAST(m_stars[i], Sprite*)->setSpriteFrame(gc->getfc()->getSpriteFrameByName(i < stage->getGrade() ? "UI/Stage/Star.png" : "UI/Stage/Unstar.png"));
    }

    // 弹出panel
    gc->playSound("sounds/Effects/GUITransitionOpen.mp3");
    m_panel->show();
    
    m_ctrlLayer->setMaskEnabled(true);
}

void StageSceneLayer::onClickPanelBattle(Ref* pObj)
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);

    CUserData::instance()->m_stageSel = m_selIndex;
    
    //gc->replaceSceneWithLoading(&BattleSceneLayer::scene);
    Director::getInstance()->replaceScene(BattleSceneLayer::scene());
    //Director::getInstance()->replaceScene(AbilitySceneLayer::scene());
    //Director::getInstance()->replaceScene(HeroRoomSceneLayer::scene());
}

void StageSceneLayer::onClickPanelClose(Ref* pObj)
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);
    gc->playSound("sounds/Effects/GUITransitionOpen.mp3");
    m_panel->hide();

    m_ctrlLayer->setMaskEnabled(false);
}



