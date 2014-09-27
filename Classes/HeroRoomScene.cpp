#include "CommHeader.h"

#include "HeroRoomScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "StageScene.h"
#include "BattleScene.h"
#include "UnitLibraryForCC.h"
#include "Archive.h"
#include "LuaScriptEngine.h"
#include "LuaBindingForCC.h"


// HeroRoomSceneLayer
HeroRoomSceneLayer::HeroRoomSceneLayer()
{
    m_heroInfos[0].id = CUnitLibraryForCC::kBarracks;
    m_heroInfos[0].name = "Warrior";
    m_heroInfos[0].desc = "Sir.\nGerald\nLightSeeker";
    m_heroInfos[0].hp = 300.0f;
    m_heroInfos[0].atkVal = CAttackValue(0, 15);
    m_heroInfos[0].armVal = CArmorValue(0, 3);
    m_heroInfos[0].attackSpeed = 1 / 1.5f;
    m_heroInfos[0].moveSpeed = 80.0f;
    m_heroInfos[0].exp = 0;

    m_heroInfos[1].id = CUnitLibraryForCC::kArcher;
    m_heroInfos[1].name = "Archer";
    m_heroInfos[1].desc = "Alleria\nSwiftWind";
    m_heroInfos[1].hp = 240.0f;
    m_heroInfos[1].atkVal = CAttackValue(0, 18);
    m_heroInfos[1].armVal = CArmorValue(0, 2);
    m_heroInfos[1].attackSpeed = 1 / 1.2f;
    m_heroInfos[1].moveSpeed = 85.0f;
    m_heroInfos[1].exp = 0;

    m_heroInfos[2].id = CUnitLibraryForCC::kWizard;
    m_heroInfos[2].name = "Wizard";
    m_heroInfos[2].desc = "Nivus";
    m_heroInfos[2].hp = 230.0f;
    m_heroInfos[2].atkVal = CAttackValue(1, 17);
    m_heroInfos[2].armVal = CArmorValue(1, 1);
    m_heroInfos[2].attackSpeed = 1 / 1.5f;
    m_heroInfos[2].moveSpeed = 75.0f;
    m_heroInfos[2].exp = 0;

    m_maxVal.hp = 300.0f;
    m_maxVal.atkVal = CAttackValue(0, 20);
    m_maxVal.armVal = CArmorValue(0, 5);
    m_maxVal.attackSpeed = 1 / 1.0f;
    m_maxVal.moveSpeed = 100.0f;
}

HeroRoomSceneLayer::~HeroRoomSceneLayer()
{
}

Scene* HeroRoomSceneLayer::scene()
{
    // 'scene' is an autorelease object
    Scene* pScene = Scene::create();

    HeroRoomSceneLayer* layer = HeroRoomSceneLayer::create();

    // add layer as a child to scene
    if (layer != nullptr)
    {
        M_DEF_GC(gc);
        //gc->preloadSound("sounds/Effects/xxxxxxx.mp3");

        pScene->addChild(layer);
    }

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool HeroRoomSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    //if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
    {
        return false;
    }

    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);

    gc->loadFrames("Global0");
    gc->loadFrames("Global1");
    gc->loadFrames("Global2");
    gc->preloadSound("sounds/Effects/UIMove.mp3");
    gc->preloadSound("sounds/Effects/LevelUp.mp3");
    gc->preloadSound("sounds/Effects/GUITransitionOpen.mp3");
    gc->playMusic("sounds/Backgrounds/MainBackground.mp3");

    auto L = CLuaScriptEngine::instance()->getLuaHandle();
    int res = luaL_includefile(L, "lib/UnitLibrary.lua");
    if (res != LUA_OK)
    {
        return false;
    }

    //m_panel = DCAST(createFirstHeroPanel(), Sprite*);
    m_panel = DCAST(createSecondHeroPanel(), Sprite*);
    m_panel->runAction(EaseExponentialOut::create(MoveTo::create(0.8f, Point(wsz.width * 0.5, wsz.height * 0.5))));

    return true;
}

Node* HeroRoomSceneLayer::createFirstHeroPanel()
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    Sprite* sp = nullptr;
    Sprite* sp2 = nullptr;
    Label* lbl = nullptr;
    char szVal[64];

    Sprite* panel = Sprite::create("UI/PanelBig.png");
    addChild(panel);
    Size sz = panel->getContentSize();
    panel->setPosition(Point(wsz.width * 0.5, wsz.height + sz.height * panel->getScaleY() * 0.5));

    // 标题
    sp = Sprite::create("UI/Title.png");
    lbl = Label::createWithTTF("Hero Room", FONT_YIKES, 72);
    lbl->setColor(Color3B(80, 60, 50));
    sp->addChild(lbl);
    lbl->setPosition(sp->getAnchorPointInPoints() + Point(0.0f, 20));
    panel->addChild(sp);
    sp->setPosition(Point(sz.width * 0.5, sz.height - lbl->getContentSize().height - 100));

    // 头像框
    ButtonPanel* bp = ButtonPanel::create(1, 3, Size(156, 164), 11, 11, Sprite::create("UI/HeroInfo/HeroesBorder.png"), 0.0f, 38.0f);
    bp->setTag(3);
    panel->addChild(bp);
    bp->setPosition(Point(sz.width * 0.05 + bp->getContentSize().width * 0.5, sz.height * 0.5 + 150));
    bp->getBackground()->setLocalZOrder(10);

    ButtonNormal* btn = ButtonNormal::createWithFrameName("UI/Portrait/WarriorSmall.png", "UI/Portrait/WarriorSmall.png", nullptr, nullptr, nullptr, 0.0f, CC_CALLBACK_1(HeroRoomSceneLayer::onClickHeroPortrait, this), nullptr);
    bp->addButton(btn, 0, 0);

    setSelectIndex(0);

    m_selSmall = Sprite::createWithSpriteFrameName("UI/Portrait/SelectSmall.png");
    bp->addChild(m_selSmall, 1);
    m_selSmall->setPosition(btn->getPosition());

    m_blinkSmall = Sprite::createWithSpriteFrameName("UI/Portrait/BlinkSmall.png");
    m_selSmall->addChild(m_blinkSmall, 1);
    m_blinkSmall->setPosition(m_selSmall->getAnchorPointInPoints());
    m_blinkSmall->setOpacity(0);

    btn = ButtonNormal::createWithFrameName("UI/Portrait/ArcherSmall.png", "UI/Portrait/ArcherSmall.png", nullptr, nullptr, nullptr, 0.0f, CC_CALLBACK_1(HeroRoomSceneLayer::onClickHeroPortrait, this), nullptr);
    bp->addButton(btn, 1, 0);

    btn = ButtonNormal::createWithFrameName("UI/Portrait/WizardSmall.png", "UI/Portrait/WizardSmall.png", nullptr, nullptr, nullptr, 0.0f, CC_CALLBACK_1(HeroRoomSceneLayer::onClickHeroPortrait, this), nullptr);
    bp->addButton(btn, 2, 0);

    // 信息框
    Sprite* info = Sprite::create("UI/HeroInfo/HeroInfo.png");
    panel->addChild(info, 10);
    info->setPosition(Point(sz.width * 0.65, sz.height * 0.5));

    m_selBig = Sprite::createWithSpriteFrameName("UI/Portrait/WarriorBig.png");
    panel->addChild(m_selBig);
    m_selBig->setPosition(info->getPosition() + Point(-275, -30));

    m_blinkBig = Sprite::createWithSpriteFrameName("UI/Portrait/BlinkBig.png");
    m_selBig->addChild(m_blinkBig, 1);
    m_blinkBig->setPosition(m_selBig->getAnchorPointInPoints());
    m_blinkBig->setOpacity(0);

    // 图标信息
    // hp
    float offsetX = info->getContentSize().width * 0.5 + 30;
    float offsetY = info->getContentSize().height * 0.5;
    sp = Sprite::createWithSpriteFrameName("UI/HeroInfo/HP.png");
    info->addChild(sp);
    sp->setPosition(Point(offsetX, offsetY));

    sp2 = Sprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(Color3B(255, 233, 16));
    m_hpBar = ProgressBar::create(Size(300, sp->getContentSize().height * 0.7), sp2, Sprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_hpBar);
    m_hpBar->setPosition(sp->getPosition() + Point(m_hpBar->getContentSize().width * 0.5 + 50, -7));
    m_hpBar->runActionForTimer(EaseExponentialOut::create(m_hpBar->setPercentageAction(m_heroInfos[0].hp / m_maxVal.hp, 0.2f)));

    sprintf(szVal, "%d", toInt(m_heroInfos[0].hp));
    m_hpLbl = Label::createWithTTF(szVal, FONT_COMIC_BOOK, 30);
    m_hpLbl->setAnchorPoint(Point(0.0f, 0.5f));
    m_hpLbl->setHorizontalAlignment(TextHAlignment::LEFT);
    info->addChild(m_hpLbl);
    m_hpLbl->setPosition(m_hpBar->getPosition() + Point(m_hpBar->getContentSize().width * 0.5 + 30, 5.0f));

    // attack
    offsetY -= sp->getContentSize().height + 10;
    sp = Sprite::createWithSpriteFrameName("UI/HeroInfo/PhysicalAttack.png");
    info->addChild(sp);
    sp->setPosition(Point(offsetX, offsetY));
    m_attackIcon = sp;

    sp2 = Sprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(Color3B(255, 233, 16));
    m_attackBar = ProgressBar::create(Size(300, sp->getContentSize().height * 0.7), sp2, Sprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_attackBar);
    m_attackBar->setPosition(sp->getPosition() + Point(m_attackBar->getContentSize().width * 0.5 + 50, -7));
    m_attackBar->runActionForTimer(EaseExponentialOut::create(m_attackBar->setPercentageAction(m_heroInfos[0].atkVal.getValue() / m_maxVal.atkVal.getValue(), 0.2f)));

    sprintf(szVal, "%d", toInt(m_heroInfos[0].atkVal.getValue()));
    m_attackLbl = Label::createWithTTF(szVal, FONT_COMIC_BOOK, 30);
    m_attackLbl->setAnchorPoint(Point(0.0f, 0.5f));
    m_attackLbl->setHorizontalAlignment(TextHAlignment::LEFT);
    info->addChild(m_attackLbl);
    m_attackLbl->setPosition(m_attackBar->getPosition() + Point(m_attackBar->getContentSize().width * 0.5 + 30, 5.0f));

    // armor
    offsetY -= sp->getContentSize().height + 10;
    sp = Sprite::createWithSpriteFrameName("UI/HeroInfo/PhysicalArmor.png");
    info->addChild(sp);
    sp->setPosition(Point(offsetX, offsetY));
    m_armorIcon = sp;

    sp2 = Sprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(Color3B(255, 233, 16));
    m_armorBar = ProgressBar::create(Size(300, sp->getContentSize().height * 0.7), sp2, Sprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_armorBar);
    m_armorBar->setPosition(sp->getPosition() + Point(m_armorBar->getContentSize().width * 0.5 + 50, -7));
    m_armorBar->runActionForTimer(EaseExponentialOut::create(m_armorBar->setPercentageAction(m_heroInfos[0].armVal.getValue() / m_maxVal.armVal.getValue(), 0.2f)));

    sprintf(szVal, "%d", toInt(m_heroInfos[0].armVal.getValue()));
    m_armorLbl = Label::createWithTTF(szVal, FONT_COMIC_BOOK, 30);
    m_armorLbl->setAnchorPoint(Point(0.0f, 0.5f));
    m_armorLbl->setHorizontalAlignment(TextHAlignment::LEFT);
    info->addChild(m_armorLbl);
    m_armorLbl->setPosition(m_armorBar->getPosition() + Point(m_armorBar->getContentSize().width * 0.5 + 30, 5.0f));

    // attack speed
    offsetY -= sp->getContentSize().height + 10;
    sp = Sprite::createWithSpriteFrameName("UI/HeroInfo/AttackSpeed.png");
    info->addChild(sp);
    sp->setPosition(Point(offsetX, offsetY));

    sp2 = Sprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(Color3B(255, 233, 16));
    m_attackSpeedBar = ProgressBar::create(Size(300, sp->getContentSize().height * 0.7), sp2, Sprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_attackSpeedBar);
    m_attackSpeedBar->setPosition(sp->getPosition() + Point(m_attackSpeedBar->getContentSize().width * 0.5 + 50, -7));
    m_attackSpeedBar->runActionForTimer(EaseExponentialOut::create(m_attackSpeedBar->setPercentageAction(m_heroInfos[0].attackSpeed / m_maxVal.attackSpeed, 0.2f)));

    sprintf(szVal, "%.2f", m_heroInfos[0].attackSpeed);
    m_attackSpeedLbl = Label::createWithTTF(szVal, FONT_COMIC_BOOK, 30);
    m_attackSpeedLbl->setAnchorPoint(Point(0.0f, 0.5f));
    m_attackSpeedLbl->setHorizontalAlignment(TextHAlignment::LEFT);
    info->addChild(m_attackSpeedLbl);
    m_attackSpeedLbl->setPosition(m_attackSpeedBar->getPosition() + Point(m_attackSpeedBar->getContentSize().width * 0.5 + 30, 5.0f));

    // move speed
    offsetY -= sp->getContentSize().height + 10;
    sp = Sprite::createWithSpriteFrameName("UI/HeroInfo/MoveSpeed.png");
    info->addChild(sp);
    sp->setPosition(Point(offsetX, offsetY));

    sp2 = Sprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(Color3B(255, 233, 16));
    m_moveSpeedBar = ProgressBar::create(Size(300, sp->getContentSize().height * 0.7), sp2, Sprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_moveSpeedBar);
    m_moveSpeedBar->setPosition(sp->getPosition() + Point(m_moveSpeedBar->getContentSize().width * 0.5 + 50, -7));
    m_moveSpeedBar->runActionForTimer(EaseExponentialOut::create(m_moveSpeedBar->setPercentageAction(m_heroInfos[0].moveSpeed / m_maxVal.moveSpeed, 0.2f)));

    sprintf(szVal, "%d", toInt(m_heroInfos[0].moveSpeed));
    m_moveSpeedLbl = Label::createWithTTF(szVal, FONT_COMIC_BOOK, 30);
    m_moveSpeedLbl->setAnchorPoint(Point(0.0f, 0.5f));
    m_moveSpeedLbl->setHorizontalAlignment(TextHAlignment::LEFT);
    info->addChild(m_moveSpeedLbl);
    m_moveSpeedLbl->setPosition(m_moveSpeedBar->getPosition() + Point(m_moveSpeedBar->getContentSize().width * 0.5 + 30, 5.0f));

    // 名字、简介
    m_name = Label::createWithTTF(m_heroInfos[0].name.c_str(), FONT_COMIC_BOOK, 52);
    info->addChild(m_name);
    m_name->setPosition(Point(800, 680));

    m_desc = Label::createWithTTF(m_heroInfos[0].desc.c_str(), FONT_COMIC_BOOK, 32);
    m_desc->setAnchorPoint(Point(0.5f, 1.0f));
    info->addChild(m_desc);
    m_desc->setPosition(m_name->getPosition() + Point(0, -m_name->getContentSize().height));

    // 按钮等UI
    Menu* mn = Menu::create();
    panel->addChild(mn);
    mn->setPosition(Point::ZERO);

    btn = ButtonNormal::createWithFile("UI/Button/BtnDoneNor.png", "UI/Button/BtnDoneSel.png", nullptr, nullptr, nullptr, 0.0f, CC_CALLBACK_1(HeroRoomSceneLayer::onClickButtonDone, this), nullptr);
    mn->addChild(btn);
    btn->setPosition(Point(sz.width - btn->getContentSize().width * 0.5 - 40, btn->getContentSize().height * 0.5 + 40));

    // 英雄动画
    m_eff = Effect::create("Sprites/Barracks/move", 0.1f);
    m_eff->addAnimation("Sprites/Barracks/act5", 0.1f);
    m_eff->setAnchorPoint(Point(0.5f, 0.1f));
    panel->addChild(m_eff, 10);
    m_eff->setPosition(Point(bp->getPositionX(), sz.height * 0.5 - 380));
    m_eff->setScale(6.0f);
    m_eff->setOpacity(160);
    m_eff->runAction(Sequence::createWithTwoActions(Spawn::createWithTwoActions(EaseBounceIn::create(ScaleTo::create(0.2f, 2.5f)), FadeTo::create(0.2f, 255)), CallFuncN::create(CC_CALLBACK_1(HeroRoomSceneLayer::onScaleDone, this))));

    // 缩放panel
    panel->setScale(min(wsz.width / sz.width, wsz.height / sz.height));
    sz = sz * panel->getScale();

    return panel;
}

Node* HeroRoomSceneLayer::createSecondHeroPanel()
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    Sprite* sp2 = nullptr;
    Label* lbl = nullptr;
    //char szVal[64];

    CUnitLibrary::instance()->loadDefaultLibrary();

    Sprite* panel = Sprite::create("UI/PanelBig.png");
    addChild(panel);
    Size sz = panel->getContentSize();
    panel->setPosition(Point(wsz.width * 0.5, wsz.height + sz.height * panel->getScaleY() * 0.5));

    Menu* mn = Menu::create();
    panel->addChild(mn);
    mn->setTag(101);
    mn->setPosition(Point::ZERO);

    //CUserData::instance()->loadUserData();
    CValue* ud = CUserData::instance()->m_ud;

    CValueMap* heroes = vmg_vm(ud, "heroes");

    int num = heroes->getDataMap().size();
    if (num == 0)
    {
        auto lbl = Label::createWithTTF("No Hero", FONT_ARIAL);
        panel->addChild(lbl);
        lbl->setPosition(panel->getAnchorPointInPoints());
    }
    else
    {
        static int index;
        index = 1;

        auto mi = MenuItemFont::create("Prev", bind([this](Ref*, Node* panel, int num)
        {
            if (index == 1)
            {
                index = num;
            }
            else
            {
                --index;
            }

            updateHeroSprite(index, panel);
        }, placeholders::_1, panel, num));
        mn->addChild(mi);
        mi->setPosition(Point(sz.width * 0.2, sz.height * 0.5));

        mi = MenuItemFont::create("Next", bind([this](Ref*, Node* panel, int num)
        {
            if (index == num)
            {
                index = 1;
            }
            else
            {
                ++index;
            }
            updateHeroSprite(index, panel);
        }, placeholders::_1, panel, num));
        mn->addChild(mi);
        mi->setPosition(Point(sz.width * 0.8, sz.height * 0.5));

        updateHeroSprite(index, panel);
    }

    auto mi = MenuItemFont::create("Confirm", [](Ref*)
    {
        Director::getInstance()->replaceScene(BattleSceneLayer::scene());
        CUserData::instance()->saveUserData();
    });
    mn->addChild(mi);
    mi->setPosition(Point(sz.width - mi->getContentSize().width * 0.5 - 50, mi->getContentSize().height * 0.5 + 50));


    // 缩放panel
    panel->setScale(min(wsz.width / sz.width, wsz.height / sz.height));
    sz = sz * panel->getScale();

    return panel;
}

void HeroRoomSceneLayer::updateHeroSprite(int index, Node* panel)
{
    auto ud = CUserData::instance()->m_ud;
    auto heroes = vmg_vm(ud, "heroes");
    auto hero = vmg_vm(heroes, index);
    auto id = vmg_int(hero, "id");
    auto u = CUnitLibrary::instance()->copyUnit(id);
    CUnitDrawForCC* d = DCAST(u->getDraw(), CUnitDrawForCC*);

    auto mn = DCAST(panel->getChildByTag(101), Menu*);
    auto sp = DCAST(panel->getChildByTag(102), Sprite*);
    auto spMi = DCAST(mn->getChildByTag(103), MenuItemFont*);
    
    if (sp != nullptr)
    {
        sp->removeFromParentAndCleanup(true);
    }
    sp = d->createSprite();
    sp->setTag(102);
    panel->addChild(sp);
    sp->setPosition(panel->getAnchorPointInPoints());

    if (spMi != nullptr)
    {
        spMi->removeFromParentAndCleanup(true);
    }

    spMi = MenuItemFont::create("[Select]", [ud, index](Ref* ref)
    {
        auto heroes = vmg_vm(ud, "heroes");
        auto hero = vmg_vm(heroes, index);
        auto order = vmg_int(hero, "order");
        auto useNum = vmg_int(ud, "heroesUseNum");
        auto mi = DCAST(ref, MenuItemFont*);
        if (order > 0)
        {
            // 点击卸载英雄

            auto& dm = heroes->getDataMap();
            auto n = (int)dm.size();
            for (auto i = 1; i <= n; ++i)
            {
                auto _hero = vmg_vm(heroes, i);
                auto _order = vmg_int(_hero, "order");
                if (_order > order)
                {
                    --_order;
                    vms_int(_hero, "order", _order);
                }
            }

            order = 0;
            --useNum;
            vms_int(hero, "order", order);
            vms_int(ud, "heroesUseNum", useNum);
            mi->setString("[Select]");
        }
        else
        {
            // 点击装备英雄
            ++useNum;
            order = useNum;
            char sz[64];
            sprintf(sz, "[%d]", order);
            mi->setString(sz);
        }
        vms_int(hero, "order", order);
        vms_int(ud, "heroesUseNum", useNum);

    });
    spMi->setTag(103);
    mn->addChild(spMi);
    spMi->setPosition(sp->getPosition() + Point(0, -spMi->getContentSize().height));

    auto order = vmg_int(hero, "order");
    if (order > 0)
    {
        char sz[64];
        sprintf(sz, "[%d]", order);
        spMi->setString(sz);
    }
    else
    {
        spMi->setString("[Select]");
    }
};

void HeroRoomSceneLayer::onClickHeroPortrait(Ref* pNode)
{
    M_DEF_GC(gc);
    gc->playSound("sounds/Effects/UIMove.mp3");
    ButtonNormal* btn = DCAST(pNode, ButtonNormal*);
    int index = btn->getButtonIndex();
    setSelectIndex(index);

    m_selSmall->setPosition(DCAST(pNode, Node*)->getPosition());
    m_blinkSmall->stopAllActions();
    m_blinkSmall->setOpacity(255);
    m_blinkSmall->runAction(FadeOut::create(0.3f));

    static const char* bigPortraits[] = {"UI/Portrait/WarriorBig.png", "UI/Portrait/ArcherBig.png", "UI/Portrait/WizardBig.png"};
    m_selBig->setSpriteFrame(gc->getfc()->getSpriteFrameByName(bigPortraits[index]));
    m_blinkBig->stopAllActions();
    m_blinkBig->setOpacity(255);
    m_blinkBig->runAction(FadeOut::create(0.3f));

    static const char* def[] = {"Sprites/Barracks/default", "Sprites/Archer/default", "Sprites/Wizard/default"};
    static const char* move[] = {"Sprites/Barracks/move", "Sprites/Archer/move", "Sprites/Wizard/move"};
    static const char* act5[] = {"Sprites/Barracks/act5", "Sprites/Archer/act5", "Sprites/Wizard/act5"};
    static const Point ap[] = {Point(0.5f, 0.1f), Point(0.5f, 0.1f), Point(0.5f, 0.2f)};
    static const char* attackIcon[] = {"UI/HeroInfo/PhysicalAttack.png", "UI/HeroInfo/MagicalAttack.png"};
    static const char* armorIcon[] = {"UI/HeroInfo/PhysicalArmor.png", "UI/HeroInfo/MagicalArmor.png"};

    Node* old = m_eff;
    m_eff = Effect::create(move[index], 0.1f);
    m_eff->addAnimation(act5[index], 0.1f);
    m_eff->setAnchorPoint(ap[index]);
    m_panel->addChild(m_eff, 10);
    m_eff->setPosition(old->getPosition());
    m_eff->setScale(6.0f);
    m_eff->setOpacity(160);
    m_eff->setSpriteFrame(gc->getFrame(def[index]));
    m_eff->runAction(Sequence::createWithTwoActions(Spawn::createWithTwoActions(EaseBounceOut::create(ScaleTo::create(0.3f, 2.5f)), FadeTo::create(0.3f, 255)), CCCallFuncN::create(CC_CALLBACK_1(HeroRoomSceneLayer::onScaleDone, this))));
    old->removeFromParentAndCleanup(true);

    m_attackIcon->setSpriteFrame(gc->getfc()->getSpriteFrameByName(attackIcon[m_heroInfos[index].atkVal.getType()]));
    m_armorIcon->setSpriteFrame(gc->getfc()->getSpriteFrameByName(armorIcon[m_heroInfos[index].armVal.getType()]));
    m_hpBar->runActionForTimer(EaseExponentialOut::create(m_hpBar->setPercentageAction(m_heroInfos[index].hp / m_maxVal.hp, 0.3f)));
    m_attackBar->runActionForTimer(EaseExponentialOut::create(m_attackBar->setPercentageAction(m_heroInfos[index].atkVal.getValue() / m_maxVal.atkVal.getValue(), 0.3f)));
    m_armorBar->runActionForTimer(EaseExponentialOut::create(m_armorBar->setPercentageAction(m_heroInfos[index].armVal.getValue() / m_maxVal.armVal.getValue(), 0.3f)));
    m_attackSpeedBar->runActionForTimer(EaseExponentialOut::create(m_attackSpeedBar->setPercentageAction(m_heroInfos[index].attackSpeed / m_maxVal.attackSpeed, 0.3f)));
    m_moveSpeedBar->runActionForTimer(EaseExponentialOut::create(m_moveSpeedBar->setPercentageAction(m_heroInfos[index].moveSpeed / m_maxVal.moveSpeed, 0.3f)));

    char szVal[64];
    sprintf(szVal, "%d", toInt(m_heroInfos[index].hp));
    m_hpLbl->setString(szVal);
    sprintf(szVal, "%d", toInt(m_heroInfos[index].atkVal.getValue()));
    m_attackLbl->setString(szVal);
    sprintf(szVal, "%d", toInt(m_heroInfos[index].armVal.getValue()));
    m_armorLbl->setString(szVal);
    sprintf(szVal, "%.2f", m_heroInfos[index].attackSpeed);
    m_attackSpeedLbl->setString(szVal);
    sprintf(szVal, "%d", toInt(m_heroInfos[index].moveSpeed));
    m_moveSpeedLbl->setString(szVal);

    m_name->setString(m_heroInfos[index].name.c_str());
    m_desc->setString(m_heroInfos[index].desc.c_str());
}

void HeroRoomSceneLayer::onClickButtonDone(Ref* pNode)
{
    auto mn = DCAST(m_panel->getChildByTag(3), ButtonPanel*)->getInnerMenu();
    mn->setEnabled(false);

    M_DEF_GC(gc);
    ((ButtonNormal*)pNode)->setEnabled(false);
    m_eff->play(1, 1.0f, 1, CallFuncN::create(CC_CALLBACK_1(HeroRoomSceneLayer::onAct5Done, this)));
    gc->playSound("sounds/Effects/LevelUp.mp3");
    
}

void HeroRoomSceneLayer::onHideDone(Node* pNode)
{
    M_DEF_GC(gc);
    CUserData* udt = CUserData::instance();
    
    udt->m_heroes.push_back(m_heroInfos[m_selIndex]);
    udt->m_heroSel = 0;

    //gc->replaceSceneWithLoading(&StageSceneLayer::scene);
    Director::getInstance()->replaceScene(StageSceneLayer::scene());
    //Director::getInstance()->replaceScene(BattleSceneLayer::scene());
}

void HeroRoomSceneLayer::onScaleDone(Node* pNode)
{
    m_eff->playForever();
}

void HeroRoomSceneLayer::onAct5Done(Node* pNode)
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);
    //gc->playSound("sounds/Effects/UIMove.mp3");
    m_panel->runAction(Sequence::createWithTwoActions(EaseExponentialOut::create(MoveTo::create(0.8f, Point(wsz.width * 0.5, wsz.height + m_panel->getContentSize().height * m_panel->getScaleY() * 0.5))), CallFuncN::create(CC_CALLBACK_1(HeroRoomSceneLayer::onHideDone, this))));
    
    //Sprite* sp = Sprite::create("UI/Loading.png");
    //addChild(sp);
    //sp->setScale(wsz.width / sp->getContentSize().width * 0.3f);
    //sp->setPosition(Point(wsz.width * 0.5, wsz.height * 0.5));
    //gc->playSound("sounds/Effects/GUITransitionOpen.mp3");
}

