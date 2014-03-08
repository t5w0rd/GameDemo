/* 
 * File:   Ability.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午11:45
 */

#include "CommInc.h"
#include "Unit.h"
#include "Ability.h"
#include "MultiRefObject.h"
#include "Application.h"
#include "Draw.h"
#include "LuaBinding.h"


// CAbility
CAbility::CAbility(const char* pRootId, const char* pName, float fCoolDown)
: CONST_ROOT_ID(pRootId)
, m_iScriptHandler(0)
, m_sName(pName)
, m_pOwner(NULL)
, m_fCoolDown(fCoolDown)
, m_fCoolingDownElapsed(FLT_MAX)
, m_fInterval(0.0f)
, m_fIntervalElapsed(0.0f)
, m_dwTriggerFlags(0)
{
    setDbgClassName("CAbility");
}

CAbility::~CAbility()
{
    if (getScriptHandler() != 0)
    {
        lua_State* L = CWorld::getLuaHandle();
        luaL_unref(L, LUA_REGISTRYINDEX, getScriptHandler());
    }
}

const char* CAbility::getDbgTag() const
{
    return getName();
}

const char* CAbility::getRootId() const
{
    return CONST_ROOT_ID.c_str();
}

bool CAbility::isCoolingDown() const
{
    return m_fCoolingDownElapsed < getCoolDown();
}

void CAbility::resetCD()
{
    m_fCoolingDownElapsed = FLT_MAX;
    getOwner()->updateAbilityCD(getId());
}

void CAbility::coolDown()
{
    setCoolingDownElapsed(0.0f);
    getOwner()->abilityCD(this);
}

void CAbility::setInterval(float fInterval)
{
    if (fInterval <= FLT_EPSILON)
    {
        m_fInterval = 0.0f;
        return;
    }
    
    setTriggerFlags(CUnit::kOnTickTrigger);
    m_fInterval = fInterval;
}

void CAbility::fireProjectile(int iProjectile, const CCommandTarget& rTarget)
{
    assert(iProjectile != 0);

    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    CWorld* w = o->getWorld();
    CProjectile* p = NULL;
    w->copyProjectile(iProjectile)->dcast(p);
    w->addProjectile(p);
    p->setSrcUnit(o->getId());
    p->setSrcAbility(this);

    if (rTarget.getTargetType() == CCommandTarget::kUnitTarget)
    {
        CUnit* t = w->getUnit(rTarget.getTargetUnit());
        CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);
        assert(td != NULL);

        p->setFromToType(CProjectile::kUnitToUnit);
        p->setFromUnit(o->getId());
        p->setToUnit(t->getId());

        p->fire();
    }
}

void CAbility::onUnitAddAbility()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());
    
    lua_getfield(L, a, "onUnitAddAbility");
    lua_pushvalue(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDelAbility()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());
    
    lua_getfield(L, a, "onUnitDelAbility");
    lua_pushvalue(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitAbilityReady()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAbilityReady");
    lua_pushvalue(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitRevive()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitRevive");
    lua_pushvalue(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDying()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDying");
    lua_pushvalue(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDead()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDead");
    lua_pushvalue(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitChangeHp(float fChanged)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitChangeHp");
    lua_pushvalue(L, a);
    lua_pushnumber(L, fChanged);
    lua_call(L, 2, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitTick(float dt)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitTick");
    lua_pushvalue(L, a);
    lua_pushnumber(L, dt);
    lua_call(L, 2, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitInterval()
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitInterval");
    lua_pushvalue(L, a);
    lua_call(L, 1, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

CAttackData* CAbility::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return pAttack;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAttackTarget");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "AttackData", pAttack);
    luaL_pushobjptr(L, "Unit", pTarget);
    lua_call(L, 3, 1);
    
    if (lua_isnil(L, -1))
    {
        pAttack = NULL;
    }
    
    lua_pop(L, 2);  // pop 'a' and ret

    return pAttack;
}

CAttackData* CAbility::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    if (getScriptHandler() == 0)
    {
        return pAttack;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitAttacked");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "AttackData", pAttack);
    luaL_pushobjptr(L, "Unit", pSource);
    lua_call(L, 3, 1);

    if (lua_isnil(L, -1))
    {
        pAttack = NULL;
    }

    lua_pop(L, 2);  // pop 'a' and ret

    return pAttack;
}

void CAbility::onUnitDamaged(CAttackData* pAttack, CUnit* pSource)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDamaged");
    lua_pushvalue(L, a);
    luaL_pushobjptr(L, "AttackData", pAttack);
    luaL_pushobjptr(L, "Unit", pSource);
    lua_call(L, 3, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDamagedDone(float fDamage, CUnit* pSource)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDamagedDone");
    lua_pushvalue(L, a);
    lua_pushnumber(L, fDamage);
    luaL_pushobjptr(L, "Unit", pSource);
    lua_call(L, 3, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitDamageTargetDone(float fDamage, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitDamageTargetDone");
    lua_pushvalue(L, a);
    lua_pushnumber(L, fDamage);
    luaL_pushobjptr(L, "Unit", pTarget);
    lua_call(L, 3, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitProjectileEffect(const CPoint& p, CUnit* pTarget)
{
    if (getScriptHandler() == 0)
    {
        return;
    }
    
    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitProjectileEffect");
    lua_pushvalue(L, a);
    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);
    luaL_pushobjptr(L, "Unit", pTarget);
    lua_call(L, 4, 0);
    
    lua_pop(L, 1);  // pop 'a'
}

void CAbility::onUnitAbilityProjectileEffect(const CPoint& p, CUnit* pTarget)
{

}


void CAbility::onAddToUnit(CUnit* pOwner)
{
    setOwner(pOwner);
    onUnitAddAbility();
}

void CAbility::onDelFromUnit()
{
    onUnitDelAbility();
    setOwner(NULL);
}

void CAbility::copyScriptHandler(int iScriptHandler)
{
    if (iScriptHandler == 0)
    {
        return;
    }

    lua_State* L = CWorld::getLuaHandle();

    // copy source lua obj
    lua_getglobal(L, "table");
    lua_getfield(L, -1, "copy");
    luaL_getregistery(L, iScriptHandler);
    lua_call(L, 1, 1);

    lua_pushlightuserdata(L, this);
    lua_setfield(L, -2, "_p");
    setScriptHandler(luaL_ref(L, LUA_REGISTRYINDEX));

    lua_pop(L, 1);  // pop "table"
}

void CAbility::setTriggerFlags(uint32_t dwTriggerFlags)
{
    m_dwTriggerFlags |= dwTriggerFlags;
}

void CAbility::unsetTriggerFlags(uint32_t dwTriggerFlags)
{
    m_dwTriggerFlags &= ~dwTriggerFlags;
}

// CActiveAbility
CActiveAbility::CActiveAbility(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType, uint32_t dwEffectiveTypeFlags)
: CAbility(pRootId, pName, fCoolDown)
, m_eCastTargetType(eCastType)
, m_dwEffectiveTypeFlags(dwEffectiveTypeFlags)
, m_fCastMinRange(0.0f)
, m_fCastRange(0.0f)
, m_fCastTargetRadius(0.0f)
, m_iTemplateProjectile(0)
, m_bCastHorizontal(false)
{
    setDbgClassName("CActiveAbility");
}

CActiveAbility::~CActiveAbility()
{
}

const float CActiveAbility::CONST_MAX_CAST_BUFFER_RANGE = 50.0f;
const float CActiveAbility::CONST_MAX_HOR_CAST_Y_RANGE = 5.0f;

bool CActiveAbility::cast()
{
    if (isCoolingDown() == true)
    {
        return false;
    }

    if (checkConditions() == false)
    {
        return false;
    }

    CUnit* o = getOwner();
    LOG("%s%s%s..", o->getName(), o->getAttackAbilityId() == getId() ? "的" : "施放了", getName());
    coolDown();
    onUnitCastAbility();  // onCastAbility在cd变化下面，所以可以添加重置cd的逻辑
    
    return true;
}

bool CActiveAbility::checkConditions()
{
    if (getScriptHandler() == 0)
    {
        return true;
    }

    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "checkConditions");
    lua_pushvalue(L, a);
    lua_call(L, 1, 1);
    bool res = lua_toboolean(L, -1) != 0;
    lua_pop(L, 1);

    return res;
}

void CActiveAbility::onUnitCastAbility()
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CWorld::getLuaHandle();
    int a = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, a, "onUnitCastAbility");
    lua_pushvalue(L, a);
    lua_call(L, 1, 0);

    lua_pop(L, 1);
}

void CActiveAbility::addCastAnimation(int id)
{
    m_vecCastAnis.push_back(id);
}

int CActiveAbility::getCastRandomAnimation() const
{
    if (m_vecCastAnis.empty())
    {
        return -1;
    }

    return m_vecCastAnis[rand() % m_vecCastAnis.size()];
}

void CActiveAbility::effect()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    // Ability
    
    coolDown();

    onUnitCastAbility();

    switch (getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        break;

    case CCommandTarget::kUnitTarget:
        {
            CUnit* t = o->getUnit(d->getCastTarget().getTargetUnit());

            if (t == NULL || t->isDead())
            {
                return;
            }

            if (getTemplateProjectile() != 0)
            {
                fireProjectile(getTemplateProjectile(), d->getCastTarget());
            }
            else
            {
                onUnitAbilityProjectileEffect(d->getCastTarget().getTargetPoint(), t);
            }
        }
        break;

    case CCommandTarget::kPointTarget:
        break;
    }

    
#if 1
    // onCastAbility在cd变化下面，所以可以添加重置cd的逻辑
#else
    ad->retain();
    TEST_ATTACK_INFO* pAi = new TEST_ATTACK_INFO;
    pAi->iTarget = t->getId();
    pAi->pAttackData = ad;
    o->runAction(new CCallFunc(this, (FUNC_CALLFUNC_ND)&CAttackAct::onTestAttackEffect, pAi));
#endif
}


// CPassiveAbility
CPassiveAbility::CPassiveAbility(const char* pRootId, const char* pName, float fCoolDown)
: CAbility(pRootId, pName, fCoolDown)
{
    setDbgClassName("CPassiveAbility");
}

CPassiveAbility::~CPassiveAbility()
{
}

// CBuffAbility
CBuffAbility::CBuffAbility(const char* pRootId, const char* pName, float fDuration, bool bStackable)
: CAbility(pRootId, pName, 0.0f)
, m_fDuration(fDuration)
, m_fElapsed(0.0f)
, m_bStackable(bStackable)
, m_iSrcUnit(0)
{
    setDbgClassName("CBuffAbility");
}

CMultiRefObject* CBuffAbility::copy() const
{
    CBuffAbility* ret = new CBuffAbility(getRootId(), getName(), m_fDuration, m_bStackable);
    //ret->setScriptHandler(getScriptHandler());
    ret->setSrcUnit(getSrcUnit());
    ret->setLevel(getLevel());
    ret->setInterval(getInterval());
    ret->setTriggerFlags(getTriggerFlags());
    ret->copyScriptHandler(getScriptHandler());
    
    return ret;
}

bool CBuffAbility::isDone() const
{
    return m_fElapsed >= m_fDuration;
}

// CAttackAct
const float CAttackAct::CONST_MIN_ATTACK_SPEED_INTERVAL = 0.1f; // 0.1s
const float CAttackAct::CONST_MIN_ATTACK_SPEED_MULRIPLE = 0.2f; // 20%
const float CAttackAct::CONST_MAX_ATTACK_SPEED_MULRIPLE = 5.0f; // 500%

CAttackAct::CAttackAct(const char* pRootId, const char* pName, float fCoolDown, const CAttackValue& rAttackValue, float fAttackValueRandomRange)
: CActiveAbility(pRootId, pName, fCoolDown, CCommandTarget::kUnitTarget, CUnitForce::kEnemy)
, m_oAttackValue(rAttackValue)
, m_fAttackValueRandomRange(fAttackValueRandomRange)
{
    setDbgClassName("CAttackAct");
}

CMultiRefObject* CAttackAct::copy() const
{
    CAttackAct* pRet = new CAttackAct(getRootId(), getName(), m_fCoolDown, m_oAttackValue, m_fAttackValueRandomRange);
    pRet->setCastTargetType(getCastTargetType());
    pRet->setEffectiveTypeFlags(getEffectiveTypeFlags());
    pRet->setCastMinRange(getCastMinRange());
    pRet->setCastRange(getCastRange());
    pRet->setCastTargetRadius(getCastTargetRadius());
    pRet->setTemplateProjectile(getTemplateProjectile());
    pRet->setCastHorizontal(isCastHorizontal());
    pRet->m_vecCastAnis = m_vecCastAnis;
    return pRet;
}

void CAttackAct::onUnitAddAbility()
{
    getOwner()->setAttackAbilityId(getId());
}

void CAttackAct::onUnitDelAbility()
{
    getOwner()->setAttackAbilityId(0);
}

bool CAttackAct::checkConditions()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != NULL);

    CUnit* t = o->getUnit(d->getCastTarget().getTargetUnit());
    if (t == NULL || t->isDead())
    {
        return false;
    }
    
    return true;
}

void CAttackAct::onUnitCastAbility()
{
}

void CAttackAct::onUnitAbilityProjectileEffect(const CPoint& p, CUnit* pTarget)
{
    CUnit* o = getOwner();

    CAttackData* ad = new CAttackData();
    ad->setAttackValue(getBaseAttack().getType(), getRealAttackValue());

    ad = o->attackAdv(ad, pTarget);
    if (ad == NULL)
    {
        return;
    }

    if (pTarget != NULL)
    {
        pTarget->damagedAdv(ad, o);
    }
}

float CAttackAct::getBaseAttackValue() const
{
    return m_oAttackValue.getValue();
}

float CAttackAct::getRealAttackValue(bool bUseRandomRange) const
{
    if (bUseRandomRange)
    {
        float fAttackValueRandomRange = m_oExAttackValueRandomRange.getValue(m_fAttackValueRandomRange);
        if (fAttackValueRandomRange > 0.001)
        {
            return m_aoExAttackValue.getValue(m_oAttackValue.getValue()) * (1 - fAttackValueRandomRange * 0.5 + (rand() % (int)(fAttackValueRandomRange * 1000)) * 0.001);
        }
    }
    return m_aoExAttackValue.getValue(m_oAttackValue.getValue());
}

float CAttackAct::getCoolDown() const
{
    return getRealAttackInterval();
}

void CAttackAct::setBaseAttackInterval(float fAttackInterval)
{
    m_fCoolDown = max(fAttackInterval, CONST_MIN_ATTACK_SPEED_INTERVAL);
}

float CAttackAct::getBaseAttackInterval() const
{
    return m_fCoolDown;
}

float CAttackAct::getBaseAttackSpeed() const
{
    return 1 / max(getBaseAttackInterval(), FLT_EPSILON);
}

float CAttackAct::getRealAttackInterval() const
{
    float fAttackInterval = getBaseAttackInterval();
    // 取攻击速度系数，不得小于最小值
    float fMulriple = m_oExAttackSpeed.getMulriple();
    fMulriple = max(fMulriple, CONST_MIN_ATTACK_SPEED_MULRIPLE);
    // 计算两种最短攻击间隔中的最大值作为最短攻击间隔
    float fMinAttackSpeedInterval = fAttackInterval / CONST_MAX_ATTACK_SPEED_MULRIPLE;
    fMinAttackSpeedInterval = max(CONST_MIN_ATTACK_SPEED_INTERVAL, fMinAttackSpeedInterval);
    // 计算实际攻击间隔，不得小于上述最短攻击间隔
    float fRealAttackInterval = fAttackInterval / fMulriple;
    return max(fRealAttackInterval, fMinAttackSpeedInterval);
}

float CAttackAct::getRealAttackSpeed() const
{
    return 1 / getRealAttackInterval();
}

void CAttackAct::setExAttackSpeed(const CExtraCoeff& roExAttackSpeed)
{
    m_oExAttackSpeed = roExAttackSpeed;  // 必须保留原值，尽管有可能超出范围，否则技能删除后无法恢复
    updateAttackSpeed();
}

const CExtraCoeff& CAttackAct::getExAttackSpeed() const
{
    return m_oExAttackSpeed;
}

void CAttackAct::updateAttackSpeed()
{
    CUnit* o = getOwner();
    assert(o != NULL);
    o->updateAbilityCD(getId());
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    if (d->getCastActiveAbilityId() == o->getAttackAbilityId())
    {
        float spd = getBaseAttackInterval() / max(FLT_EPSILON, getRealAttackInterval());
        LOG("ATK SPD: %.1f", spd);
        d->setActionSpeed(d->getCastActionId(), spd);
    }
    
}

void CAttackAct::onTestAttackEffect(CMultiRefObject* pObj, void* pData)
{
    TEST_ATTACK_INFO* pAi = (TEST_ATTACK_INFO*)pData;
    
    CUnit* o = getOwner();
    CUnit* t = o->getUnit(pAi->iTarget);
    CAttackData* ad = pAi->pAttackData;
    
    if (t != NULL && t->isDead() == false)
    {
        t->damagedAdv(ad, o);
    }
    
    ad->release();
    
    delete pAi;
}

// CBuffMakerAct
CBuffMakerAct::CBuffMakerAct(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType, uint32_t dwEffectiveTypeFlags, float fChance, int iTemplateBuff)
: CActiveAbility(pRootId, pName, fCoolDown, eCastType, dwEffectiveTypeFlags)
, m_fChance(fChance)
, m_iTemplateBuff(iTemplateBuff)
, m_pTarget(NULL)
{
    setDbgClassName("CBuffMakerAct");
}

CMultiRefObject* CBuffMakerAct::copy() const
{
    CBuffMakerAct* pRet = new CBuffMakerAct(getRootId(), getName(), getCoolDown(), getCastTargetType(), getEffectiveTypeFlags(), getChance(), getTemplateBuff());
    pRet->setCastRange(getCastRange());
    pRet->setCastTargetRadius(getCastTargetRadius());
    pRet->setTemplateProjectile(getTemplateProjectile());
    return pRet;
}

bool CBuffMakerAct::checkConditions()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != NULL);

    switch (getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        m_pTarget = o;
        break;
        
    case CCommandTarget::kUnitTarget:
        m_pTarget = o->getUnit(d->getCastTarget().getTargetUnit());
        if (m_pTarget != NULL && m_pTarget->isDead())
        {
            m_pTarget = NULL;
        }
        break;
        
    default:
        ;
    }
    
    if (m_pTarget != NULL &&
        !o->isEffective(DCAST(m_pTarget, CUnitForce*), getEffectiveTypeFlags()))
    {
        // 如果有待选目标(自身或命令目标)，但是无法作用
        if (getCastTargetRadius() <= FLT_EPSILON ||
            getEffectiveTypeFlags() == CUnitForce::kSelf)
        {
            // 如果不能扩展周围单位
            return false;
        }
    }
    
    return true;
}

void CBuffMakerAct::onUnitCastAbility()
{
}

void CBuffMakerAct::onUnitAbilityProjectileEffect( const CPoint& p, CUnit* pTarget )
{
    CUnit* o = getOwner();

    switch (getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
    case CCommandTarget::kUnitTarget:

        if (M_RAND_HIT(m_fChance) && o->isEffective(DCAST(m_pTarget, CUnitForce*), getEffectiveTypeFlags()))
        {
            m_pTarget->addBuffAbility(getTemplateBuff(), o->getId(), getLevel());
        }
        break;

    default:
        ;
    }

    if (getCastTargetRadius() <= FLT_EPSILON)
    {
        return;
    }

    CWorld* w = o->getWorld();
    CUnitDraw2D* od  = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != NULL);
    CBuffAbility* pBuff = NULL;
    CWorld::MAP_UNITS& mapUnits = w->getUnits();
    M_MAP_FOREACH(mapUnits)
    {
        CUnit* u = M_MAP_EACH;
        M_MAP_NEXT;

        if (M_RAND_HIT(m_fChance) == false || u == NULL || u->isDead())
        {
            continue;
        }

        if (!o->isEffective(DCAST(u, CUnitForce*), m_dwEffectiveTypeFlags))
        {
            continue;
        }

        CUnitDraw2D* ud  = DCAST(u->getDraw(), CUnitDraw2D*);
        assert(ud != NULL);
        if (ud->getPosition().getDistance(od->getPosition()) > getCastTargetRadius())
        {
            continue;
        }

        if (pBuff == NULL)
        {
            w->copyAbility(getTemplateBuff())->dcast(pBuff);
        }
        else
        {
            pBuff->copy()->dcast(pBuff);
        }

        u->addBuffAbility(pBuff);
    }
}


// CAuraPas
CAuraPas::CAuraPas(const char* pRootId, const char* pName, float fInterval, int iTemplateBuff, float fRange, uint32_t dwEffectiveTypeFlags)
: CPassiveAbility(pRootId, pName)
, m_iTemplateBuff(iTemplateBuff)
, m_fRange(fRange)
, m_dwEffectiveTypeFlags(dwEffectiveTypeFlags)
{
    setDbgClassName("CAuraPas");
    setInterval(fInterval);
}

CAuraPas::~CAuraPas()
{
}

CMultiRefObject* CAuraPas::copy() const
{
    return new CAuraPas(getRootId(), getName(), m_fInterval, m_iTemplateBuff, m_fRange, m_dwEffectiveTypeFlags);
}

void CAuraPas::onUnitInterval()
{
    CUnit* o = getOwner();
    CWorld* w = o->getWorld();
    CBuffAbility* pBuff = NULL;
    
    CUnitDraw2D* od = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != NULL);
    CWorld::MAP_UNITS& mapUnits = w->getUnits();
    M_MAP_FOREACH(mapUnits)
    {
        CUnit* u = M_MAP_EACH;
        M_MAP_NEXT;
        
        if (u == NULL || u->isDead())
        {
            continue;
        }
        
        if (!o->isEffective(DCAST(u, CUnitForce*), m_dwEffectiveTypeFlags))
        {
            continue;
        }

        CUnitDraw2D* ud = DCAST(u->getDraw(), CUnitDraw2D*);
        assert(ud != NULL);
        if (ud->getPosition().getDistance(od->getPosition()) > m_fRange)
        {
            continue;
        }
        
        if (pBuff == NULL)
        {
            w->copyAbility(m_iTemplateBuff)->dcast(pBuff);
        }
        else
        {
            pBuff->copy()->dcast(pBuff);
        }
        
        u->addBuffAbility(pBuff);
    }
}

// CAttackBuffMakerPas
CAttackBuffMakerPas::CAttackBuffMakerPas(const char* pRootId, const char* pName, float fChance, int iTemplateBuff, bool bToSelf, const CExtraCoeff& roExAttackValue)
: CPassiveAbility(pRootId, pName)
, m_fChance(fChance)
, m_iTemplateBuff(iTemplateBuff)
, m_bToSelf(bToSelf)
, m_oExAttackValue(roExAttackValue)
{
    setDbgClassName("CAttackBuffMakerPas");
    setTriggerFlags(CUnit::kOnAttackTargetTrigger);
}

CMultiRefObject* CAttackBuffMakerPas::copy() const
{
    CAttackBuffMakerPas* ret = new CAttackBuffMakerPas(getRootId(), getName(), m_fChance, m_iTemplateBuff, m_bToSelf, m_oExAttackValue);
    ret->setCoolDown(getCoolDown());
    return ret;
}

CAttackData* CAttackBuffMakerPas::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    if (isCoolingDown())
    {
        return pAttack;
    }

    coolDown();

    if (M_RAND_HIT(m_fChance) == false)
    {
        return pAttack;
    }
    
    pAttack->getAttackValue().setValue(m_oExAttackValue.getValue(pAttack->getAttackValue().getValue()));
    
    if (m_iTemplateBuff != 0)
    {
        if (isToSelf())
        {
            CUnit* o = getOwner();
            o->addBuffAbility(m_iTemplateBuff, o->getId(), getLevel());
        }
        else
        {
            pAttack->addAttackBuff(CAttackBuff(m_iTemplateBuff, getLevel()));
        }
    }
    
    return pAttack;
}

// CDamageBuffMakerBuff
CDamageBuffMakerBuff::CDamageBuffMakerBuff( const char* pName, const CAttackValue& rDamage, float fChance, int iTemplateBuff, bool bToSelf /*= false*/, const CExtraCoeff& roExAttackValue /*= CExtraCoeff()*/ )
: CBuffAbility("DBM", pName, 0.0f, true)
, m_oDamage(rDamage)
, m_fChance(fChance)
, m_iTemplateBuff(iTemplateBuff)
, m_bToSelf(bToSelf)
, m_oExAttackValue(roExAttackValue)
{
}

CMultiRefObject* CDamageBuffMakerBuff::copy() const
{
    return new CDamageBuffMakerBuff(getName(), getDamage(), getChance(), getTemplateBuff(), isToSelf(), getExAttackValue());
}

void CDamageBuffMakerBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    CUnit* s = o->getUnit(getSrcUnit());
    //CUnit* sd = DCAST(s->getDraw(), CUnitDraw2D*);
    
    
    CAttackData* ad = new CAttackData;
    if (getDamage().getValue() == 0.0f)
    {
        CAttackAct* atk = NULL;
        s->getActiveAbility(s->getAttackAbilityId())->dcast(atk);
        if (atk != NULL)
        {
            ad->setAttackValue(atk->getBaseAttack().getType(), getExAttackValue().getValue(atk->getRealAttackValue()));
        }
    }
    else
    {
        ad->setAttackValue(getDamage());
    }
    
    if (M_RAND_HIT(getChance()))
    {
        if (isToSelf())
        {
            if (s != NULL && !s->isDead())
            {
                s->addBuffAbility(getTemplateBuff(), getSrcUnit(), getLevel());
            }
        }
        else
        {
            ad->addAttackBuff(CAttackBuff(getTemplateBuff(), getLevel()));
        }
        
    }

    o->damagedAdv(ad, s);
}

// CVampirePas
CVampirePas::CVampirePas(const char* pRootId, const char* pName, float fPercentConversion)
: CPassiveAbility(pRootId, pName)
, m_fPercentConversion(fPercentConversion)
{
    setDbgClassName("CVampirePas");
    setTriggerFlags(CUnit::kOnDamageTargetDoneTrigger);
}

CMultiRefObject* CVampirePas::copy() const
{
    return new CVampirePas(getRootId(), getName(), getPercentConversion());
}

void CVampirePas::onUnitDamageTargetDone(float fDamage, CUnit* pTarget)
{
    if (isCoolingDown())
    {
        return;
    }

    coolDown();

    CUnit* o = getOwner();
    float fDtHp = fDamage * getPercentConversion();
    o->setHp(o->getHp() + fDtHp);
    LOG("%s恢复%d点HP", o->getName(), toInt(fDtHp));

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnit* u = getOwner();
    CUnitDrawForCC* d = NULL;
    u->getDraw()->dcast(d);

    if (d != NULL)
    {
        char sz[64];
        sprintf(sz, "+%d", toInt(fDtHp));
        d->addBattleTip(sz, "Comic Book", 18, ccc3(113, 205, 44));
    }
#endif
}

// CStunBuff
CStunBuff::CStunBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
{
    setDbgClassName("CStunBuff");
}

CMultiRefObject* CStunBuff::copy() const
{
    return new CStunBuff(getRootId(), getName(), m_fDuration, m_bStackable);
}

void CStunBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(d != NULL);
    d->cmdStop();
    o->suspend();
    
    LOG("%s%s中", o->getName(), getName());

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnitDrawForCC* ccd = NULL;
    o->getDraw()->dcast(ccd);

    if (ccd != NULL)
    {
        char sz[64];
        sprintf(sz, "%s!", getName());
        ccd->addBattleTip(sz, "Comic Book", 18, ccc3(250, 104, 16));
    }
#endif
}

void CStunBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    o->resume();
    
    if (!o->isSuspended())
    {
        LOG("%s不在%s", o->getName(), getName());
    }
}

// CDoubleAttackPas
CDoubleAttackPas::CDoubleAttackPas(const char* pRootId, const char* pName, float fChange, const CExtraCoeff& roExAttackValue)
: CAttackBuffMakerPas(pRootId, pName, fChange, 0, true, roExAttackValue)
{
    setDbgClassName("CDoubleAttackPas");
}

CMultiRefObject* CDoubleAttackPas::copy() const
{
    return new CDoubleAttackPas(getRootId(), getName(), m_fChance, m_oExAttackValue);
}

CAttackData* CDoubleAttackPas::onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget)
{
    CUnit* o = getOwner();
    if (o->getAttackAbilityId() == 0)
    {
        return pAttack;
    }

    if (isCoolingDown())
    {
        return pAttack;
    }

    coolDown();

    if (M_RAND_HIT(m_fChance) == false)
    {
        return pAttack;
    }

    pAttack->getAttackValue().setValue(m_oExAttackValue.getValue(pAttack->getAttackValue().getValue()));

    CAttackAct* pAtk = NULL;
    o->getActiveAbility(o->getAttackAbilityId())->dcast(pAtk);
    
    pAtk->resetCD();
    
    LOG("%s将进行%s", o->getName(), getName());

    return pAttack;
}

// CSpeedBuff
CSpeedBuff::CSpeedBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, const CExtraCoeff& roExMoveSpeedDelta, const CExtraCoeff& roExAttackSpeedDelta)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
, m_oExMoveSpeedDelta(roExMoveSpeedDelta)
, m_oExAttackSpeedDelta(roExAttackSpeedDelta)
{
    setDbgClassName("CSpeedBuff");
}

CMultiRefObject* CSpeedBuff::copy() const
{
    return new CSpeedBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_oExMoveSpeedDelta, m_oExAttackSpeedDelta);
}

void CSpeedBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    
    CUnitDraw2D* od = DCAST(o->getDraw(), CUnitDraw2D*);
    assert(od != NULL);
    const CExtraCoeff& rExMs = od->getExMoveSpeed();
    od->setExMoveSpeed(CExtraCoeff(rExMs.getMulriple() + m_oExMoveSpeedDelta.getMulriple(), rExMs.getAddend() + m_oExMoveSpeedDelta.getAddend()));
    
    CAttackAct* pAtkAct = NULL;
    o->getActiveAbility(o->getAttackAbilityId())->dcast(pAtkAct);
    if (pAtkAct == NULL)
    {
        return;
    }
    
    float fTestOld = pAtkAct->getRealAttackInterval();
    const CExtraCoeff& rExAs = pAtkAct->getExAttackSpeed();
    pAtkAct->setExAttackSpeed(CExtraCoeff(rExAs.getMulriple() + m_oExAttackSpeedDelta.getMulriple(), rExAs.getAddend() + m_oExAttackSpeedDelta.getAddend()));
    
    LOG("%s攻击速度变慢(%.1fs->%.1fs)\n", o->getName(), fTestOld, pAtkAct->getRealAttackInterval());

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnitDrawForCC* ccd = NULL;
    o->getDraw()->dcast(ccd);

    if (ccd != NULL)
    {
        char sz[64];
        sprintf(sz, "%s!", getName());
        ccd->addBattleTip(sz, "Comic Book", 18, ccc3(72, 130, 200));
    }
#endif
}

void CSpeedBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    
    CUnitDraw2D* d = DCAST(o->getDraw(), CUnitDraw2D*);
    const CExtraCoeff& rExMs = d->getExMoveSpeed();
    d->setExMoveSpeed(CExtraCoeff(rExMs.getMulriple() - m_oExMoveSpeedDelta.getMulriple(), rExMs.getAddend() - m_oExMoveSpeedDelta.getAddend()));
    
    CAttackAct* pAtkAct = NULL;
    o->getActiveAbility(o->getAttackAbilityId())->dcast(pAtkAct);
    if (pAtkAct == NULL)
    {
        return;
    }
    
    float fTestOld = pAtkAct->getRealAttackInterval();
    const CExtraCoeff& rExAs = pAtkAct->getExAttackSpeed();
    pAtkAct->setExAttackSpeed(CExtraCoeff(rExAs.getMulriple() - m_oExAttackSpeedDelta.getMulriple(), rExAs.getAddend() - m_oExAttackSpeedDelta.getAddend()));
    
    LOG("%s攻击速度恢复(%.1fs->%.1fs)\n", o->getName(), fTestOld, pAtkAct->getRealAttackInterval());
}

// CChangeHpPas
CChangeHpPas::CChangeHpPas(const char* pRootId, const char* pName, float fInterval, const CExtraCoeff& roChangeHp, const CExtraCoeff& roMinHp)
    : CPassiveAbility(pRootId, pName)
    , m_oChangeHp(roChangeHp)
    , m_oMinHp(roMinHp)
{
    setDbgClassName("CChangeHpPas");
    setInterval(fInterval);
}

CMultiRefObject* CChangeHpPas::copy() const
{
    return new CChangeHpPas(getRootId(), getName(), m_fInterval, m_oChangeHp, m_oMinHp);
}

void CChangeHpPas::onUnitAddAbility()
{
    CUnit* o = getOwner();
    LOG("%s获得%s状态(%.1f/s)\n", o->getName(), getName(), getChangeHp().getValue(o->getMaxHp()));
}

void CChangeHpPas::onUnitDelAbility()
{
    CUnit* o = getOwner();
    LOG("%s失去%s状态\n", o->getName(), getName());
}

void CChangeHpPas::onUnitInterval()
{
    CUnit* o = getOwner();
    float fNewHp = o->getHp();
    float fChangeHp = getChangeHp().getValue(o->getMaxHp());
    fNewHp += fChangeHp;

    float fMinHp = getMinHp().getValue(o->getMaxHp());
    if (fNewHp < fMinHp)
    {
        fNewHp = fMinHp;
    }

    o->setHp(fNewHp);
}

// CChangeHpBuff
CChangeHpBuff::CChangeHpBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fInterval, const CExtraCoeff& roChangeHp, const CExtraCoeff& roMinHp)
: CBuffAbility(pRootId, pName, fDuration, bStackable)
, m_oChangeHp(roChangeHp)
, m_oMinHp(roMinHp)
{
    setDbgClassName("CChangeHpBuff");
    setInterval(fInterval);
}

CMultiRefObject* CChangeHpBuff::copy() const
{
    return new CChangeHpBuff(getRootId(), getName(), m_fDuration, m_bStackable, m_fInterval, m_oChangeHp, m_oMinHp);
}

void CChangeHpBuff::onUnitAddAbility()
{
    CUnit* o = getOwner();
    LOG("%s获得%s状态(%.1f/s)\n", o->getName(), getName(), getChangeHp().getValue(o->getMaxHp()));
}

void CChangeHpBuff::onUnitDelAbility()
{
    CUnit* o = getOwner();
    LOG("%s失去%s状态\n", o->getName(), getName());
}

void CChangeHpBuff::onUnitInterval()
{
    CUnit* o = getOwner();
    float fNewHp = o->getHp();
    float fChangeHp = getChangeHp().getValue(o->getMaxHp());
    fNewHp += fChangeHp;

    float fMinHp = getMinHp().getValue(o->getMaxHp());
    if (fNewHp < fMinHp)
    {
        fNewHp = fMinHp;
    }

    o->setHp(fNewHp);
}

// CRebirthPas
CRebirthPas::CRebirthPas(const char* pRootId, const char* pName, float fCoolDown, const CExtraCoeff& rExMaxHp)
    : CPassiveAbility(pRootId, pName, fCoolDown)
    , m_oExMaxHp(rExMaxHp)
    , m_bRevivableBefore(false)
{
    setDbgClassName("CRebirthPas");
    setTriggerFlags(CUnit::kOnDeadTrigger);
}

CMultiRefObject* CRebirthPas::copy() const
{
    return new CRebirthPas(getRootId(), getName(), getCoolDown(), m_oExMaxHp);
}

void CRebirthPas::onUnitAddAbility()
{
    CUnit* o = getOwner();
    setRevivableBefore(o->isRevivable());
    o->setRevivable();
}

void CRebirthPas::onUnitDelAbility()
{
    CUnit* o = getOwner();
    o->setRevivable(isRevivableBefore());
}

void CRebirthPas::onUnitDead()
{
    if (isCoolingDown())
    {
        return;
    }

    coolDown();

    CUnit* o = getOwner();
    CWorld* w = o->getWorld();

    CUnit* oo = w->getUnit(o->getId());
    float fHp = getExMaxHp().getValue(o->getMaxHp());
    if (oo != NULL)
    {
        oo->revive(fHp);
    }
    else
    {
        w->reviveUnit(o->getId(), fHp);
    }

#ifdef DEBUG_FOR_CC
    // for cocos2d
    CUnitDrawForCC* ccd = NULL;
    o->getDraw()->dcast(ccd);

    if (ccd != NULL)
    {
        char sz[64];
        sprintf(sz, "%s!", getName());
        ccd->addBattleTip(sz, "Comic Book", 18, ccc3(217, 47, 111));
    }
#endif

    LOG("Doing: %u", o->getDoingFlags());
}

// CEvadePas
CEvadePas::CEvadePas(const char* pRootId, const char* pName, float fChance, int iTemplateBuff)
    : CPassiveAbility(pRootId, pName)
    , m_fChance(fChance)
    , m_iTemplateBuff(iTemplateBuff)
{
    setDbgClassName("CEvadePas");
    setTriggerFlags(CUnit::kOnAttackedTrigger);
}

CMultiRefObject* CEvadePas::copy() const
{
    return new CEvadePas(getRootId(), getName(), m_fChance, m_iTemplateBuff);
}

CAttackData* CEvadePas::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    if (M_RAND_HIT(getChance()))
    {
        CUnit* o = getOwner();
        if (getTemplateBuff() != 0)
        {
            o->addBuffAbility(getTemplateBuff(), o->getId(), getLevel());
        }

        LOG("%s%s了%s的攻击", getOwner()->getName(), getName(), pSource->getName());

#ifdef DEBUG_FOR_CC
        // for cocos2d
        CUnitDrawForCC* ccd = NULL;
        o->getDraw()->dcast(ccd);

        if (ccd != NULL)
        {
            char sz[64];
            sprintf(sz, "%s!", getName());
            ccd->addBattleTip(sz, "Comic Book", 18, ccc3(250, 104, 16));
        }
#endif
        return NULL;
    }

    return pAttack;
}

// CEvadeBuff
CEvadeBuff::CEvadeBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fChance)
    : CBuffAbility(pRootId, pName, fDuration, bStackable)
    , m_fChance(fChance)
{
    setDbgClassName("CEvadeBuff");
    setTriggerFlags(CUnit::kOnAttackedTrigger);
}

CMultiRefObject* CEvadeBuff::copy() const
{
    return new CEvadeBuff(getRootId(), getName(), getDuration(), isStackable(), m_fChance);
}

CAttackData* CEvadeBuff::onUnitAttacked(CAttackData* pAttack, CUnit* pSource)
{
    if (M_RAND_HIT(getChance()))
    {
        LOG("%s%s了%s的攻击", getOwner()->getName(), getName(), pSource->getName());

#ifdef DEBUG_FOR_CC
        // for cocos2d
        CUnit* o = getOwner();
        CUnitDrawForCC* ccd = NULL;
        o->getDraw()->dcast(ccd);

        if (ccd != NULL)
        {
            char sz[64];
            sprintf(sz, "%s!", getName());
            ccd->addBattleTip(sz, "Comic Book", 18, ccc3(250, 104, 16));
        }
        return NULL;
#endif
    }
    
    return pAttack;
}

