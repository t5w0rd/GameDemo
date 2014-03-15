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
    CUnitDrawForCC* d = NULL;
    CAbility* a = NULL;
    int id = 0;
    CAttackAct* atk = NULL;

    M_DEF_GC(gc);
    gc->loadTexture("Global0");
    gc->loadTexture("Heroes0");
    gc->loadTexture("Heroes1");
    gc->loadTexture("Heroes2");
    gc->loadTexture("Heroes3");
    gc->loadTexture("Heroes4");
    gc->loadTexture("Heroes5");
    gc->loadTexture("Projectiles0");
    gc->loadTexture("Battle0");

    m_oULib.init();

    u = m_oULib.copyUnit(CUnitLibraryForCC::kThor);
    addUnit(u);
    setControlUnit(u->getId());
    u->setForceByIndex(2);
    d = DCAST(u->getDraw(), CUnitDrawForCC*);
    d->setPosition(CPoint(500, 1000));
    
    onLuaWorldInit();
    DCAST(getLayer(), CCBattleSceneLayer*)->updateTargetInfo(getControlUnit());

    return true;
}

void CBattleWorld::onTick( float dt )
{
    CCBattleSceneLayer* l = DCAST(getLayer(), CCBattleSceneLayer*);
    l->updateTargetInfo();
    onLuaWorldTick(dt);
}


bool CBattleWorld::onLuaWorldInit()
{
    // lua
    lua_State* L = getLuaHandle();
    CCBattleSceneLayer* layer = DCAST(getLayer(), CCBattleSceneLayer*);

    luaL_insertloader(L, luaModuleLoader4cc);
    luaRegCommFunc(L);
    luaRegWorldFuncs(L, this);
    luaRegWorldFuncsForCC(L, this);

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    string name = "/sdcard/ts/gamedemo/world.lua";
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

        return false;
    }

    assert(lua_gettop(L) == 0);

    DCAST(getLayer(), CCBattleSceneLayer*)->initTargetInfoLayer();

    return true;
}

void CBattleWorld::onLuaWorldTick( float dt )
{
    lua_State* L = getLuaHandle();
    CCBattleSceneLayer* layer = DCAST(getLayer(), CCBattleSceneLayer*);

    lua_getglobal(L, "onWorldTick");
    lua_pushnumber(L, dt);
    int res = lua_pcall(L, 1, 0, 0);
    if (res != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        CCLOG("ERR | LuaErr: %s", err);
        lua_pop(L, 1);
        layer->log("%s", err);

        return;
    }

    assert(lua_gettop(L) == 0);
}

CProjectile* CBattleWorld::copyProjectile( int id ) const
{
    return m_oULib.copyProjectile(id);
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

    M_DEF_GC(gc);
    gc->getfc()->addSpriteFramesWithFile("Global0.plist");

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
        //return;
    }

    CUnitDraw2D* d = hero ? DCAST(hero->getDraw(), CUnitDraw2D*) : NULL;
    if (d == NULL)
    {
        //return;
    }

    CPoint p(pos.x, pos.y);
    CUnit* t = CUnitGroup::getNearestUnitInRange(w, p, 50.0f);

    if (d != NULL && t != NULL && t->isEnemyOf(hero))
    {
        if (hero != t)
        {
            d->setCastTarget(CCommandTarget(t->getId()));
            d->cmdCastSpell(hero->getAttackAbilityId());
        }
    }
    else if (d != NULL)
    {
        d->cmdMove(p);
    }

    if (t != NULL)
    {
        updateTargetInfo(t->getId());
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

void CCBattleSceneLayer::initTargetInfoLayer()
{
    // 初始化目标信息面板
    m_oTargetInfoLayer.initWithColor(ccc4(140, 70, 35, 255));
    m_pCtrlLayer->addChild(&m_oTargetInfoLayer);
    m_oTargetInfoLayer.setContentSize(CCSizeMake(700, 48));
    m_oTargetInfoLayer.setPosition(ccp(5, 5));

    CCSprite* pSprite = NULL;
    pSprite = CCSprite::createWithSpriteFrameName("UI/pos.png");
    m_pCtrlLayer->addChild(pSprite);
    pSprite->setPosition(ccp(5, 5));

    // 等级
    float fW0 = 0;
    float fW1 = 32;
    float fBaseX = M_FIX_BASE_X(fW0, fW1, 10);
    pSprite = CCSprite::createWithSpriteFrameName("UI/ph32.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setPosition(ccp(fBaseX, 24));
    pSprite->setColor(ccc3(240, 160, 120));

    fW0 = fW1;
    fW1 = 64;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_oTargetLv.initWithString("12", "Hobo Std", 24, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_oTargetInfoLayer.addChild(&m_oTargetLv);
    m_oTargetLv.setPosition(ccp(fBaseX, 24));
    m_oTargetLv.setColor(ccc3(250, 190, 100));

    // hp
    fW0 = fW1;
    fW1 = 32;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 4);
    pSprite = CCSprite::createWithSpriteFrameName("UI/status/HP.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setPosition(ccp(fBaseX, 24));
    //pSprite->setColor(ccc3(100, 185, 75));

    fW0 = fW1;
    fW1 = 160;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_oTargetHp.initWithString("1320/3208", "Hobo Std", 24, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_oTargetInfoLayer.addChild(&m_oTargetHp);
    m_oTargetHp.setPosition(ccp(fBaseX, 24));
    m_oTargetHp.setColor(ccc3(250, 190, 100));

    // 攻击
    fW0 = fW1;
    fW1 = 32;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 4);
    m_pTargetAtk = CCSprite::createWithSpriteFrameName("UI/status/PhysicalAttack.png");
    m_oTargetInfoLayer.addChild(m_pTargetAtk);
    m_pTargetAtk->setPosition(ccp(fBaseX, 24));
    //pSprite->setColor(ccc3(230, 40, 30));

    /*
    pSprite = CCSprite::createWithSpriteFrameName("UI/pos.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setPosition(ccp(fBaseX, 24));
    */

    fW0 = fW1;
    fW1 = 0; // 锚点在最左处
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_oTargetAtk.initWithString("", "Hobo Std", 24);
    m_oTargetAtk.setAnchorPoint(ccp(0.0, 0.5));
    m_oTargetInfoLayer.addChild(&m_oTargetAtk);
    m_oTargetAtk.setHorizontalAlignment(kCCTextAlignmentLeft);
    m_oTargetAtk.setString("105 - 110");
    //m_oTargetAtk.setDimensions(CCSizeMake(m_oTargetAtk.getTextureRect().size.width, 32));
    m_oTargetAtk.setPosition(ccp(fBaseX, 24));
    m_oTargetAtk.setColor(ccc3(250, 190, 100));
    m_oTargetAtk.setString("150 - 110");

    /*
    pSprite = CCSprite::createWithSpriteFrameName("UI/pos.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setTag(100);
    pSprite->setPosition(ccp(fBaseX, 24));
    */

    fBaseX += m_oTargetAtk.getTextureRect().size.width;
    m_oTargetAtkEx.initWithString("", "Hobo Std", 24);
    m_oTargetAtkEx.setAnchorPoint(ccp(0.0, 0.5));
    m_oTargetInfoLayer.addChild(&m_oTargetAtkEx);
    m_oTargetAtkEx.setHorizontalAlignment(kCCTextAlignmentLeft);
    m_oTargetAtkEx.setString(" +15000");
    //m_oTargetAtkEx.setDimensions(CCSizeMake(m_oTargetAtkEx.getTextureRect().size.width, 32));
    m_oTargetAtkEx.setPosition(ccp(fBaseX, 24));
    m_oTargetAtk.setString("151 - 167");
    m_oTargetAtkEx.setColor(ccc3(40, 220, 40));

    /*
    pSprite = CCSprite::createWithSpriteFrameName("UI/pos.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setTag(101);
    pSprite->setPosition(ccp(fBaseX, 24));
    */
    // 护甲
    fW1 = 32;
    fBaseX += 240 - m_oTargetAtk.getTextureRect().size.width + fW1 * 0.5;
    m_pTargetDef = CCSprite::createWithSpriteFrameName("UI/status/HeavyArmor.png");
    m_oTargetInfoLayer.addChild(m_pTargetDef);
    m_pTargetDef->setPosition(ccp(fBaseX, 24));
    //pSprite->setColor(ccc3(0, 110, 155));

    fW0 = fW1;
    fW1 = 64;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_oTargetDef.initWithString("32", "Hobo Std", 24, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_oTargetInfoLayer.addChild(&m_oTargetDef);
    m_oTargetDef.setPosition(ccp(fBaseX, 24));
    m_oTargetDef.setColor(ccc3(250, 190, 100));

    CWorld* w = getWorld();
    updateTargetInfo(w->getControlUnit());
}

void CCBattleSceneLayer::updateTargetInfo(int id)
{
    static int lastId = 0;
    if (lastId != id && id != 0)
    {
        lastId = id;
    }

    CUnit* pUnit = getWorld()->getUnit(lastId);
    if (!pUnit || pUnit->isDead())
    {
        return;
    }

    M_DEF_GC(gc);
    CCSpriteFrameCache* fc = gc->getfc();
    CUnitDraw2D* d = DCAST(pUnit->getDraw(), CUnitDraw2D*);
    
    if (!m_oTargetInfoLayer.isVisible())
    {
        m_oTargetInfoLayer.setVisible(true);
    }

    //if (pUnit != m_pLastTarget)

    char szBuf[64];

    uint32_t dwLevel = pUnit->getLevel();
    if (dwLevel != m_stTargetInfo.dwLevel)
    {
        sprintf(szBuf, "%u", pUnit->getLevel());
        m_oTargetLv.setString(szBuf);
        m_stTargetInfo.dwLevel = dwLevel;
    }

    uint32_t dwHp = pUnit->getHp();
    uint32_t dwMaxHp = pUnit->getRealMaxHp();
    if ((dwHp != m_stTargetInfo.dwHp) || (dwMaxHp != m_stTargetInfo.dwMaxHp))
    {
        sprintf(szBuf, "%u/%u", dwHp, dwMaxHp);
        m_oTargetHp.setString(szBuf);
        m_stTargetInfo.dwHp = dwHp;
        m_stTargetInfo.dwMaxHp = dwMaxHp;
    }

    CAttackAct* atkAct = DCAST(pUnit->getActiveAbility(pUnit->getAttackAbilityId()), CAttackAct*);
    if (atkAct != NULL)
    {
        switch (atkAct->getBaseAttack().getType())
        {
        case CAttackValue::kPhysical:
            m_pTargetAtk->setDisplayFrame(fc->spriteFrameByName("UI/status/PhysicalAttack.png"));
            break;

        case CAttackValue::kMagical:
            m_pTargetAtk->setDisplayFrame(fc->spriteFrameByName("UI/status/MagicalAttack.png"));
            break;
        }
        uint32_t dwAtk0 = 0;
        uint32_t dwAtk1 = 0;
        float fAtk = 0;
        float fAtkRnd = 0;
        uint32_t dwAtkEx = 0;

        fAtk = atkAct->getBaseAttackValue();
        fAtkRnd = atkAct->getAttackValueRandomRange() * 0.5;
        dwAtk0 = fAtk * (1 - fAtkRnd);
        dwAtk1 = fAtk * (1 + fAtkRnd);
        dwAtkEx = fAtk * (atkAct->getExAttackValue().getMulriple() - 1.0) + atkAct->getExAttackValue().getAddend();

        if ((dwAtk0 != m_stTargetInfo.dwAtk0) || (dwAtk1 != m_stTargetInfo.dwAtk1) || (dwAtkEx != m_stTargetInfo.dwAtkEx))
        {
            sprintf(szBuf, "%u - %u", dwAtk0, dwAtk1);
            m_oTargetAtk.setString(szBuf);
            //m_oTargetAtkEx.setPosition();
            m_stTargetInfo.dwAtk0 = dwAtk0;
            m_stTargetInfo.dwAtk1 = dwAtk1;

            //m_oTargetInfoLayer.getChildByTag(100)->setPosition(m_oTargetAtk.getPosition());

            if (dwAtkEx)
            {
                sprintf(szBuf, " +%u", dwAtkEx);
                m_oTargetAtkEx.setString(szBuf);
                //m_oTargetAtkEx.setDimensions(CCSizeMake(m_oTargetAtkEx.getTextureRect().size.width, 32));
                m_oTargetAtkEx.setPosition(ccpAdd(m_oTargetAtk.getPosition(), ccp(m_oTargetAtk.getTextureRect().size.width, 0)));
                //m_oTargetInfoLayer.getChildByTag(101)->setPosition(m_oTargetAtkEx.getPosition());
            }
            else
            {
                m_oTargetAtkEx.setString("");
            }
            m_stTargetInfo.dwAtkEx = dwAtkEx;
        }
    }
    
    switch (pUnit->getBaseArmor().getType())
    {
    case CArmorValue::kNormal:
    case CArmorValue::kHeavy:
        m_pTargetDef->setDisplayFrame(fc->spriteFrameByName("UI/status/HeavyArmor.png"));
        break;

    case CArmorValue::kCrystal:
        m_pTargetDef->setDisplayFrame(fc->spriteFrameByName("UI/status/CrystalArmor.png"));
        break;
    }
    uint32_t dwDef = pUnit->getRealArmorValue();
    if (dwDef != m_stTargetInfo.dwDef)
    {
        sprintf(szBuf, "%u", dwDef);
        m_oTargetDef.setString(szBuf);
        m_stTargetInfo.dwDef = dwDef;
    }

}
