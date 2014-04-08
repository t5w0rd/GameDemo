#include "CommHeader.h"

#include "HeroRoomScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "BattleScene.h"
#include "UnitLibraryForCC.h"


// CCHeroRoomSceneLayer
CCHeroRoomSceneLayer::CCHeroRoomSceneLayer()
{
    m_heroVals[0].id = CUnitLibraryForCC::kBarracks;
    m_heroVals[0].name = "Warrior";
    m_heroVals[0].desc = "Sir.\nGerald\nLightSeeker";
    m_heroVals[0].hp = 300.0f;
    m_heroVals[0].atkVal = CAttackValue(0, 15);
    m_heroVals[0].armVal = CArmorValue(0, 3);
    m_heroVals[0].attackSpeed = 1 / 1.5f;
    m_heroVals[0].moveSpeed = 80.0f;

    m_heroVals[1].id = CUnitLibraryForCC::kArcher;
    m_heroVals[1].name = "Archer";
    m_heroVals[1].desc = "Alleria\nSwiftWind";
    m_heroVals[1].hp = 240.0f;
    m_heroVals[1].atkVal = CAttackValue(0, 18);
    m_heroVals[1].armVal = CArmorValue(0, 2);
    m_heroVals[1].attackSpeed = 1 / 1.2f;
    m_heroVals[1].moveSpeed = 85.0f;

    m_heroVals[2].id = CUnitLibraryForCC::kWizard;
    m_heroVals[2].name = "Wizard";
    m_heroVals[2].desc = "Nivus";
    m_heroVals[2].hp = 230.0f;
    m_heroVals[2].atkVal = CAttackValue(1, 17);
    m_heroVals[2].armVal = CArmorValue(1, 1);
    m_heroVals[2].attackSpeed = 1 / 1.5f;
    m_heroVals[2].moveSpeed = 75.0f;

    m_maxVal.hp = 300.0f;
    m_maxVal.atkVal = CAttackValue(0, 20);
    m_maxVal.armVal = CArmorValue(0, 5);
    m_maxVal.attackSpeed = 1 / 1.0f;
    m_maxVal.moveSpeed = 100.0f;
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
    CCSprite* sp2 = NULL;
    CCLabelTTF* lbl = NULL;
    char szVal[64];

    gc->loadTexture("Global0");
    gc->loadTexture("Global1");
    gc->preloadSound("sounds/Effect/UIMove.mp3");
    gc->preloadSound("sounds/Effect/LevelUp.mp3");
    gc->preloadSound("sounds/Effect/GUITransitionOpen.mp3");
    gc->playMusic("sounds/Background/MainBackground.mp3");

    m_panel = CCSprite::create("UI/PanelBig.png");
    addChild(m_panel);
    CCSize sz = m_panel->getContentSize();
    m_panel->setPosition(ccp(wsz.width * 0.5, wsz.height + sz.height * m_panel->getScaleY() * 0.5));

    // 标题
    sp = CCSprite::create("UI/Title.png");
    lbl = CCLabelTTF::create("Hero Room", "fonts/Yikes!.ttf", 72);
    lbl->setColor(ccc3(80, 60, 50));
    sp->addChild(lbl);
    lbl->setPosition(ccpAdd(sp->getAnchorPointInPoints(), ccp(0.0f, 20)));
    m_panel->addChild(sp);
    sp->setPosition(ccp(sz.width * 0.5, sz.height - lbl->getContentSize().height - 100));
    
    // 头像框
    CCButtonPanel* bp = CCButtonPanel::create(1, 3, 156, 164, 18, 11, CCSprite::create("UI/HeroInfo/HeroesBorder.png"), 0.0f, 38.0f);
    m_panel->addChild(bp);
    bp->setPosition(ccp(sz.width * 0.05 + bp->getContentSize().width * 0.5, sz.height * 0.5 + 150));
    bp->getBackground()->setZOrder(10);

    CCButtonNormal* btn = CCButtonNormal::createWithFrameName("UI/Portrait/WarriorSmall.png", "UI/Portrait/WarriorSmall.png", NULL, NULL, NULL, 0.0f, this, callfuncN_selector(CCHeroRoomSceneLayer::onClickHeroPortrait), NULL);
    bp->addButton(btn, 0, 0);

    setSelectIndex(0);

    m_selSmall = CCSprite::createWithSpriteFrameName("UI/Portrait/SelectSmall.png");
    bp->addChild(m_selSmall);
    m_selSmall->setPosition(btn->getPosition());
    
    m_blinkSmall = CCSprite::createWithSpriteFrameName("UI/Portrait/BlinkSmall.png");
    m_selSmall->addChild(m_blinkSmall);
    m_blinkSmall->setPosition(m_selSmall->getAnchorPointInPoints());
    m_blinkSmall->setOpacity(0);

    btn = CCButtonNormal::createWithFrameName("UI/Portrait/ArcherSmall.png", "UI/Portrait/ArcherSmall.png", NULL, NULL, NULL, 0.0f, this, callfuncN_selector(CCHeroRoomSceneLayer::onClickHeroPortrait), NULL);
    bp->addButton(btn, 1, 0);

    btn = CCButtonNormal::createWithFrameName("UI/Portrait/WizardSmall.png", "UI/Portrait/WizardSmall.png", NULL, NULL, NULL, 0.0f, this, callfuncN_selector(CCHeroRoomSceneLayer::onClickHeroPortrait), NULL);
    bp->addButton(btn, 2, 0);

    // 信息框
    CCSprite* info = CCSprite::create("UI/HeroInfo/HeroInfo.png");
    m_panel->addChild(info, 10);
    info->setPosition(ccp(sz.width * 0.65, sz.height * 0.5));

    m_selBig = CCSprite::createWithSpriteFrameName("UI/Portrait/WarriorBig.png");
    m_panel->addChild(m_selBig);
    m_selBig->setPosition(ccpAdd(info->getPosition(), ccp(-275, -30)));

    m_blinkBig = CCSprite::createWithSpriteFrameName("UI/Portrait/BlinkBig.png");
    m_selBig->addChild(m_blinkBig);
    m_blinkBig->setPosition(m_selBig->getAnchorPointInPoints());
    m_blinkBig->setOpacity(0);

    // 图标信息
    // hp
    float offsetX = info->getContentSize().width * 0.5 + 30;
    float offsetY = info->getContentSize().height * 0.5;
    sp = CCSprite::createWithSpriteFrameName("UI/HeroInfo/HP.png");
    info->addChild(sp);
    sp->setPosition(ccp(offsetX, offsetY));

    sp2 = CCSprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(ccc3(255, 233, 16));
    m_hpBar = CCProgressBar::create(CCSizeMake(300, sp->getContentSize().height * 0.7), sp2, CCSprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_hpBar);
    m_hpBar->setPosition(ccpAdd(sp->getPosition(), ccp(m_hpBar->getContentSize().width * 0.5 + 50, -7)));
    m_hpBar->runActionForTimer(CCEaseExponentialOut::create(m_hpBar->setPercentageAction(m_heroVals[0].hp * 100 / m_maxVal.hp, 0.2f)));

    sprintf(szVal, "%d", toInt(m_heroVals[0].hp));
    m_hpLbl = CCLabelTTF::create(szVal, "fonts/Comic Book.ttf", 30);
    m_hpLbl->setAnchorPoint(ccp(0.0f, 0.5f));
    m_hpLbl->setHorizontalAlignment(kCCTextAlignmentLeft);
    info->addChild(m_hpLbl);
    m_hpLbl->setPosition(ccpAdd(m_hpBar->getPosition(), ccp(m_hpBar->getContentSize().width * 0.5 + 30, 5.0f)));
    
    // attack
    offsetY -= sp->getContentSize().height + 10;
    sp = CCSprite::createWithSpriteFrameName("UI/HeroInfo/PhysicalAttack.png");
    info->addChild(sp);
    sp->setPosition(ccp(offsetX, offsetY));
    m_attackIcon = sp;

    sp2 = CCSprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(ccc3(255, 233, 16));
    m_attackBar = CCProgressBar::create(CCSizeMake(300, sp->getContentSize().height * 0.7), sp2, CCSprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_attackBar);
    m_attackBar->setPosition(ccpAdd(sp->getPosition(), ccp(m_attackBar->getContentSize().width * 0.5 + 50, -7)));
    m_attackBar->runActionForTimer(CCEaseExponentialOut::create(m_attackBar->setPercentageAction(m_heroVals[0].atkVal.getValue() * 100 / m_maxVal.atkVal.getValue(), 0.2f)));

    sprintf(szVal, "%d", toInt(m_heroVals[0].atkVal.getValue()));
    m_attackLbl = CCLabelTTF::create(szVal, "fonts/Comic Book.ttf", 30);
    m_attackLbl->setAnchorPoint(ccp(0.0f, 0.5f));
    m_attackLbl->setHorizontalAlignment(kCCTextAlignmentLeft);
    info->addChild(m_attackLbl);
    m_attackLbl->setPosition(ccpAdd(m_attackBar->getPosition(), ccp(m_attackBar->getContentSize().width * 0.5 + 30, 5.0f)));

    // armor
    offsetY -= sp->getContentSize().height + 10;
    sp = CCSprite::createWithSpriteFrameName("UI/HeroInfo/PhysicalArmor.png");
    info->addChild(sp);
    sp->setPosition(ccp(offsetX, offsetY));
    m_armorIcon = sp;

    sp2 = CCSprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(ccc3(255, 233, 16));
    m_armorBar = CCProgressBar::create(CCSizeMake(300, sp->getContentSize().height * 0.7), sp2, CCSprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_armorBar);
    m_armorBar->setPosition(ccpAdd(sp->getPosition(), ccp(m_armorBar->getContentSize().width * 0.5 + 50, -7)));
    m_armorBar->runActionForTimer(CCEaseExponentialOut::create(m_armorBar->setPercentageAction(m_heroVals[0].armVal.getValue() * 100 / m_maxVal.armVal.getValue(), 0.2f)));

    sprintf(szVal, "%d", toInt(m_heroVals[0].armVal.getValue()));
    m_armorLbl = CCLabelTTF::create(szVal, "fonts/Comic Book.ttf", 30);
    m_armorLbl->setAnchorPoint(ccp(0.0f, 0.5f));
    m_armorLbl->setHorizontalAlignment(kCCTextAlignmentLeft);
    info->addChild(m_armorLbl);
    m_armorLbl->setPosition(ccpAdd(m_armorBar->getPosition(), ccp(m_armorBar->getContentSize().width * 0.5 + 30, 5.0f)));

    // attack speed
    offsetY -= sp->getContentSize().height + 10;
    sp = CCSprite::createWithSpriteFrameName("UI/HeroInfo/AttackSpeed.png");
    info->addChild(sp);
    sp->setPosition(ccp(offsetX, offsetY));

    sp2 = CCSprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(ccc3(255, 233, 16));
    m_attackSpeedBar = CCProgressBar::create(CCSizeMake(300, sp->getContentSize().height * 0.7), sp2, CCSprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_attackSpeedBar);
    m_attackSpeedBar->setPosition(ccpAdd(sp->getPosition(), ccp(m_attackSpeedBar->getContentSize().width * 0.5 + 50, -7)));
    m_attackSpeedBar->runActionForTimer(CCEaseExponentialOut::create(m_attackSpeedBar->setPercentageAction(m_heroVals[0].attackSpeed * 100 / m_maxVal.attackSpeed, 0.2f)));

    sprintf(szVal, "%.2f", m_heroVals[0].attackSpeed);
    m_attackSpeedLbl = CCLabelTTF::create(szVal, "fonts/Comic Book.ttf", 30);
    m_attackSpeedLbl->setAnchorPoint(ccp(0.0f, 0.5f));
    m_attackSpeedLbl->setHorizontalAlignment(kCCTextAlignmentLeft);
    info->addChild(m_attackSpeedLbl);
    m_attackSpeedLbl->setPosition(ccpAdd(m_attackSpeedBar->getPosition(), ccp(m_attackSpeedBar->getContentSize().width * 0.5 + 30, 5.0f)));

    // move speed
    offsetY -= sp->getContentSize().height + 10;
    sp = CCSprite::createWithSpriteFrameName("UI/HeroInfo/MoveSpeed.png");
    info->addChild(sp);
    sp->setPosition(ccp(offsetX, offsetY));

    sp2 = CCSprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    //sp2->setColor(ccc3(255, 233, 16));
    m_moveSpeedBar = CCProgressBar::create(CCSizeMake(300, sp->getContentSize().height * 0.7), sp2, CCSprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png"), 0.0f, 0.0f, true);
    info->addChild(m_moveSpeedBar);
    m_moveSpeedBar->setPosition(ccpAdd(sp->getPosition(), ccp(m_moveSpeedBar->getContentSize().width * 0.5 + 50, -7)));
    m_moveSpeedBar->runActionForTimer(CCEaseExponentialOut::create(m_moveSpeedBar->setPercentageAction(m_heroVals[0].moveSpeed * 100 / m_maxVal.moveSpeed, 0.2f)));

    sprintf(szVal, "%d", toInt(m_heroVals[0].moveSpeed));
    m_moveSpeedLbl = CCLabelTTF::create(szVal, "fonts/Comic Book.ttf", 30);
    m_moveSpeedLbl->setAnchorPoint(ccp(0.0f, 0.5f));
    m_moveSpeedLbl->setHorizontalAlignment(kCCTextAlignmentLeft);
    info->addChild(m_moveSpeedLbl);
    m_moveSpeedLbl->setPosition(ccpAdd(m_moveSpeedBar->getPosition(), ccp(m_moveSpeedBar->getContentSize().width * 0.5 + 30, 5.0f)));

    // 名字、简介
    m_name = CCLabelTTF::create(m_heroVals[0].name, "fonts/Comic Book.ttf", 52);
    info->addChild(m_name);
    m_name->setPosition(ccp(800, 680));

    m_desc = CCLabelTTF::create(m_heroVals[0].desc, "fonts/Comic Book.ttf", 32);
    m_desc->setAnchorPoint(ccp(0.5f, 1.0f));
    info->addChild(m_desc);
    m_desc->setPosition(ccpAdd(m_name->getPosition(), ccp(0, -m_name->getContentSize().height)));

    // 按钮等UI
    CCMenu* mn = CCMenu::create();
    m_panel->addChild(mn);
    mn->setPosition(CCPointZero);

    btn = CCButtonNormal::createWithFile("UI/Button/DoneNormal.png", "UI/Button/DoneOn.png", NULL, NULL, NULL, 0.0f, this, callfuncN_selector(CCHeroRoomSceneLayer::onClickButtonDone), NULL);
    mn->addChild(btn);
    btn->setPosition(ccp(sz.width - btn->getContentSize().width * 0.5 - 40, btn->getContentSize().height * 0.5 + 40));

    // 英雄动画
    m_eff = CCEffect::create("Units/Barracks/move", 0.1f);
    m_eff->addAnimation("Units/Barracks/act5", 0.1f);
    m_eff->setAnchorPoint(ccp(0.5f, 0.1f));
    m_panel->addChild(m_eff, 10);
    m_eff->setPosition(ccp(bp->getPositionX(), sz.height * 0.5 - 380));
    m_eff->setScale(6.0f);
    m_eff->setOpacity(160);
    m_eff->runAction(CCSequence::createWithTwoActions(CCSpawn::createWithTwoActions(CCEaseBounceIn::create(CCScaleTo::create(0.2f, 2.5f)), CCFadeTo::create(0.2f, 255)), CCCallFuncN::create(this, callfuncN_selector(CCHeroRoomSceneLayer::onScaleDone))));
    
    // 缩放panel
    m_panel->setScale(min(wsz.width / sz.width, wsz.height / sz.height));
    sz = sz * m_panel->getScale();
    m_panel->runAction(CCEaseExponentialOut::create(CCMoveTo::create(0.8f, ccp(wsz.width * 0.5, wsz.height * 0.5))));

    return true;
}

void CCHeroRoomSceneLayer::onClickHeroPortrait( CCNode* pNode )
{
    M_DEF_GC(gc);
    gc->playSound("sounds/Effect/UIMove.mp3");
    CCButtonNormal* btn = DCAST(pNode, CCButtonNormal*);
    int index = btn->getButtonIndex();
    setSelectIndex(index);

    m_selSmall->setPosition(pNode->getPosition());
    m_blinkSmall->stopAllActions();
    m_blinkSmall->runAction(CCFadeOut::create(0.3f));

    static const char* bigPortraits[] = {"UI/Portrait/WarriorBig.png", "UI/Portrait/ArcherBig.png", "UI/Portrait/WizardBig.png"};
    m_selBig->setDisplayFrame(gc->getfc()->spriteFrameByName(bigPortraits[index]));
    m_blinkBig->stopAllActions();
    m_blinkBig->runAction(CCFadeOut::create(0.3f));

    static const char* def[] = {"Units/Barracks/default", "Units/Archer/default", "Units/Wizard/default"};
    static const char* move[] = {"Units/Barracks/move", "Units/Archer/move", "Units/Wizard/move"};
    static const char* act5[] = {"Units/Barracks/act5", "Units/Archer/act5", "Units/Wizard/act5"};
    static const CCPoint ap[] = {ccp(0.5f, 0.1f), ccp(0.5f, 0.1f), ccp(0.5f, 0.2f)};
    static const char* attackIcon[] = {"UI/HeroInfo/PhysicalAttack.png", "UI/HeroInfo/MagicalAttack.png"};
    static const char* armorIcon[] = {"UI/HeroInfo/PhysicalArmor.png", "UI/HeroInfo/MagicalArmor.png"};

    CCNode* old = m_eff;
    m_eff = CCEffect::create(move[index], 0.1f);
    m_eff->addAnimation(act5[index], 0.1f);
    m_eff->setAnchorPoint(ap[index]);
    m_panel->addChild(m_eff, 10);
    m_eff->setPosition(old->getPosition());
    m_eff->setScale(6.0f);
    m_eff->setOpacity(160);
    m_eff->setDisplayFrame(gc->getFrame(def[index]));
    m_eff->runAction(CCSequence::createWithTwoActions(CCSpawn::createWithTwoActions(CCEaseBounceOut::create(CCScaleTo::create(0.3f, 2.5f)), CCFadeTo::create(0.3f, 255)), CCCallFuncN::create(this, callfuncN_selector(CCHeroRoomSceneLayer::onScaleDone))));
    old->removeFromParentAndCleanup(true);

    m_attackIcon->setDisplayFrame(gc->getfc()->spriteFrameByName(attackIcon[m_heroVals[index].atkVal.getType()]));
    m_armorIcon->setDisplayFrame(gc->getfc()->spriteFrameByName(armorIcon[m_heroVals[index].armVal.getType()]));
    m_hpBar->runActionForTimer(CCEaseExponentialOut::create(m_hpBar->setPercentageAction(m_heroVals[index].hp * 100 / m_maxVal.hp, 0.3f)));
    m_attackBar->runActionForTimer(CCEaseExponentialOut::create(m_attackBar->setPercentageAction(m_heroVals[index].atkVal.getValue() * 100 / m_maxVal.atkVal.getValue(), 0.3f)));
    m_armorBar->runActionForTimer(CCEaseExponentialOut::create(m_armorBar->setPercentageAction(m_heroVals[index].armVal.getValue() * 100 / m_maxVal.armVal.getValue(), 0.3f)));
    m_attackSpeedBar->runActionForTimer(CCEaseExponentialOut::create(m_attackSpeedBar->setPercentageAction(m_heroVals[index].attackSpeed * 100 / m_maxVal.attackSpeed, 0.3f)));
    m_moveSpeedBar->runActionForTimer(CCEaseExponentialOut::create(m_moveSpeedBar->setPercentageAction(m_heroVals[index].moveSpeed * 100 / m_maxVal.moveSpeed, 0.3f)));

    char szVal[64];
    sprintf(szVal, "%d", toInt(m_heroVals[index].hp));
    m_hpLbl->setString(szVal);
    sprintf(szVal, "%d", toInt(m_heroVals[index].atkVal.getValue()));
    m_attackLbl->setString(szVal);
    sprintf(szVal, "%d", toInt(m_heroVals[index].armVal.getValue()));
    m_armorLbl->setString(szVal);
    sprintf(szVal, "%.2f", m_heroVals[index].attackSpeed);
    m_attackSpeedLbl->setString(szVal);
    sprintf(szVal, "%d", toInt(m_heroVals[index].moveSpeed));
    m_moveSpeedLbl->setString(szVal);

    m_name->setString(m_heroVals[index].name);
    m_desc->setString(m_heroVals[index].desc);
}

void CCHeroRoomSceneLayer::onClickButtonDone( CCNode* pNode )
{
    M_DEF_GC(gc);
    ((CCButtonNormal*)pNode)->setEnabled(false);
    m_eff->play(1, 1.0f, 1, this, callfuncN_selector(CCHeroRoomSceneLayer::onAct5Done));
    gc->playSound("sounds/Effect/LevelUp.mp3");
}

void CCHeroRoomSceneLayer::onHideDone( CCNode* pNode )
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, CCBattleSceneLayer::scene(m_heroVals[getSelectIndex()])));
}

void CCHeroRoomSceneLayer::onScaleDone( CCNode* pNode )
{
    m_eff->playForever();
}

void CCHeroRoomSceneLayer::onAct5Done( CCNode* pNode )
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GC(gc);
    gc->playSound("sounds/Effect/UIMove.mp3");
    m_panel->runAction(CCSequence::createWithTwoActions(CCEaseExponentialOut::create(CCMoveTo::create(0.8f, ccp(wsz.width * 0.5, wsz.height + m_panel->getContentSize().height * m_panel->getScaleY() * 0.5))), CCCallFuncN::create(this, callfuncN_selector(CCHeroRoomSceneLayer::onHideDone))));
    
    CCSprite* sp = CCSprite::create("UI/Loading.png");
    addChild(sp);
    sp->setScale(wsz.width / sp->getContentSize().width * 0.3f);
    sp->setPosition(ccp(wsz.width * 0.5, wsz.height * 0.5));
    gc->playSound("sounds/Effect/GUITransitionOpen.mp3");
}

