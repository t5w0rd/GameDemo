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
#include "ComponentForCC.h"
#include "HeroRoomScene.h"


class CHeroLevelUp : public CLevelUpdate
{
public:
    int funcExp(int lvl)
    {
        return (lvl == 0) ? 0 : (lvl * lvl * 9 + lvl * 3 + 8);
    }

    virtual void updateExpRange(CLevelExp* pLevel)
    {
        int lvl = pLevel->getLevel();
        pLevel->setBaseExp(funcExp(lvl - 1));
        pLevel->setMaxExp(funcExp(lvl));
    }

    virtual void onLevelChange(CLevelExp* pLevel, int iChanged)
    {
        static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
        static SimpleAudioEngine* ae = SimpleAudioEngine::sharedEngine();
        
        CUnit* u = DCAST(pLevel, CUnit*);
        CUnitDrawForCC* d = DCAST(u->getDraw(), CUnitDrawForCC*);
        d->cmdStop();
        
        u->setMaxHp(u->getMaxHp() * 1.2);

        CBattleWorld* w = DCAST(u->getWorld(), CBattleWorld*);
        d->setCastTarget(CCommandTarget());
        d->cmdCastSpell(w->getWarCryAct()->getId());
        u->addBuffAbility(new CChangeHpBuff("LevelUpHeal", "LevelUpHeal", 5.0f, false, 0.2f, CExtraCoeff(0.02f, 0.0f)));
        u->addBuffAbility(new CReflectBuff("Reflect", "Reflect", 5.0f));
        CAttackAct* atk = DCAST(u->getActiveAbility(u->getAttackAbilityId()), CAttackAct*);
        atk->getBaseAttack().setValue(atk->getBaseAttack().getValue() + 7.8);
        d->setBaseMoveSpeed(d->getBaseMoveSpeed() + 1);
    }
};

class CMyAI : public CBaseAI
{
public:
    CMyAI()
        : m_bFirstTick(true)
        , m_iBaseTag(CKeyGen::nextKey() * 10)
        , m_iCurTag(m_iBaseTag)
    {
    }

    virtual void onUnitTick(CUnit* pUnit, float dt)
    {
        CBaseAI::onUnitTick(pUnit, dt);
        return;

        CUnitDraw2D* d = DCAST(pUnit->getDraw(), CUnitDraw2D*);
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
const float CBattleWorld::CONST_MAX_REWARD_RANGE = 400;

CBattleWorld::CBattleWorld()
{
    lua_State* L = CWorld::getLuaHandle();
}

CBattleWorld::~CBattleWorld()
{
}

bool CBattleWorld::onInit()
{
    static CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    static CCPoint org = CCDirector::sharedDirector()->getVisibleOrigin();

    CUnit* u = NULL;
    CUnitDrawForCC* d = NULL;
    CAbility* a = NULL;
    int id = 0;
    CAttackAct* atk = NULL;

    M_DEF_GC(gc);
    gc->loadTexture("Global0");
    gc->loadTexture("Global1");
    gc->loadTexture("Heroes0");
    gc->loadTexture("Heroes1");
    gc->loadTexture("Heroes2");
    gc->loadTexture("Heroes3");
    gc->loadTexture("Heroes4");
    gc->loadTexture("Heroes5");
    gc->loadTexture("Projectiles0");
    gc->loadTexture("Battle0");

    gc->loadAnimation("Effects/Lightning", "Effects/Lightning", 0.05f);
    gc->loadAnimation("Effects/Lightning2", "Effects/Lightning2", 0.05f);
    gc->loadAnimation("Effects/Lightning3", "Effects/Lightning3", 0.05f);
    gc->loadAnimation("Effects/Stun/Big", "Effects/Stun/Big", 0.1f);
    gc->loadAnimation("Effects/Stun/Small", "Effects/Stun/Small", 0.1f);

    m_oULib.init();

    // init hero
    //u = m_oULib.copyUnit(CUnitLibraryForCC::kThor);
    u = m_oULib.copyUnit(m_heroInfo.id == CUnitLibraryForCC::kBarracks ? CUnitLibraryForCC::kThor : m_heroInfo.id);
    addUnit(u);
    setControlUnit(u->getId());
    setHero(u);
    u->setMaxLevel(20);
    u->setLevelUpdate(new CHeroLevelUp);
    u->updateExpRange();
    u->setRevivable();
    u->setForceByIndex(2);
    CForceResource* fr = new CForceResource(this, (FUNC_CALLFUNC_N)(&CBattleWorld::onChangeGold)); // 势力资源
    u->setResource(fr);
    u->getActiveAbility(u->getAttackAbilityId())->dcast(atk);
    atk->setExAttackValue(CExtraCoeff(1.3f, 0.0f));

    m_pRebirthPas = new CRebirthPas("Rebirth", "Rebirth", 120.0f);
    u->addPassiveAbility(m_pRebirthPas);

    a = new CSpeedBuff("SlowDownBuff", "SlowDown", 8.0f, false, CExtraCoeff(-0.8f, 0.0f), CExtraCoeff(-0.8f, 0.0f));
    id = addTemplateAbility(a);
    a = new CKnockBackBuff("KnockBackBuff", "KnockBackBuff", 0.5, true, 40);
    DCAST(a, CBuffAbility*)->setAppendBuff(id);
    id = addTemplateAbility(a);
    a = new CDamageBuff("", CAttackValue(CAttackValue::kMagical, 100.0f), 1.0f);
    DCAST(a, CBuffAbility*)->setAppendBuff(id);
    id = addTemplateAbility(a);
    m_pThunderCapAct = new CBuffMakerAct("", "ThunderCap", 5.0f, CCommandTarget::kNoTarget, CUnitForce::kEnemy, 1.0f, id);
    m_pThunderCapAct->setCastTargetRadius(150.0f);
    m_pThunderCapAct->addCastAnimation(CUnitDraw::kAniAct2);
    m_pThunderCapAct->addEffectSound("sounds/Effect/ThunderCap.mp3");
    u->addActiveAbility(m_pThunderCapAct);

    
#if 1
    a = new CDamageBuff("dmg", CAttackValue(CAttackValue::kMagical, 100.0f), 1.0f);
    id = addTemplateAbility(a);
    
    a = new CStunBuff("Stun", "Stun", 5.0f, false);
    DCAST(a, CBuffAbility*)->setAppendBuff(id);
    id = addTemplateAbility(a);
#else
    a = new CChangeHpBuff("ChainHealBuff", "WarCryHeal", 1.0f, false, 0.02f, CExtraCoeff(0.02f, 0.0f));
    id = addTemplateAbility(a);
#endif
    a = new CTransitiveLinkBuff("Chain", 0.3f, 150.0f, 4, CUnitForce::kEnemy);
    DCAST(a, CBuffAbility*)->setAppendBuff(id);
    DCAST(a, CTransitiveLinkBuff*)->setTemplateProjectile(CUnitLibraryForCC::kLightning);
    id = addTemplateAbility(a);
    
    m_pHammerThrowAct = new CBuffMakerAct("", "HammerThrow", 12.0f, CCommandTarget::kUnitTarget, CUnitForce::kEnemy, 1.0f, id);
    m_pHammerThrowAct->setCastRange(400.0f);
    m_pHammerThrowAct->addCastAnimation(CUnitDraw::kAniAct3);
    m_pHammerThrowAct->setTemplateProjectile(CUnitLibraryForCC::kThorHammer);
    m_pHammerThrowAct->addEffectSound("sounds/Effect/LightningLink.mp3");
    u->addActiveAbility(m_pHammerThrowAct);

    a = new CSpeedBuff("WarCrySpeedBuff", "WarCrySpeed", 10.0f, false, CExtraCoeff(2.0f, 0.0f), CExtraCoeff(2.0f, 0.0f));
    id = addTemplateAbility(a);
    a = new CChangeHpBuff("WarCryHealBuff", "WarCryHeal", 10.0f, false, 0.2f, CExtraCoeff(0.02f, 0.0f));
    DCAST(a, CBuffAbility*)->setAppendBuff(id);
    id = addTemplateAbility(a);
    m_pWarCryAct = new CBuffMakerAct("", "WarCry", 0.0f, CCommandTarget::kNoTarget, CUnitForce::kAlly, 1.0f, id);
    m_pWarCryAct->setCastTargetRadius(200.0f);
    m_pWarCryAct->addCastAnimation(CUnitDraw::kAniAct5);
    m_pWarCryAct->addEffectSound("sounds/Effect/LevelUp.mp3");
    u->addActiveAbility(m_pWarCryAct);

    m_pDragonStrikeBuff = new CStunBuff("DragonStrikeBuff", "DragonStrikeBuff", 5.0f, false);
    addTemplateAbility(m_pDragonStrikeBuff);

    a = new CChangeHpPas("AutoHeal", "AutoHeal", 0.2f, CExtraCoeff(0.001f, 0.0f));
    id = addTemplateAbility(a);
    u->addPassiveAbility(id);

    d = DCAST(u->getDraw(), CUnitDrawForCC*);
    d->setPosition(CPoint(800, 600));
    
    u->setName(m_heroInfo.name);
    u->setMaxHp(m_heroInfo.hp);
    atk->setBaseAttack(m_heroInfo.atkVal);
    atk->setBaseAttackInterval(1 / m_heroInfo.attackSpeed);
    u->setBaseArmor(m_heroInfo.armVal);
    d->setBaseMoveSpeed(m_heroInfo.moveSpeed);

    onLuaWorldInit();
    
    CCBattleSceneLayer* l = DCAST(getLayer(), CCBattleSceneLayer*);
    l->initTargetInfo();
    l->updateTargetInfo(getControlUnit());

    l->initResourceInfo();
    l->updateResourceInfo(fr->getGold());

    l->initHeroPortrait();
    l->updateHeroPortrait();

    //runAction(CCSequence::createWithTwoActions(CCDelayTime::create(5.0f), CCSpawn::createWithTwoActions(CCMoveBy::create(2.0f, ccp(500, 500)), CCScaleTo::create(2.0f, 4.0f))));
    //l->setPosition(ccp());

    // DemoTemp
    static SimpleAudioEngine* ae = SimpleAudioEngine::sharedEngine();
    ae->preloadEffect("sounds/Units/Thor/move/00.mp3");
    ae->preloadEffect("sounds/Units/Thor/move/01.mp3");
    ae->preloadEffect("sounds/Units/Thor/move/02.mp3");
    ae->preloadEffect("sounds/Units/Thor/move/03.mp3");
    ae->preloadEffect("sounds/Units/Thor/die/00.mp3");
    ae->preloadEffect("sounds/Effect/WaveIncoming.mp3");
    ae->preloadEffect("sounds/Effect/Fighting.mp3");
    ae->preloadEffect("sounds/Effect/HammerThrow.mp3");
    ae->preloadEffect("sounds/Effect/LevelUp.mp3");
    ae->preloadEffect("sounds/Effect/OpenDoor.mp3");
    ae->preloadEffect("sounds/Effect/TeslaRay00.mp3");
    ae->preloadEffect("sounds/Effect/TeslaRay01.mp3");
    ae->preloadEffect("sounds/Effect/ThunderCap.mp3");
    ae->preloadEffect("sounds/Effect/ArcaneRay.mp3");
    ae->preloadEffect("sounds/Effect/UIMove.mp3");
    ae->preloadEffect("sounds/Effect/ArrowRelease01.mp3");
    ae->preloadEffect("sounds/Effect/ArrowRelease02.mp3");
    ae->preloadEffect("sounds/Effect/MageShot.mp3");
    ae->preloadEffect("sounds/Effect/Monk00.mp3");
    ae->preloadEffect("sounds/Effect/Monk01.mp3");
    ae->preloadEffect("sounds/Effect/Monk02.mp3");
    ae->preloadEffect("sounds/Effect/Hero_Monk_Firedragon.mp3");
    ae->preloadEffect("sounds/Effect/Sound_GUIButtonCommon.mp3");
    ae->preloadEffect("sounds/Effect/GUITransitionOpen.mp3");    
    
    char sz[1024];
    sprintf(sz, "sounds/Background/Background%02d.mp3", rand() % 5);
    ae->playBackgroundMusic(sz, true);
    ae->playEffect("sounds/Effect/WaveIncoming.mp3");

    return true;
}

// DemoTemp
float MAX_MOVE[] = {1.5f, 2.5f, 2.5f, 1.5f};
int g_moveIndex = 0;
float g_move = 100.0f;
float MAX_FIGHT = 8.0f;
float g_fight = 100.0f;
int g_fightId = 0;
float g_fightFree = 0.0f;

void CBattleWorld::onTick( float dt )
{
    // DemoTemp
    static SimpleAudioEngine* ae = SimpleAudioEngine::sharedEngine();
    g_move += dt;
    g_fight += dt;
    g_fightFree += dt;
    if (g_fightId != 0 && g_fightFree > 2.0f)
    {
        ae->stopEffect(g_fightId);
        g_fightId = 0;
        g_fight = 100.0f;
    }
    
    onLuaWorldTick(dt);

    CCBattleSceneLayer* l = DCAST(getLayer(), CCBattleSceneLayer*);
    l->updateTargetInfo();
    l->updateHeroPortrait();
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

    lua_getglobal(L, "setSearchPath");
    lua_call(L, 0, 0);

    int res = 0;

    lua_getglobal(L, "include");
    lua_pushstring(L, "world.lua");
    res = lua_pcall(L, 1, 0, 0);
    if (res != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        CCLOG("ERR | LuaErr: %s", err);
        lua_pop(L, 1);
        layer->log("%s", err);

        return false;
    }
/*

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    string comm = "/sdcard/ts/gamedemo/common.lua";
    string name = "/sdcard/ts/gamedemo/world.lua";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    string comm = "/var/mobile/Documents/common.lua";
    string name = "/var/mobile/Documents/world.lua";
#else
    string comm = "scripts/common.lua";
    string name = "scripts/world.lua";
#endif
    CCLOG("MSG | loadScript: %s", comm.c_str());
    CCLOG("MSG | loadScript: %s", name.c_str());

    string err;

    if (luaL_loadscript4cc(L, comm.c_str(), err) == false)
    {
        CCLOG("ERR | LuaErr: %s", err.c_str());
        layer->log("%s", err.c_str());

        return false;
    }

    if (luaL_loadscript4cc(L, name.c_str(), err) == false)
    {
        CCLOG("ERR | LuaErr: %s", err.c_str());
        layer->log("%s", err.c_str());

        return false;
    }*/

    lua_getglobal(L, "onWorldInit");
    res = lua_pcall(L, 0, 0, 0);
    if (res != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        CCLOG("ERR | LuaErr: %s", err);
        lua_pop(L, 1);
        layer->log("%s", err);

        return false;
    }

    assert(lua_gettop(L) == 0);

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

void CBattleWorld::onUnitDying(CUnit* pUnit)
{
    static SimpleAudioEngine* ae = SimpleAudioEngine::sharedEngine();

    CUnitDrawForCC* d = DCAST(pUnit->getDraw(), CUnitDrawForCC*);
    if (pUnit == getHero())
    {
    }
    else if (!d->isFixed())
    {
        char sz[256];
        sprintf(sz, "sounds/Effect/HumenDie%02d.mp3", rand() % 4);
        ae->playEffect(sz);
    }
    else
    {
    }

    if (pUnit != getHero() && pUnit->getRewardExp() && pUnit->isEnemyOf(getHero()) && M_RAND_HIT(1.0f))
    {
        // 掉落物品
    }

    // TODO: 金钱经验掉落
    //CWorld* w = pUnit->getWorld();
    CWorld::MAP_UNITS& units = getUnits();
    vector<CUnit*> vec;
    
    M_MAP_FOREACH(units)
    {
        CUnit* uu = M_MAP_EACH;
        CUnitDraw2D* dd = DCAST(uu->getDraw(), CUnitDraw2D*);
        if (!uu->isDead() && uu->canIncreaseExp() && uu->isEnemyOf(pUnit) && dd->getPosition().getDistance(d->getPosition()) < CBattleWorld::CONST_MAX_REWARD_RANGE)
        {
            vec.push_back(uu);
        }
        M_MAP_NEXT;
    }

    int n = vec.size();
    if (n != 0)
    {
        int iG = toInt(randValue(pUnit->getRewardGold(), 0.2f) / n);
        int iE = pUnit->getRewardExp() / n;
        CForceResource* pRes;
        M_VEC_FOREACH(vec)
        {
            CUnit* uu = M_VEC_EACH;
            uu->addExp(iE);
            pRes = uu->getResource();
            if (pRes)
            {
                pRes->changeGold(+iG);
                CUnitDrawForCC* dd = DCAST(uu->getDraw(), CUnitDrawForCC*);
                char szTip[64];
                sprintf(szTip, "+%d Gold", iG);
                dd->addBattleTip(szTip, "fonts/Comic Book.fnt", 18, ccc3(255, 247, 53));
            }
            M_VEC_NEXT;
        }
    }

    if (pUnit == getHero())
    {
        //setHero(NULL);
    }
}

void CBattleWorld::onUnitAttackTarget( CUnit* pUnit, CAttackData* pAttack, CUnit* pTarget )
{
    // DemoTemp
    static SimpleAudioEngine* ae = SimpleAudioEngine::sharedEngine();
    if (strcmp(pUnit->getDraw()->getName(), "Soldier") == 0 || strcmp(pUnit->getDraw()->getName(), "Templar") == 0)
    {
        if (g_fight > MAX_FIGHT)
        {
            g_fightId = ae->playEffect("sounds/Effect/Fighting.mp3");
            g_fight = 0.0f;
        }
        g_fightFree = 0.0f;
    }

    CUnitDraw2D* d = DCAST(pUnit->getDraw(), CUnitDraw2D*);
    CUnitDraw2D* hd = DCAST(getHero()->getDraw(), CUnitDraw2D*);
    if (pTarget == getHero() && hd->getCastActionId() == 0)
    {
        float dis = hd->getPosition().getDistance(d->getPosition());
        if (dis < 150 && !getThunderCapAct()->isCoolingDown() && pTarget->isDoingOr(CUnit::kMoving) == false)
        {
            hd->setCastTarget(CCommandTarget());
            hd->cmdCastSpell(getThunderCapAct()->getId());
            //ae->playEffect("sounds/Effect/ThunderCap.mp3");
            getLayer()->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.4f), CCShake::create(0.2f, 4, 10.0f)));
        }
        else if (dis >= 150 && !getHammerThrowAct()->isCoolingDown() && !pTarget->isDoingOr(CUnit::kObstinate))
        {
            hd->setCastTarget(pUnit->getId());
            hd->cmdCastSpell(getHammerThrowAct()->getId());
        }
        //DCAST(getHero()->getWorld(), CBattleWorld*)->getLayer()->runAction(CCShake::create(0.8f, 2, 50.0f));
    }
}

void CBattleWorld::onUnitProjectileEffect( CUnit* pUnit, CProjectile* pProjectile, CUnit* pTarget )
{
    // DemoTemp
    M_DEF_GC(gc);
    if (strcmp(pProjectile->getName(), "ThorHammer") == 0)
    {
        CCNode* sn = DCAST(pTarget->getDraw(), CUnitDrawForCC*)->getSprite()->getShadow();
#if 1
        CCAnimation* pAni = gc->getAnimation("Effects/Lightning");
        CCSprite* sp = CCSprite::createWithSpriteFrameName("Effects/Lightning/00.png");

        CCAnimation* pAni2 = gc->getAnimation("Effects/Lightning2");
        CCSprite* sp2 = CCSprite::createWithSpriteFrameName("Effects/Lightning2/00.png");

        CCAnimation* pAni3 = gc->getAnimation("Effects/Lightning3");
        CCSprite* sp3 = CCSprite::createWithSpriteFrameName("Effects/Lightning3/00.png");

        sn->getParent()->runAction(CCShake::create(0.2f, 4, 10.0f));
        
        sn->addChild(sp, M_BASE_Z - sn->getPosition().y);
        sp->setPosition(ccp(sn->getContentSize().width * sn->getAnchorPoint().x, sp->getContentSize().height * 0.5 - 100.0f));
        sp->runAction(CCSequence::create(CCAnimate::create(pAni), CCRemoveSelf::create(), NULL));

        sn->addChild(sp2, M_BASE_Z - sn->getPosition().y);
        sp2->setPosition(ccp(sn->getContentSize().width * sn->getAnchorPoint().x, sp2->getContentSize().height * 0.5 - 100.0f));
        sp2->runAction(CCSequence::create(CCAnimate::create(pAni2), CCRemoveSelf::create(), NULL));
        
        sn->addChild(sp3, M_BASE_Z - sn->getPosition().y);
        sp3->setPosition(ccp(sn->getContentSize().width * sn->getAnchorPoint().x, sp3->getContentSize().height * 0.5 - 100.0f));
        sp3->runAction(CCSequence::create(CCAnimate::create(pAni3), CCRemoveSelf::create(), NULL));
#else
        CCEffect* eff = CCEffect::create("Units/KungFu/act8", 0.1f);
        sn->getParent()->addChild(eff);
        eff->setPosition(sn->getParent()->convertToNodeSpace(sn->convertToWorldSpaceAR(sn->getAnchorPointInPoints())));
        eff->playRelease();
        
        
#endif
    }
}


void CBattleWorld::onChangeGold( CMultiRefObject* obj )
{
    CCBattleSceneLayer* l = DCAST(getLayer(), CCBattleSceneLayer*);
    CUnit* hero = getHero();
    if (hero != NULL)
    {
        l->updateResourceInfo(hero->getResource()->getGold());
    }
}

void CBattleWorld::onAniDone( CMultiRefObject* obj, void* data )
{
    getHero()->resume();
}




// CCBattleScene
CCBattleScene::CCBattleScene()
    : m_pWorld(NULL)
{
}

CCBattleScene::~CCBattleScene()
{
    M_SAFE_RELEASE(m_pWorld);
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
    , m_pTargetAtk(NULL)
    , m_pTargetDef(NULL)
    , m_bShowTargetInfo(false)
{
    CCLabelTTF* lbl = CCLabelTTF::create("TestSize", FONT_ARIAL, 20);
    const CCSize& sz = lbl->getContentSize();
    static CCSize winSz = CCDirector::sharedDirector()->getVisibleSize();
    m_iMaxLogs = (winSz.height - 20) / sz.height;
    memset(&m_stTargetInfo, 0xFF, sizeof(m_stTargetInfo));
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

CCScene* CCBattleSceneLayer::scene(CCHeroRoomSceneLayer::HERO_INFO& heroInfo)
{
    // 'scene' is an autorelease object
    CCBattleScene* pScene = CCBattleScene::create();

    CBattleWorld* pWorld = DCAST(pScene->getWorld(), CBattleWorld*);
    // 'layer' is an autorelease object
    CCBattleSceneLayer* layer = CCBattleSceneLayer::create();
    pWorld->setLayer(layer);

    // add layer as a child to scene
    pScene->addChild(layer);
    pScene->addChild(layer->getCtrlLayer(), 1);

    pWorld->m_heroInfo = heroInfo;
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

    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();

    m_pCtrlLayer = CCLayer::create();
    m_pCtrlLayer->retain();

    M_DEF_GC(gc);
    gc->getfc()->addSpriteFramesWithFile("Global0.plist");

    char sz[1024];
    sprintf(sz, "backgrounds/BackgroundHD%02d.png", rand() % 2);
    setBackgroundSprite(CCSprite::create(sz));
    setBufferEffectParam(1.5f, 0.9f, 20.0f, 0.1f);
    setPosition(ccp(0, 0));

    /////////////////////////////
    // 3. add your codes below...

    setWorldInterval(0.02f);
    
    CCMenu* mn = CCMenu::create();
    m_pCtrlLayer->addChild(mn);
    mn->setPosition(CCPointZero);

    CCButtonPanel* bp = CCButtonPanel::create(1, 1, 132, 132, 0, 0, NULL);
    CCButtonBase* btn = CCButtonNormal::createWithFrameName("UI/Button/Fist/Normal.png", "UI/Button/Fist/On.png", "UI/Button/Fist/Disabled.png", "UI/Button/Fist/Blink.png", "UI/Button/Fist/Mask.png", 90.0f, this, menu_selector(CCBattleSceneLayer::onClickFist), NULL);
    bp->addButton(btn, 0, 0);

    m_pCtrlLayer->addChild(bp);
    bp->setPosition(ccp(wsz.width - btn->getContentSize().width * 0.5 - 50, btn->getContentSize().height * 0.5 + 150));

    
    CCMenuItemLabel* lbl = CCMenuItemLabel::create(CCLabelTTF::create("RESTART", FONT_COMIC_BOOK, 64), this, menu_selector(CCBattleSceneLayer::onClickRestart));
    mn->addChild(lbl);

    lbl->setPosition(ccp(wsz.width - lbl->getContentSize().width * 0.5 - 50, wsz.height - lbl->getContentSize().height * 0.5 - 50));


    return true;
}

void CCBattleSceneLayer::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
    CCSetIterator it = pTouches->begin();
    CCTouch* pTouch = ((CCTouch*)(*it));

    static SimpleAudioEngine* ae = SimpleAudioEngine::sharedEngine();

    CCWinLayer::ccTouchesEnded(pTouches, pEvent);
    if (!isClickAction())
    {
        return;
    }

    CCPoint pos = convertTouchToNodeSpace(pTouch);

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

    if (d != NULL && t != NULL && hero->isEnemyOf(t))
    {
        if (hero != t)
        {
            d->setCastTarget(CCommandTarget(t->getId()));
            d->cmdCastSpell(hero->getAttackAbilityId());

            // DemoTemp
            if (!w->getHammerThrowAct()->isCoolingDown() && d->getCastActionId() == 0)
            {
                d->setCastTarget(CCommandTarget(t->getId()));
                d->cmdCastSpell(w->getHammerThrowAct()->getId());
            }

            // DemoTemp
            if (!hero->isDead() && g_move > MAX_MOVE[g_moveIndex])
            {
                int me = rand() % 4;
                while (me == g_moveIndex) me = rand() % 4;
                char sz[256];
                sprintf(sz, "sounds/Units/Thor/move/%02d.mp3", me);
                ae->playEffect(sz);
                g_moveIndex = me;
                g_move = 0.0f;
            }
        }
    }
    else if (d != NULL)
    {
        CCSprite* sp = CCSprite::createWithSpriteFrameName("UI/cmd/Target.png");
        addChild(sp);
        sp->setPosition(ccp(p.x, p.y));
        sp->runAction(CCSequence::create(CCFadeOut::create(0.5f), CCRemoveSelf::create(true), NULL));
        
        sp = CCSprite::createWithSpriteFrameName("UI/cmd/Target2.png");
        addChild(sp);
        sp->setPosition(ccp(p.x, p.y));
        sp->runAction(CCSequence::create(CCSpawn::create(CCScaleTo::create(0.5f, 1.5f, 1.5f), CCFadeOut::create(0.5f), NULL), CCRemoveSelf::create(true), NULL));

        d->cmdMove(p);
        // DemoTemp
        SimpleAudioEngine* ae = SimpleAudioEngine::sharedEngine();

        if (!hero->isDead() && g_move > MAX_MOVE[g_moveIndex])
        {
            int me = rand() % 4;
            while (me == g_moveIndex) me = rand() % 4;
            char sz[256];
            sprintf(sz, "sounds/Units/Thor/move/%02d.mp3", me);
            ae->playEffect(sz);
            g_moveIndex = me;
            g_move = 0.0f;
        }
    }

    if (t != NULL)
    {
        showTargetInfo(true);
        updateTargetInfo(t->getId());
    }
}

void CCBattleSceneLayer::log( const char* fmt, ... )
{
    static CCSize winSz = CCDirector::sharedDirector()->getVisibleSize();
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

    lbl = CCLabelTTF::create(sz, FONT_ARIAL, 20);
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

void CCBattleSceneLayer::initTargetInfo()
{
    static CCSize vSz = CCDirector::sharedDirector()->getVisibleSize();

    // 初始化目标信息面板
    CCSprite* pSprite = NULL;
    CCLabelTTF* pLabel = NULL;
    
    m_pTargetInfoPanel = CCSprite::create("UI/TargetInfoPanel.png");
    m_pCtrlLayer->addChild(m_pTargetInfoPanel);
    m_pTargetInfoPanel->setPosition(ccp(vSz.width * 0.5, -m_pTargetInfoPanel->getContentSize().height * 0.5));

    float fFontSize = 28;
    float fBaseY = 46;

    // 选择头像
    pSprite = CCSprite::createWithSpriteFrameName("UI/status/portrait_sel.png");
    m_pTargetInfoPanel->addChild(pSprite, 10);
    pSprite->setPosition(ccp(56, fBaseY - 2));

    m_pPortraitSel = CCSprite::createWithSpriteFrameName("UI/status/portrait_sel.png");
    m_pTargetInfoPanel->addChild(m_pPortraitSel);
    m_pPortraitSel->setPosition(pSprite->getPosition());

    // 名字
    float fW0 = 0;
    float fW1 = 100;
    float fBaseX = M_FIX_BASE_X(fW0, fW1, 80);
    m_pNameSel = CCLabelTTF::create("", FONT_COMIC_BOOK, fFontSize);
    m_pNameSel->setAnchorPoint(ccp(0.0, 0.5));
    m_pTargetInfoPanel->addChild(m_pNameSel);
    m_pNameSel->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_pNameSel->setPosition(ccp(fBaseX, fBaseY));

    // 等级
    fW0 = fW1;
    fW1 = 52;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 130);
    pLabel = CCLabelTTF::create("Lv", FONT_COMIC_BOOK, fFontSize, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_pTargetInfoPanel->addChild(pLabel);
    pLabel->setPosition(ccp(fBaseX, fBaseY));

    fW0 = fW1;
    fW1 = 32;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_pTargetLv = CCLabelTTF::create("12", FONT_COMIC_BOOK, fFontSize, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_pTargetInfoPanel->addChild(m_pTargetLv);
    m_pTargetLv->setPosition(ccp(fBaseX, fBaseY));

    // hp
    fW0 = fW1;
    fW1 = 52;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 84);
    pSprite = CCSprite::createWithSpriteFrameName("UI/status/HP.png");
    m_pTargetInfoPanel->addChild(pSprite);
    pSprite->setPosition(ccp(fBaseX, fBaseY));

    fW0 = fW1;
    fW1 = 200;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_pTargetHp = CCLabelTTF::create("1320/3208", FONT_COMIC_BOOK, fFontSize, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_pTargetInfoPanel->addChild(m_pTargetHp);
    m_pTargetHp->setPosition(ccp(fBaseX, fBaseY));

    // 攻击
    fW0 = fW1;
    fW1 = 52;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 64);
    m_pTargetAtkIcon = CCSprite::createWithSpriteFrameName("UI/status/PhysicalAttack.png");
    m_pTargetInfoPanel->addChild(m_pTargetAtkIcon);
    m_pTargetAtkIcon->setPosition(ccp(fBaseX, fBaseY));


    fW0 = fW1;
    fW1 = 0; // 锚点在最左处
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_pTargetAtk = CCLabelTTF::create("", FONT_COMIC_BOOK, fFontSize);
    m_pTargetAtk->setAnchorPoint(ccp(0.0, 0.5));
    m_pTargetInfoPanel->addChild(m_pTargetAtk);
    m_pTargetAtk->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_pTargetAtk->setString("105 - 110");
    m_pTargetAtk->setPosition(ccp(fBaseX, fBaseY));
    m_pTargetAtk->setString("150 - 110");

    fBaseX += m_pTargetAtk->getTextureRect().size.width;
    m_pTargetAtkEx = CCLabelTTF::create("", FONT_COMIC_BOOK, fFontSize);
    m_pTargetAtkEx->setAnchorPoint(ccp(0.0, 0.5));
    m_pTargetInfoPanel->addChild(m_pTargetAtkEx);
    m_pTargetAtkEx->setHorizontalAlignment(kCCTextAlignmentLeft);
    m_pTargetAtkEx->setString(" +15000");
    m_pTargetAtkEx->setPosition(ccp(fBaseX, fBaseY));
    m_pTargetAtk->setString("151 - 167");
    m_pTargetAtkEx->setColor(ccc3(40, 220, 40));

    // 护甲
    fW1 = 52;
    fBaseX += 260 - m_pTargetAtk->getTextureRect().size.width + fW1 * 0.5;
    m_pTargetDefIcon = CCSprite::createWithSpriteFrameName("UI/status/HeavyArmor.png");
    m_pTargetInfoPanel->addChild(m_pTargetDefIcon);
    m_pTargetDefIcon->setPosition(ccp(fBaseX, fBaseY));

    fW0 = fW1;
    fW1 = 64;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_pTargetDef = CCLabelTTF::create("32", FONT_COMIC_BOOK, fFontSize, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_pTargetInfoPanel->addChild(m_pTargetDef);
    m_pTargetDef->setPosition(ccp(fBaseX, fBaseY));
}

void CCBattleSceneLayer::updateTargetInfo(int id)
{
    if (m_bShowTargetInfo == false)
    {
        return;
    }

    static int lastId = 0;
    if (lastId != id && id != 0)
    {
        lastId = id;
    }

    CUnit* pUnit = getWorld()->getUnit(lastId);
    if (!pUnit)
    {
        return;
    }

    M_DEF_GC(gc);
    CCSpriteFrameCache* fc = gc->getfc();
    CUnitDraw2D* d = DCAST(pUnit->getDraw(), CUnitDraw2D*);
    
    if (!m_pTargetInfoPanel->isVisible())
    {
        m_pTargetInfoPanel->setVisible(true);
    }

    char szBuf[1024];

    // 选择头像
    sprintf(szBuf, "Units/%s/portrait_sel.png", pUnit->getDraw()->getName());
    CCSpriteFrame* sf = fc->spriteFrameByName(szBuf);
    if (sf == NULL)
    {
        sf = fc->spriteFrameByName("UI/status/portrait_sel.png");
    }
    m_pPortraitSel->setDisplayFrame(sf);
    m_pNameSel->setString(pUnit->getName());
    
    // 等级
    uint32_t dwLevel = pUnit->getLevel();
    if (dwLevel != m_stTargetInfo.dwLevel)
    {
        sprintf(szBuf, "%u", pUnit->getLevel());
        m_pTargetLv->setString(szBuf);
        m_stTargetInfo.dwLevel = dwLevel;
    }

    // hp
    uint32_t dwHp = toInt(pUnit->getHp());
    uint32_t dwMaxHp = toInt(pUnit->getRealMaxHp());
    if ((dwHp != m_stTargetInfo.dwHp) || (dwMaxHp != m_stTargetInfo.dwMaxHp))
    {
        sprintf(szBuf, "%u/%u", dwHp, dwMaxHp);
        m_pTargetHp->setString(szBuf);
        m_stTargetInfo.dwHp = dwHp;
        m_stTargetInfo.dwMaxHp = dwMaxHp;
    }

    // 攻击
    CAttackAct* atkAct = DCAST(pUnit->getActiveAbility(pUnit->getAttackAbilityId()), CAttackAct*);
    if (atkAct != NULL)
    {
        switch (atkAct->getBaseAttack().getType())
        {
        case CAttackValue::kPhysical:
            m_pTargetAtkIcon->setDisplayFrame(fc->spriteFrameByName("UI/status/PhysicalAttack.png"));
            break;

        case CAttackValue::kMagical:
            m_pTargetAtkIcon->setDisplayFrame(fc->spriteFrameByName("UI/status/MagicalAttack.png"));
            break;
        }
        uint32_t dwAtk0 = 0;
        uint32_t dwAtk1 = 0;
        float fAtk = 0;
        float fAtkRnd = 0;
        uint32_t dwAtkEx = 0;

        fAtk = atkAct->getBaseAttackValue();
        fAtkRnd = atkAct->getAttackValueRandomRange() * 0.5;
        dwAtk0 = toInt(fAtk * (1 - fAtkRnd));
        dwAtk1 = toInt(fAtk * (1 + fAtkRnd));
        dwAtkEx = toInt(fAtk * (atkAct->getExAttackValue().getMulriple() - 1.0) + atkAct->getExAttackValue().getAddend());

        if ((dwAtk0 != m_stTargetInfo.dwAtk0) || (dwAtk1 != m_stTargetInfo.dwAtk1) || (dwAtkEx != m_stTargetInfo.dwAtkEx))
        {
            sprintf(szBuf, "%u - %u", dwAtk0, dwAtk1);
            m_pTargetAtk->setString(szBuf);
            m_stTargetInfo.dwAtk0 = dwAtk0;
            m_stTargetInfo.dwAtk1 = dwAtk1;

            if (dwAtkEx)
            {
                sprintf(szBuf, " +%u", dwAtkEx);
                m_pTargetAtkEx->setString(szBuf);
                m_pTargetAtkEx->setPosition(ccpAdd(m_pTargetAtk->getPosition(), ccp(m_pTargetAtk->getTextureRect().size.width, 0)));
            }
            else
            {
                m_pTargetAtkEx->setString("");
            }
            m_stTargetInfo.dwAtkEx = dwAtkEx;
        }
    }
    
    // 护甲
    switch (pUnit->getBaseArmor().getType())
    {
    case CArmorValue::kHeavy:
        m_pTargetDefIcon->setDisplayFrame(fc->spriteFrameByName("UI/status/HeavyArmor.png"));
        break;

    case CArmorValue::kCrystal:
        m_pTargetDefIcon->setDisplayFrame(fc->spriteFrameByName("UI/status/CrystalArmor.png"));
        break;
    }
    int iDef = toInt(pUnit->getRealArmorValue());
    if (iDef != m_stTargetInfo.iDef)
    {
        sprintf(szBuf, "%d", iDef);
        m_pTargetDef->setString(szBuf);
        m_stTargetInfo.iDef = iDef;
    }

    if (pUnit->isDead())
    {
        showTargetInfo(false);
    }
}

void CCBattleSceneLayer::showTargetInfo( bool bShow /*= true*/ )
{
    if (bShow == m_bShowTargetInfo)
    {
        return;
    }

    m_pTargetInfoPanel->stopAllActions();
    const CCPoint& from = m_pTargetInfoPanel->getPosition();
    CCPoint to(from.x, 0.0f);
    if (bShow)
    {
        to.y = m_pTargetInfoPanel->getContentSize().height * 0.5;
    }
    else
    {
        to.y = -m_pTargetInfoPanel->getContentSize().height * 0.5;
    }

    // DemoTemp
    static SimpleAudioEngine* ae = SimpleAudioEngine::sharedEngine();
    ae->playEffect("sounds/Effect/UIMove.mp3");
    m_pTargetInfoPanel->runAction(CCMoveTo::create(to.getDistance(from) * 0.002, to));

    m_bShowTargetInfo = bShow;
}

void CCBattleSceneLayer::initHeroPortrait()
{
    static CCPoint org = CCDirector::sharedDirector()->getVisibleOrigin();
    static CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    
    CBattleWorld* w = DCAST(getWorld(), CBattleWorld*);
    CUnit* hero = w->getHero();

    M_DEF_GC(gc);

    CCSpriteFrameCache* fc = gc->getfc();
    char sz[1024];
    sprintf(sz, "Units/%s/portrait_hero.png", hero->getDraw()->getName());
    CCSpriteFrame* fr = fc->spriteFrameByName(sz);
    
    //m_pHeroPortrait = CCSprite::createWithSpriteFrame(fr);
    //m_pCtrlLayer->addChild(m_pHeroPortrait);
    
    const CCPoint& pos = m_pCtrlLayer->getChildByTag(1000)->getPosition();
    CCButtonPanel* bp = CCButtonPanel::create(1, 1, 112, 124, 0, 0, NULL);
    m_pCtrlLayer->addChild(bp);
    CCButtonNormal* btn = CCButtonNormal::createWithFrameName(sz, sz, NULL, NULL, NULL, 0.0f, this, menu_selector(CCBattleSceneLayer::onClickHeroPortrait), NULL);
    bp->setPosition(ccp(oSz.width * 0.07, pos.y - btn->getContentSize().height * 0.5 - 100));
    bp->addButton(btn, 0, 0);
    

    //m_pHeroPortrait->setPosition(ccp(oSz.width * 0.07, pos.y - m_pHeroPortrait->getContentSize().height * 0.5 - 100));
    

    CCSprite* fill = CCSprite::createWithSpriteFrameName("UI/status/HpBarFill.png");
    CCSprite* border = CCSprite::createWithSpriteFrameName("UI/status/HpBarBorder.png");
    m_pHeroHpBar = CCProgressBar::create(CCSizeMake(104, 8), fill, border, 0, 0, true);
    btn->addChild(m_pHeroHpBar);
    //m_pHeroPortrait->addChild(m_pHeroHpBar);
    //m_pHeroHpBar->setPosition(ccp(m_pHeroPortrait->getContentSize().width * 0.5, 16));
    m_pHeroHpBar->setPosition(ccp(btn->getContentSize().width * 0.5, 16));
    
    fill = CCSprite::createWithSpriteFrameName("UI/status/ExpBarFill.png");
    border = CCSprite::createWithSpriteFrameName("UI/status/ExpBarBorder.png");
    m_pHeroExpBar = CCProgressBar::create(CCSizeMake(104, 8), fill, border, 0, 0, true);
    //m_pHeroPortrait->addChild(m_pHeroExpBar);
    //m_pHeroExpBar->setPosition(ccp(m_pHeroPortrait->getContentSize().width * 0.5, 6));
    btn->addChild(m_pHeroExpBar);
    m_pHeroExpBar->setPosition(ccp(btn->getContentSize().width * 0.5, 6));

    m_pHeroLevel = CCLabelTTF::create("1", FONT_ARIAL, 24, CCSizeMake(28, 28), kCCTextAlignmentCenter);
    //m_pHeroPortrait->addChild(m_pHeroLevel);
    btn->addChild(m_pHeroLevel);
    m_pHeroLevel->setPosition(ccp(94, 40));
    m_stHeroInfo.iLevel = 1;
}

void CCBattleSceneLayer::updateHeroPortrait()
{
    CBattleWorld* w = DCAST(getWorld(), CBattleWorld*);
    CUnit* hero = w->getHero();

    float fHpPer = hero->getHp() * 100 / hero->getRealMaxHp();
    if (fHpPer != m_stHeroInfo.fHpPer)
    {
        m_stHeroInfo.fHpPer = fHpPer;
        m_pHeroHpBar->setPercentage(fHpPer);
    }

    float fExpPer = (hero->getExp() - hero->getBaseExp()) * 100 / (hero->getMaxExp() - hero->getBaseExp());
    if (fExpPer != m_stHeroInfo.fExpPer)
    {
        m_stHeroInfo.fExpPer = fExpPer;
        m_pHeroExpBar->setPercentage(fExpPer);
    }

    if (hero->getLevel() != m_stHeroInfo.iLevel)
    {
        m_stHeroInfo.iLevel = hero->getLevel();
        char sz[64];
        sprintf(sz, "%d", hero->getLevel());
        m_pHeroLevel->setString(sz);
    }
}

void CCBattleSceneLayer::initResourceInfo()
{
    static CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    CCSprite* sp = CCSprite::create("UI/WavePanel.png");
    m_pCtrlLayer->addChild(sp);
    sp->setTag(1000);
    const CCSize& spSz = sp->getContentSize();
    sp->setPosition(ccp(spSz.width * 0.5 + 50, oSz.height - spSz.height * 0.5 - 50));
    m_pGold = CCLabelTTF::create("      ", FONT_COMIC_BOOK, 28, CCSizeMake(100, 48), kCCTextAlignmentLeft);
    sp->addChild(m_pGold);
    m_pGold->setPosition(ccp(310, 85));
}

void CCBattleSceneLayer::updateResourceInfo( int gold )
{
    char sz[64];
    sprintf(sz, "%d", gold);
    m_pGold->setString(sz);
}

void CCBattleSceneLayer::onDragonStrikeUpdate( CCNode* pNode )
{
    CBattleWorld* w = DCAST(getWorld(), CBattleWorld*);
    CUnit* u = w->getHero();
    if (u == NULL)
    {
        return;
    }

    M_DEF_GC(gc);
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    CCEffect* eff = NULL;
    CCEffect* eff2 = NULL;
    CCLayerColor* lc = NULL;
    CCPoint ctrlPos;
    CCSprite* sp = NULL;
    
    CUnitDrawForCC* d = DCAST(u->getDraw(), CUnitDrawForCC*);
    CCNode* sn = d->getSprite()->getShadow();

    char sz[1024];

    switch (m_dscur)
    {
    case 0:  // 慢动作特写
        sprintf(sz, "sounds/Effect/Monk%02d.mp3", rand() % 3);
        gc->playSound(sz);
        eff = (CCEffect*)pNode;
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(0.2f);
        eff->runAction(
            CCSequence::create(
                CCMoveTo::create(eff->getPosition().getDistance(sn->getPosition()) / 1000, ccp(sn->getPosition().x, eff->getPosition().y)),
                CCCallFuncN::create(this, callfuncN_selector(CCBattleSceneLayer::onDragonStrikeUpdate)),
                NULL));

        ctrlPos = eff->getPosition();
        lc = CCLayerColor::create(ccc4(0, 0, 0, 128));
        lc->setContentSize(CCSizeMake(wsz.width * 10, wsz.height));
        addChild(lc);
        lc->setTag(7771);
        lc->setPosition(ccp(0.0f, wsz.height));
        lc->setOpacity(0);
        lc->runAction(CCEaseExponentialOut::create(CCSpawn::createWithTwoActions(CCMoveTo::create(0.15f, ccp(0.0f, ctrlPos.y - 0)), CCFadeTo::create(0.15f, 200))));

        lc = CCLayerColor::create(ccc4(0, 0, 0, 128));
        lc->setContentSize(CCSizeMake(wsz.width * 10, wsz.height));
        addChild(lc);
        lc->setTag(7772);
        lc->setPosition(ccp(0.0f, -wsz.height));
        lc->setOpacity(0);
        lc->runAction(CCEaseExponentialOut::create(CCSpawn::createWithTwoActions(CCMoveTo::create(0.15f, ccp(0.0f, ctrlPos.y - 200 - wsz.height)), CCFadeTo::create(0.15f, 200))));
        break;

    case 1:  // Dra
        eff = (CCEffect*)pNode;
        eff->playRelease(1);
        sp = CCSprite::create("UI/Dra.png");
        addChild(sp, 10000);
        sp->setPosition(ccpAdd(eff->getPosition(), ccp(200 - eff->getContentSize().width, 0.0f)));
        sp->setOpacity(0);
        sp->setScale(8.0f);
        sp->runAction(
            CCSequence::create(
                CCEaseExponentialIn::create(
                    CCSpawn::create(
                        CCFadeIn::create(0.08f),
                        CCScaleTo::create(0.08f, 0.5f),
                        NULL)),
                CCCallFuncN::create(this, callfuncN_selector(CCBattleSceneLayer::onDragonStrikeUpdate)),
                CCFadeOut::create(0.5f),
                CCRemoveSelf::create(),
                NULL));
        
        break;

    case 2:  // Shake and Gon
        eff = (CCEffect*)getChildByTag(77777);
        M_MAP_FOREACH(w->getUnits())
        {
            CUnit* t = M_MAP_EACH;
            CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);
            if (t != NULL && CUnitGroup::isLivingEnemyOf(t, w->getHero()) && td->getPosition().getDistance(CPoint(eff->getPositionX(), eff->getPositionY())) < 300.0f)
            {
                CAttackData* ad = new CAttackData;
                ad->setAttackValue(CAttackValue::kHoly, 100 + rand() % (50 * w->getHero()->getLevel()));
                t->damaged(ad, w->getHero(), CUnit::kMaskAll);
            }
            M_MAP_NEXT;
        }
        runAction(CCShake::create(0.05f, 4, 30.0f));
        gc->playSound("sounds/Effect/ThunderCap.mp3");
        eff2 = CCEffect::create("Effects/Wave", 0.02f);
        eff->addChild(eff2);
        eff2->setPosition(ccpAdd(eff->getAnchorPointInPoints(), ccp(0.0f, -133.0f)));
        eff2->playRelease(0, 2.0f);

        sp = CCSprite::create("UI/Gon.png");
        addChild(sp, 10000);
        sp->setPosition(ccpAdd(eff->getPosition(), ccp(-50.0f, 0.0f)));
        sp->setOpacity(0);
        sp->setScale(8.0f);
        sp->runAction(
            CCSequence::create(
                CCEaseExponentialIn::create(
                    CCSpawn::create(
                        CCFadeIn::create(0.08f),
                        CCScaleTo::create(0.08f, 0.5f),
                        NULL)),
                CCCallFuncN::create(this, callfuncN_selector(CCBattleSceneLayer::onDragonStrikeUpdate)),
                CCFadeOut::create(0.5f),
                CCRemoveSelf::create(),
                NULL));
        break;

    case 3:  // Shake
        eff = (CCEffect*)getChildByTag(77777);
        M_MAP_FOREACH(w->getUnits())
        {
            CUnit* t = M_MAP_EACH;
            CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);
            if (t != NULL && CUnitGroup::isLivingEnemyOf(t, w->getHero()) && td->getPosition().getDistance(CPoint(eff->getPositionX(), eff->getPositionY())) < 300.0f)
            {
                CAttackData* ad = new CAttackData;
                ad->setAttackValue(CAttackValue::kHoly, 100 + rand() % (100 * w->getHero()->getLevel()));
                t->damaged(ad, w->getHero(), CUnit::kMaskAll);
            }
            M_MAP_NEXT;
        }
        runAction(CCSequence::createWithTwoActions(CCShake::create(0.05f, 4, 30.0f), CCCallFuncN::create(this, callfuncN_selector(CCBattleSceneLayer::onDragonStrikeUpdate))));
        gc->playSound("sounds/Effect/ThunderCap.mp3");
        eff2 = CCEffect::create("Effects/Wave", 0.02f);
        eff->addChild(eff2);
        eff2->setPosition(ccpAdd(eff->getAnchorPointInPoints(), ccp(0.0f, -133.0f)));
        eff2->playRelease(0, 2.0f);
        break;

    case 4:  // Strike
        eff = (CCEffect*)getChildByTag(77777);
        sp = CCSprite::create("UI/Strike.png");
        addChild(sp, 10000);
        sp->setPosition(ccpAdd(eff->getPosition(), ccp(eff->getContentSize().width * 0.5 - 50, 0.0f)));
        sp->setOpacity(0);
        sp->setScale(8.0f);
        sp->runAction(
            CCSequence::create(
                CCEaseExponentialIn::create(
                CCSpawn::create(
                    CCFadeIn::create(0.2f),
                    CCScaleTo::create(0.2f, 0.5f),
                    NULL)),
                CCCallFuncN::create(this, callfuncN_selector(CCBattleSceneLayer::onDragonStrikeUpdate)),
                CCFadeOut::create(0.5f),
                CCRemoveSelf::create(),
                NULL));
        gc->playSound("sounds/Effect/Hero_Monk_Firedragon.mp3");
        break;

    case 5:  // Shake
        eff = (CCEffect*)getChildByTag(77777);
        M_MAP_FOREACH(w->getUnits())
        {
            CUnit* t = M_MAP_EACH;
            CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);
            if (t != NULL && CUnitGroup::isLivingEnemyOf(t, w->getHero()) && td->getPosition().getDistance(CPoint(eff->getPositionX(), eff->getPositionY())) < 300.0f)
            {
                CAttackData* ad = new CAttackData;
                ad->setAttackValue(CAttackValue::kHoly, 100 + rand() % (150 * w->getHero()->getLevel()));
                ad->addAttackBuff(CAttackBuff(w->getDragonStrikeBuff()->getId(), 1));
                t->damaged(ad, w->getHero(), CUnit::kMaskAll);
            }
            M_MAP_NEXT;
        }
        runAction(CCShake::create(0.1f, 8, 60.0f));
        gc->playSound("sounds/Effect/ThunderCap.mp3");
        eff->runAction(
            CCSequence::create(
                CCDelayTime::create(0.05f),
                CCCallFuncN::create(this, callfuncN_selector(CCBattleSceneLayer::onDragonStrikeUpdate)),
                NULL));

        getChildByTag(7771)->runAction(CCSequence::createWithTwoActions(CCEaseExponentialOut::create(CCSpawn::createWithTwoActions(CCMoveTo::create(0.5f, ccp(0.0f, wsz.height)), CCFadeTo::create(0.5f, 0))), CCRemoveSelf::create()));
        getChildByTag(7772)->runAction(CCSequence::createWithTwoActions(CCEaseExponentialOut::create(CCSpawn::createWithTwoActions(CCMoveTo::create(0.5f, ccp(0.0f, -wsz.height)), CCFadeTo::create(0.5f, 0))), CCRemoveSelf::create()));
        break;

    case 6:
        setMoveEnabled(true);
        eff = CCEffect::create("Effects/Wave", 0.1f);
        getChildByTag(77777)->addChild(eff);
        eff->setPosition(ccpAdd(getChildByTag(77777)->getAnchorPointInPoints(), ccp(0.0f, -133.0f)));
        eff->playRelease();
        CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);
        break;
    }
    ++m_dscur;
}

void CCBattleSceneLayer::onClickFist( CCObject* pNode )
{
    M_DEF_GC(gc);
    gc->playSound("sounds/Effect/Sound_GUIButtonCommon.mp3");
    // DragonStrike
    DCAST(pNode, CCButtonNormal*)->coolDown();
    CBattleWorld* w = DCAST(getWorld(), CBattleWorld*);
    CUnit* u = w->getHero();
    CUnitDrawForCC* d = DCAST(u->getDraw(), CUnitDrawForCC*);
    setMoveEnabled(false);
    CCNode* sp = d->getSprite();
    CCEffect* eff = CCEffect::create("Units/KungFu/move", 0.03f);
    eff->addAnimation("Units/KungFu/act8", 0.05f);
    addChild(eff, M_BASE_Z - d->getPosition().y, 77777);
    eff->setPosition(ccp(getContentSize().width * getScale() + 100, d->getPosition().y + 150.0f));
    eff->playForever();
    eff->setFlipX(true);
    m_dscur = 0;
    eff->runAction(
        CCSequence::create(
        CCMoveTo::create(eff->getPosition().getDistance(sp->getPosition()) * 0.9 / 1000, ccpAdd(ccp((sp->getPositionX() * 0.9 + eff->getPositionX() * 0.1), sp->getPositionY()), ccp(0.0f, 133.0f))),
        CCCallFuncN::create(this, callfuncN_selector(CCBattleSceneLayer::onDragonStrikeUpdate)),
        NULL));
}

void CCBattleSceneLayer::onClickHeroPortrait( CCObject* pNode )
{
    M_DEF_GC(gc);
    gc->playSound("sounds/Effect/Sound_GUIButtonCommon.mp3");

    CBattleWorld* w = DCAST(getWorld(), CBattleWorld*);
    if (w->getHero())
    {
        showTargetInfo();
        updateTargetInfo(w->getHero()->getId());
    }
}

void CCBattleSceneLayer::onClickRestart( CCObject* obj )
{
    getWorld()->shutdown();
    CCDirector::sharedDirector()->replaceScene(CCBattleSceneLayer::scene(DCAST(getWorld(), CBattleWorld*)->m_heroInfo));
}
