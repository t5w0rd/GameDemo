#include "CommHeader.h"

#include "Logic/Unit.h"
#include "Logic/Ability.h"
#include "BattleScene.h"
#include "GameControl.h"
#include "DrawForCC.h"
#include "AbilityForCC.h"
#include "ActionForCC.h"


class CMyAI : public CDefaultAI
{
public:
    CMyAI()
        : m_bFirstTick(true)
        , m_iBaseTag(CKeyGen::nextKey() * 10)
        , m_iCurTag(m_iBaseTag)
    {
    }

    virtual void onUnitTick(float dt)
    {
        CDefaultAI::onUnitTick(dt);
        return;

        CUnitDraw2D* d = DCAST(getNotifyUnit()->getDraw(), CUnitDraw2D*);
        if (isFirstTick())
        {
            setFirstTick(false);
            setDstPoint(d->getPosition());
            setOrgPoint(getDstPoint() + CPoint(300.0f, 300.0f));
            return;
        }

        if (getDstPoint().getDistance(d->getPosition()) < 10.0f)
        {
            CPoint tmp = getOrgPoint();
            setOrgPoint(getDstPoint());
            setDstPoint(tmp);
            d->cmdMove(getDstPoint());
        }
    }

    M_SYNTHESIZE_BOOL(FirstTick);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oOrgPoint, OrgPoint);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oDstPoint, DstPoint);
    M_SYNTHESIZE(int, m_iBaseTag, BaseTag);
    M_SYNTHESIZE(int, m_iCurTag, CurTag);
};

// CBattleWorld
CBattleWorld::CBattleWorld()
    : m_iHeroUnit(0)
{
}

CBattleWorld::~CBattleWorld()
{

}

void CBattleWorld::onInit()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint org = CCDirector::sharedDirector()->getVisibleOrigin();

    M_DEF_GC(gc);
    gc->loadTexture("Global");
    gc->loadAnimation("Units/Malik/move", "Units/Malik/move", 0.08f);
    gc->loadAnimation("Units/Malik/die", "Units/Malik/die", 0.18f);
    gc->loadAnimation("Units/Malik/act1", "Units/Malik/act1", 0.05f);
    gc->loadAnimation("Units/Malik/act2", "Units/Malik/act2", 0.05f);
    gc->loadAnimation("Units/Matchstick/move", "Units/Matchstick/move", 0.08f);
    gc->loadAnimation("Units/Matchstick/die", "Units/Matchstick/die", 0.08f);
    gc->loadAnimation("Units/Matchstick/act1", "Units/Matchstick/act1", 0.08f);
    gc->loadAnimation("Units/Matchstick/act2", "Units/Matchstick/act2", 0.08f);
    gc->loadAnimation("Units/Matchstick/act3", "Units/Matchstick/act3", 0.08f);
    gc->loadAnimation("Units/Matchstick/act4", "Units/Matchstick/act4", 0.08f);

    CUnit* u = NULL;
    CUnitDrawForCC* ud = NULL;

#if 0
    u = createUnit(kDemonHunter);
    u->getDraw()->dcast(ud);
    u->setForceByIndex(1);
    ud->setPosition(CPoint(vs.width * 1.2, vs.height * 1.2));

    u = createUnit(kMountainKing);
    m_iHeroUnit = u->getId();
    u->getDraw()->dcast(ud);
    u->setForceByIndex(2);
    ud->setPosition(CPoint(vs.width * 1.5, vs.height * 1.2));

    return;
#endif

    CAbility* a = NULL;
    int id = 0;

    // ´´½¨Draw
    ud = new CUnitDrawForCC("Malik");
    ud->prepareFrame(CUnitDrawForCC::kFrmDefault, "default");
    ud->prepareAnimation(CUnitDrawForCC::kAniMove, "move", -1);
    ud->prepareAnimation(CUnitDrawForCC::kAniDie, "die", -1);
    ud->prepareAnimation(CUnitDrawForCC::kAniAct1, "act1", 4);
    ud->prepareAnimation(CUnitDrawForCC::kAniAct2, "act2", 4);

    ud->setGeometry(7.0f, 10.0f, ccp(0.5f, 0.1125f), ccp(10.0f, 10.0f));

    // ´´½¨hero
    u = new CUnit("CUnit");
    m_iHeroUnit = u->getId();
    u->setDraw(ud);
    u->setName("Malik");
    u->setMaxHp(500.0f);
    u->setForceByIndex(1);
    u->setAI(CMyAI());

    addUnit(u);

    CStatusShowPas* hpb = new CStatusShowPas(
        );
    u->addPassiveAbility(hpb, false);

    CAttackAct* atk = new CAttackAct(
        "NormalAttack",
        "¹¥»÷",
        1.75,
        CAttackValue(1,
        CAttackValue::kPhysical,
        30.0),
        0.5);
    atk->setCastMinRange(-3.0f);
    atk->setCastRange(15.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    u->addActiveAbility(atk);

    a = new CSpeedBuff(
        "SlowDown",
        "Slow",
        5.0f,
        true,
        CExtraCoeff(-0.2f, 0.0f),
        CExtraCoeff(-0.2f, 0.0f));
    id = addTemplateAbility(a);

    a = new CAttackBuffMakerPas(
        "AttackBuffMaker.[SlowDown]",
        "ÖÂ²Ð",
        0.2f,
        id);
    id = addTemplateAbility(a);
    u->addPassiveAbility(id);

    a = new CSpeedBuff(
        "SpeedUp",
        "SpeedUp",
        10.0f,
        true,
        CExtraCoeff(0.2f, 0.0f),
        CExtraCoeff(0.2f, 0.0f));
    id = addTemplateAbility(a);

    a = new CAttackBuffMakerPas(
        "AttackBuffMaker.[SlowDown]",
        "ÐË·Ü",
        0.2f,
        id,
        true);
    id = addTemplateAbility(a);
    u->addPassiveAbility(id);

    a = new CEvadeBuff(
        "EvadeBuff",
        "Evade",
        5.0,
        true,
        0.3f);
    id = addTemplateAbility(a);
    u->addPassiveAbility(id);

    a = new CEvadePas(
        "EvadePas",
        "Evade",
        0.3f,
        id);
    id = addTemplateAbility(a);
    u->addPassiveAbility(id);

    a = new CStunBuff(
        "Stun2s",
        "Stun",
        2.0f,
        false);
    id = addTemplateAbility(a);

    a = new CAttackBuffMakerPas(
        "AttackBuffMaker.[Stun]",
        "ÖØ»÷",
        0.2f,
        id,
        false,
        CExtraCoeff(1.0f, 50.0f));
    id = addTemplateAbility(a);
    u->addPassiveAbility(id);

    a = new CVampirePas(
        "Vampire20%",
        "ÎüÑª",
        0.2f);
    id = addTemplateAbility(a);
    u->addPassiveAbility(id);

    u->addPassiveAbility(new CRebirthPas("Rebirth", "REBIRTH", 2.0f, CExtraCoeff(0.5, 0)));

    ud->setBaseMoveSpeed(80.0f);
    ud->setPosition(CPoint(vs.width * 1.4, vs.height * 1.2));
    //ud->setFlightHeight(30);

    // create other units
    ud = new CUnitDrawForCC("Matchstick");
    ud->prepareFrame(CUnitDrawForCC::kFrmDefault, "default");
    ud->prepareAnimation(CUnitDrawForCC::kAniMove, "move", -1);
    ud->prepareAnimation(CUnitDrawForCC::kAniDie, "die", -1);
    ud->prepareAnimation(CUnitDrawForCC::kAniAct1, "act1", 3);
    ud->prepareAnimation(CUnitDrawForCC::kAniAct2, "act2", 2);
    ud->setGeometry(24.0f, 27.0f, ccp(69.0 / 128, 6.0 / 128), ccp(41.0f, 29.0f));

    u = new CUnit("CUnit");
    u->setDraw(ud);
    u->setName("Matchstick");
    u->setMaxHp(500.0f);
    u->setForceByIndex(2);
    u->setAI(CMyAI());

    addUnit(u);

    hpb = new CStatusShowPas(
        );
    u->addPassiveAbility(hpb, false);

    atk = new CAttackAct(
        "NormalAttack",
        "¹¥»÷",
        2.00,
        CAttackValue(1,
        CAttackValue::kPhysical,
        50.0),
        0.5);
    atk->setCastMinRange(-3.0f);
    atk->setCastRange(15.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    u->addActiveAbility(atk);

    u->addPassiveAbility(new CRebirthPas("Rebirth", "REBIRTH", 2.0f, CExtraCoeff(0.5, 0)));
    
    a = new CDoubleAttackBuff("DoubleAttack", "¹¥»÷CDÖØÖÃ");
    id = addTemplateAbility(a);
    u->addPassiveAbility(new CAttackBuffMakerPas("abm.DoubleAttack", "Á¬»÷", 0.5f, id, true));

    ud->setBaseMoveSpeed(50.0f);
    ud->setPosition(CPoint(vs.width * 1.2, vs.height * 1.2));

}

CUnit* CBattleWorld::createUnit( UNIT_INDEX eId )
{
    CUnit* u = NULL;
    CUnitDrawForCC* ud = NULL;
    CAbility* a = NULL;
    int id = 0;
    CAttackAct* atk = NULL;

    switch (eId)
    {
    case kBladeMaster:
        ud = new CUnitDrawForCC("Matchstick");
        ud->prepareFrame(CUnitDrawForCC::kFrmDefault, "default");
        ud->prepareAnimation(CUnitDrawForCC::kAniMove, "move", -1);
        ud->prepareAnimation(CUnitDrawForCC::kAniDie, "die", -1);
        ud->prepareAnimation(CUnitDrawForCC::kAniAct2, "act2", 2);
        ud->prepareAnimation(CUnitDrawForCC::kAniAct4, "act4", 1);
        ud->setGeometry(24.0f, 27.0f, ccp(69.0 / 128, 6.0 / 128), ccp(41.0f, 29.0f));

        u = new CUnit("CUnit");
        m_iHeroUnit = u->getId();
        u->setDraw(ud);
        u->setAI(CMyAI());
        addUnit(u);

        u->setName("BladeMaster");
        u->setMaxHp(1000);
        u->setBaseArmorValue(9.0f);

        ud->setBaseMoveSpeed(80.0f);

        u->addPassiveAbility(new CStatusShowPas(), false);

        // 40~62 = 51~43.1%
        atk = new CAttackAct(
            "NormalAttack",
            "¹¥»÷",
            1.77f,
            CAttackValue(1,
            CAttackValue::kPhysical,
            51.0),
            0.431f);
        u->addActiveAbility(atk);
        atk->setCastMinRange(-3.0f);
        atk->setCastRange(15.0f);
        atk->setCastHorizontal();
        atk->addCastAnimation(CUnitDraw::kAniAct2);
        atk->addCastAnimation(CUnitDraw::kAniAct4);
        atk->setExAttackSpeed(CExtraCoeff(1.38f));

        // Critical 15% x4
        a = new CAttackBuffMakerPas(
            "Criticalx4",
            "Critical",
            0.15f,
            0,
            false,
            CExtraCoeff(4.0f));
        id = addTemplateAbility(a);
        u->addPassiveAbility(id);
        break;

    case kDemonHunter:
        ud = new CUnitDrawForCC("Matchstick");
        ud->prepareFrame(CUnitDrawForCC::kFrmDefault, "default");
        ud->prepareAnimation(CUnitDrawForCC::kAniMove, "move", -1);
        ud->prepareAnimation(CUnitDrawForCC::kAniDie, "die", -1);
        ud->prepareAnimation(CUnitDrawForCC::kAniAct1, "act1", 3);
        ud->prepareAnimation(CUnitDrawForCC::kAniAct2, "act2", 2);
        ud->setGeometry(24.0f, 27.0f, ccp(69.0 / 128, 6.0 / 128), ccp(41.0f, 29.0f));

        u = new CUnit("CUnit");
        m_iHeroUnit = u->getId();
        u->setDraw(ud);
        u->setAI(CMyAI());
        addUnit(u);

        u->setName("DemonHunter");
        u->setMaxHp(1100);
        u->setBaseArmorValue(9.0f);

        ud->setBaseMoveSpeed(80.0f);

        u->addPassiveAbility(new CStatusShowPas(), false);

        // 37~59 = 48~45.8%
        atk = new CAttackAct(
            "NormalAttack",
            "¹¥»÷",
            1.7f,
            CAttackValue(1,
            CAttackValue::kPhysical,
            48.0),
            0.458f);
        u->addActiveAbility(atk);
        atk->setCastMinRange(-3.0f);
        atk->setCastRange(15.0f);
        atk->setCastHorizontal();
        atk->addCastAnimation(CUnitDraw::kAniAct1);
        atk->addCastAnimation(CUnitDraw::kAniAct2);
        atk->setExAttackSpeed(CExtraCoeff(1.35f));

        // Critical 15% x4
        a = new CEvadePas(
            "Evade30%",
            "Evade",
            0.3f);
        id = addTemplateAbility(a);
        u->addPassiveAbility(id);
        break;

    case kMountainKing:
        ud = new CUnitDrawForCC("Malik");
        ud->prepareFrame(CUnitDrawForCC::kFrmDefault, "default");
        ud->prepareAnimation(CUnitDrawForCC::kAniMove, "move", -1);
        ud->prepareAnimation(CUnitDrawForCC::kAniDie, "die", -1);
        ud->prepareAnimation(CUnitDrawForCC::kAniAct1, "act1", 4);
        ud->prepareAnimation(CUnitDrawForCC::kAniAct2, "act2", 4);

        ud->setGeometry(7.0f, 10.0f, ccp(0.5f, 0.1125f), ccp(10.0f, 10.0f));
        
        u = new CUnit("CUnit");
        m_iHeroUnit = u->getId();
        u->setDraw(ud);
        u->setAI(CMyAI());
        addUnit(u);

        u->setName("MountainKing");
        u->setMaxHp(1375.0f);
        u->setBaseArmorValue(6.0f);

        ud->setBaseMoveSpeed(80.0f);

        u->addPassiveAbility(new CStatusShowPas(), false);

        // 53~63 = 58~17.2%
        atk = new CAttackAct(
            "NormalAttack",
            "¹¥»÷",
            2.22f,
            CAttackValue(1,
            CAttackValue::kPhysical,
            58.0),
            0.172f);
        u->addActiveAbility(atk);
        atk->setCastMinRange(-3.0f);
        atk->setCastRange(15.0f);
        atk->setCastHorizontal();
        atk->addCastAnimation(CUnitDraw::kAniAct1);
        atk->addCastAnimation(CUnitDraw::kAniAct2);
        atk->setExAttackSpeed(CExtraCoeff(1.24f));

        // Thump 40% +25 1s
        a = new CStunBuff(
            "Stun",
            "Stun",
            1.0f,
            true);
        id = addTemplateAbility(a);
        a = new CAttackBuffMakerPas(
            "ABM.Stun",
            "Thump",
            0.4f,
            id,
            false,
            CExtraCoeff(1.0f, 25.0f));
        id = addTemplateAbility(a);
        u->addPassiveAbility(id);

        break;

    }

    return u;
}


// CCBattleScene
CCBattleScene::CCBattleScene()
    : m_pWorld(NULL)
{
}

CCBattleScene::~CCBattleScene()
{
    if (m_pWorld != NULL)
    {
        m_pWorld->release();
    }
}

bool CCBattleScene::init()
{
    m_pWorld = new CBattleWorld;
    m_pWorld->retain();

    return CCScene::init();
}


// CCBattleSceneLayer
CCBattleSceneLayer::CCBattleSceneLayer()
{
}

CCBattleSceneLayer::~CCBattleSceneLayer()
{
}

CCScene* CCBattleSceneLayer::scene()
{
    // 'scene' is an autorelease object
    CCBattleScene* pScene = CCBattleScene::create();

    CUnitWorldForCC* pWorld = pScene->getWorld();
    // 'layer' is an autorelease object
    CCUnitLayer* layer = CCBattleSceneLayer::create();
    pWorld->setLayer(layer);

    // add layer as a child to scene
    pScene->addChild(layer);

    pWorld->init();

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool CCBattleSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    setBackGroundSprite(CCSprite::create("BackgroundHD.png"));
    setBufferEffectParam(0.9f, 10.0f, 0.1f);

    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint org = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);

    // position the label on the center of the screen
    pLabel->setPosition(ccp(org.x + vs.width/2,
        org.y + vs.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    setWorldInterval(0.02f);


    return true;
}

void CCBattleSceneLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    CCWinUnitLayer::ccTouchEnded(pTouch, pEvent);
    if (!isClickAction())
    {
        return;
    }

    CCPoint pos = ccpSub(pTouch->getLocation(), getPosition());

    CBattleWorld* w = DCAST(getWorld(), CBattleWorld*);
    CUnit* hero = w->getUnit(w->getHeroUnit());
    if (hero == NULL)
    {
        return;
    }

    CUnitDraw2D* d = DCAST(hero->getDraw(), CUnitDraw2D*);
    if (d == NULL)
    {
        return;
    }

    CPoint p(pos.x, pos.y);
    CUnit* t = CUnitGroup::getNearestUnitInRange(w, p, 50.0f);

    if (t != NULL && hero != t)
    {
        d->setCastTarget(CCommandTarget(t->getId()));
        d->cmdCastSpell(hero->getAttackAbilityId());

    }
    else
    {
        d->cmdMove(p);
    }
}

void CCBattleSceneLayer::onMovePreviousLabel( CCNode* pCurLble, void* PreLbl )
{

}


