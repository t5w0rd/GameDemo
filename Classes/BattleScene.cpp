#include "CommHeader.h"

#include "Unit.h"
#include "Ability.h"
#include "BattleScene.h"
#include "GameControl.h"
#include "DrawForCC.h"
#include "AbilityForCC.h"
#include "ActionForCC.h"
#include "LuaBinding.h"
#include "LuaBindingForCC.h"


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
{
    lua_State* L = CWorld::getLuaHandle();
}

CBattleWorld::~CBattleWorld()
{

}

bool CBattleWorld::onInit()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint org = CCDirector::sharedDirector()->getVisibleOrigin();

    CUnit* u = NULL;
    CUnitDrawForCC* ud = NULL;
    CAbility* a = NULL;
    int id = 0;

    M_DEF_GC(gc);
    gc->loadTexture("Global");

    gc->loadAnimation("Units/Ball1/move", "Units/Ball1/move", 0.1f);
    gc->loadAnimation("Units/Ball1/die", "Units/Ball1/die", 0.1f);
    gc->loadAnimation("Units/Lightning1/die", "Units/Lightning1/die", 0.05f);

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

    // 30, 31, (0.5122, 0.1), (40, 30)
    gc->loadAnimation("Units/Alric/move", "Units/Alric/move", 0.08f);
    gc->loadAnimation("Units/Alric/die", "Units/Alric/die", 0.08f);
    gc->loadAnimation("Units/Alric/act1", "Units/Alric/act1", 0.08f);  // 4
    gc->loadAnimation("Units/Alric/act2", "Units/Alric/act2", 0.08f);  // 3
    gc->loadAnimation("Units/Alric/act3", "Units/Alric/act3", 0.08f);  // 8
    gc->loadAnimation("Units/Alric/act4", "Units/Alric/act4", 0.08f);  // 9
    gc->loadAnimation("Units/Alric/act5", "Units/Alric/act5", 0.08f);  // 3

    // 创建Draw
    ud = new CUnitDrawForCC("Malik");
    ud->prepareFrame(CUnitDraw::kFrmDefault, "default");
    ud->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    ud->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    ud->prepareAnimation(CUnitDraw::kAniAct1, "act1", 4);
    ud->prepareAnimation(CUnitDraw::kAniAct2, "act2", 4);
    ud->setGeometry(7.0f, 10.0f, ccp(0.5f, 0.1125f), CPoint(10.0f, 10.0f));

    // 创建hero
    u = new CUnit("CUnit");
    m_iControlUnit = u->getId();
    u->setDraw(ud);
    u->setName("Malik");
    u->setMaxHp(500.0f);
    u->setForceByIndex(1);
    u->setAI(CMyAI());

    addUnit(u);

    CStatusShowPas* hpb = new CStatusShowPas();
    u->addPassiveAbility(hpb, false);

    CAttackAct* atk = new CAttackAct(
        "NormalAttack",
        "攻击",
        1.75,
        CAttackValue(CAttackValue::kPhysical, 30.0),
        0.5);
    atk->setCastMinRange(-3.0f);
    atk->setCastRange(50.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);

    CProjectileForCC* p = NULL;
#if 0
    p = new CProjectileForCC("Ball1");
    p->setMoveSpeed(300.0f);
    p->setMaxHeightDelta(100.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    p->prepareAnimation(CProjectile::kAniMove, "move", -1);
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
#else
    p = new CProjectileForCC("Lightning1");
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireLink);
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
#endif
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    id = addTemplateProjectile(p);
    atk->setTemplateProjectile(id);

    u->addActiveAbility(atk);
#if 1
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
        "致残",
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
        "兴奋",
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
        "重击",
        0.2f,
        id,
        false,
        CExtraCoeff(1.0f, 50.0f));
    id = addTemplateAbility(a);
    u->addPassiveAbility(id);

    a = new CVampirePas(
        "Vampire20%",
        "吸血",
        0.2f);
    id = addTemplateAbility(a);
    u->addPassiveAbility(id);
#endif
    u->addPassiveAbility(new CRebirthPas("Rebirth", "REBIRTH", 10.0f, CExtraCoeff(0.5, 0)));

    ud->setBaseMoveSpeed(80.0f);
    ud->setPosition(CPoint(vs.width * 0.6, vs.height * 0.5));
    //ud->setHeight(30);

    // 主动技能测试
    a = new CStunBuff(
        "Stun2s",
        "Stun",
        2.0f,
        false);
    id = addTemplateAbility(a);

    a = new CDamageBuffMakerBuff(
        "锤子伤害",
        CAttackValue(CAttackValue::kMagical, 0.0f),
        1.0f,
        id,
        false,
        CExtraCoeff(2.0f, 0.0f));
    id = addTemplateAbility(a);

    CBuffMakerAct* bm = new CBuffMakerAct("", "锤子", 5.0f, CCommandTarget::kUnitTarget, CUnitForce::kEnemy, 1.0f, id);
    p = new CProjectileForCC("Ball1");
    p->setMoveSpeed(500.0f);
    p->setMaxHeightDelta(10.0f);
    p->setPenaltyFlags(CProjectile::kOnDying);
    p->setFireType(CProjectile::kFireFollow);
    p->prepareAnimation(CProjectile::kAniMove, "move", -1);
    p->prepareAnimation(CProjectile::kAniDie, "die", 0);
    p->prepareFrame(CProjectile::kFrmDefault, "default");
    id = addTemplateProjectile(p);
    bm->setTemplateProjectile(id);
    bm->addCastAnimation(CUnitDraw::kAniAct1);
    bm->setCastRange(200.0f);
    u->addActiveAbility(bm);

    CUnitDrawForCC* hero = ud;
    
    // create other units
    ud = new CUnitDrawForCC("Matchstick");
    ud->prepareFrame(CUnitDraw::kFrmDefault, "default");
    ud->prepareAnimation(CUnitDraw::kAniMove, "move", -1);
    ud->prepareAnimation(CUnitDraw::kAniDie, "die", -1);
    ud->prepareAnimation(CUnitDraw::kAniAct1, "act1", 3);
    ud->prepareAnimation(CUnitDraw::kAniAct2, "act2", 2);
    ud->setGeometry(24.0f, 27.0f, ccp(69.0 / 128, 6.0 / 128), CPoint(41.0f, 29.0f));

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
        "攻击",
        2.00,
        CAttackValue(CAttackValue::kPhysical, 50.0),
        0.5);
    atk->setCastMinRange(-3.0f);
    atk->setCastRange(15.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    u->addActiveAbility(atk);

    u->addPassiveAbility(new CRebirthPas("Rebirth", "REBIRTH", 10.0f, CExtraCoeff(0.5, 0)));
    
    u->addPassiveAbility(new CDoubleAttackPas("DoubleAttack", "连击", 0.5f));

    ud->setBaseMoveSpeed(50.0f);
    ud->setPosition(CPoint(vs.width * 0.4, vs.height * 0.5));

    onLuaWorldInit();

    hero->setCastTarget(CCommandTarget(u->getId()));
    hero->cmdCastSpell(bm->getId());

    return true;
}

bool CBattleWorld::onLuaWorldInit()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint org = CCDirector::sharedDirector()->getVisibleOrigin();

    // lua
    lua_State* L = getLuaHandle();
    CCBattleSceneLayer* layer = DCAST(getLayer(), CCBattleSceneLayer*);

    luaL_insertloader(L, luaModuleLoader4cc);
    luaRegCommFunc(L);
    luaRegWorldFuncs(L, this);
    luaRegWorldFuncsForCC(L, this);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    string name = "/sdcard/Android/data/com.ts.gamedemo/files/world.lua";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    string name = "/var/mobile/Documents/world.lua";
#else
    string name = "script/world.lua";
#endif
    CCLOG("MSG | loadScript: %s", name.c_str());

    string err;
    if (luaL_loadscript4cc(L, name.c_str(), err) == false)
    {
        CCLOG("ERR | LuaErr: %s", err.c_str());
        layer->log("%s", err.c_str());
//         CCLabelTTF* pLabel = CCLabelTTF::create(err.c_str(), "Arial", 20);
//         pLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
//         pLabel->setColor(ccc3(240, 60, 0));
//         DCAST(getLayer(), CCBattleSceneLayer*)->getCtrlLayer()->addChild(pLabel);
//         pLabel->setPosition(ccp(pLabel->getContentSize().width * 0.5 + 10, vs.height - pLabel->getContentSize().height * 0.5 + 10));

        return false;
    }

    lua_getglobal(L, "onWorldInit");
    int res = lua_pcall(L, 0, 0, 0);
    if (res != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        CCLOG("ERR | LuaErr: %s", err);
        lua_pop(L, 1);
        layer->log("%s", err);
//         CCLabelTTF* pLabel = CCLabelTTF::create(err, "Arial", 20);
//         pLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
//         pLabel->setColor(ccc3(240, 60, 0));
//         DCAST(getLayer(), CCBattleSceneLayer*)->getCtrlLayer()->addChild(pLabel);
//         pLabel->setPosition(ccp(pLabel->getContentSize().width * 0.5 + 10, vs.height - pLabel->getContentSize().height * 0.5 + 10));

        return false;
    }

    assert(lua_gettop(L) == 0);

    return true;
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
    : m_pCtrlLayer(NULL)
    , m_iMaxLogs(0)
    , m_iBaseLogId(CKeyGen::nextKey())
    , m_iCurLogId(m_iBaseLogId)
{
    CCLabelTTF* lbl = CCLabelTTF::create("TestSize", "Arial", 20);
    const CCSize& sz = lbl->getContentSize();
    CCSize winSz = CCDirector::sharedDirector()->getVisibleSize();
    m_iMaxLogs = (winSz.height - 20) / sz.height;
}

CCBattleSceneLayer::~CCBattleSceneLayer()
{
    if (m_pCtrlLayer != NULL)
    {
        if (m_pCtrlLayer->getParent() != NULL)
        {
            m_pCtrlLayer->removeFromParentAndCleanup(true);
        }
        
        m_pCtrlLayer->release();
    }
}

CCScene* CCBattleSceneLayer::scene()
{
    // 'scene' is an autorelease object
    CCBattleScene* pScene = CCBattleScene::create();

    CWorldForCC* pWorld = pScene->getWorld();
    // 'layer' is an autorelease object
    CCBattleSceneLayer* layer = CCBattleSceneLayer::create();
    pWorld->setLayer(layer);

    // add layer as a child to scene
    pScene->addChild(layer);
    pScene->addChild(layer->getCtrlLayer(), 1);

    if (pWorld->init() == false)
    {
        return NULL;
    }
    

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool CCBattleSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!CCLayer::init())
    {
        return false;
    }

    m_pCtrlLayer = CCLayer::create();
    m_pCtrlLayer->retain();

    setBackGroundSprite(CCSprite::create("BackgroundHD.png"));
    setBufferEffectParam(0.9f, 10.0f, 0.1f);
    setPosition(ccp(0, 0));

    /////////////////////////////
    // 3. add your codes below...

    setWorldInterval(0.02f);

    return true;
}

void CCBattleSceneLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCWinUnitLayer::ccTouchEnded(pTouch, pEvent);
    if (!isClickAction())
    {
        return;
    }

    CCPoint pos = ccpSub(pTouch->getLocation(), getPosition());

    CBattleWorld* w = DCAST(getWorld(), CBattleWorld*);
    CUnit* hero = w->getUnit(w->getControlUnit());
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

void CCBattleSceneLayer::log( const char* fmt, ... )
{
    CCSize winSz = CCDirector::sharedDirector()->getVisibleSize();
    char sz[1024];
    va_list argv;
    va_start(argv, fmt);
    vsprintf(sz, fmt, argv);
    va_end(argv);

    CCNode* l = getCtrlLayer();
    int curLogId = getCurLogId();
    cirInc(curLogId, getBaseLogId(), getMaxLogs());
    setCurLogId(curLogId);

    CCLabelTTF* lbl = DCAST(l->getChildByTag(getCurLogId()), CCLabelTTF*);
    if (lbl != NULL)
    {
        lbl->removeFromParentAndCleanup(true);
    }

    lbl = CCLabelTTF::create(sz, "Arial", 20);
    lbl->setHorizontalAlignment(kCCTextAlignmentLeft);
    lbl->setColor(ccc3(79, 0, 255));
    l->addChild(lbl, 1, getCurLogId());
    const CCSize& rSz = lbl->getContentSize();
    lbl->setPosition(ccp(rSz.width * 0.5 + 10, winSz.height + rSz.height * 0.5 - 10));

    for (int i = 0, j = getCurLogId(); i < getMaxLogs(); ++i)
    {
        CCNode* pNode = l->getChildByTag(j);
        if (pNode != NULL)
        {
            pNode->runAction(CCMoveBy::create(0.1f, ccp(0.0f, -lbl->getContentSize().height)));
        }

        cirDec(j, getBaseLogId(), getMaxLogs());
    }
}
