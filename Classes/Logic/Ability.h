/* 
 * File:   Ability.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午11:45
 */

#ifndef __ABILITY_H__
#define __ABILITY_H__

#include "MultiRefObject.h"
#include "Level.h"
#include "Base.h"
#include "Action.h"
#include "Unit.h"


class CAbility : public CMultiRefObject, public CLevelExp
{
protected:
    const string CONST_ROOT_ID;
    
public:
    CAbility(const char* pRootId, const char* pName, float fCoolDown = 0.0f);
    virtual ~CAbility();
    
    virtual const char* getDbgTag() const;
    
    M_SYNTHESIZE(int, m_iScriptHandler, ScriptHandler);
    
    const char* getRootId() const;
    M_SYNTHESIZE_STR(Name);
    
    M_SYNTHESIZE(CUnit*, m_pOwner, Owner);
    
    M_SYNTHESIZE(float, m_fCoolDown, CoolDown);
    M_SYNTHESIZE(float, m_fCoolingDownElapsed, CoolingDownElapsed);
    
    virtual bool isCoolingDown() const;
    virtual void resetCD();
    virtual void coolDown();
    
    M_SYNTHESIZE_READONLY(float, m_fInterval, Interval);
    virtual void setInterval(float fInterval);
    M_SYNTHESIZE(float, m_fIntervalElapsed, IntervalElapsed);

    typedef vector<int> VEC_CAST_ANIS;
    M_SYNTHESIZE_PASS_BY_REF(VEC_CAST_ANIS, m_vecCastAnis, CastAnimations);

    void fireProjectile(int iProjectile, const CCommandTarget& rTarget);
    
    // 技能持有者事件响应，只覆被注册的触发器相应的事件函数即可
    // @override
    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();
    virtual void onUnitAbilityReady();
    virtual void onUnitRevive();
    virtual void onUnitDying();
    virtual void onUnitDead();
    virtual void onUnitChangeHp(float fChanged);
    virtual void onUnitTick(float dt);
    virtual void onUnitInterval();
    virtual CAttackData* onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    virtual CAttackData* onUnitAttacked(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitDamagedDone(float fDamage, CUnit* pSource);
    virtual void onUnitDamageTargetDone(float fDamage, CUnit* pTarget);
    virtual void onUnitProjectileEffect(const CPoint& p, CUnit* pTarget);
    virtual void onUnitAbilityProjectileEffect(const CPoint& p, CUnit* pTarget);
    
public:
    // 来自CUnit内部调用，bNotify为false时，不需要通知onUnitAddAbility，通常这种情况在Buff被覆盖的时候发生
    void onAddToUnit(CUnit* pOwner);
    void onDelFromUnit();
    void copyScriptHandler(int iScriptHandler);
    
    M_SYNTHESIZE_READONLY(uint32_t, m_dwTriggerFlags, TriggerFlags);
    virtual void setTriggerFlags(uint32_t dwTriggerFlags);
    virtual void unsetTriggerFlags(uint32_t dwTriggerFlags);
    
};

class CActiveAbility : public CAbility
{
public:
    CActiveAbility(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType = CCommandTarget::kNoTarget, uint32_t dwEffectiveTypeFlags = CUnitForce::kSelf | CUnitForce::kOwn | CUnitForce::kAlly | CUnitForce::kEnemy);
    virtual ~CActiveAbility();
    
    static const float CONST_MAX_CAST_BUFFER_RANGE;
    static const float CONST_MAX_HOR_CAST_Y_RANGE;

    bool cast();
    virtual bool checkConditions();
    virtual void onUnitCastAbility();

    void effect();
    
    // 限定施法参数
    M_SYNTHESIZE(CCommandTarget::TARGET_TYPE, m_eCastTargetType, CastTargetType);
    M_SYNTHESIZE(uint32_t, m_dwEffectiveTypeFlags, EffectiveTypeFlags);
    M_SYNTHESIZE(float, m_fCastMinRange, CastMinRange);  // 最小施法距离
    M_SYNTHESIZE(float, m_fCastRange, CastRange);  // 施法距离
    M_SYNTHESIZE(float, m_fCastTargetRadius, CastTargetRadius);  // 作用范围
    
    // 传递施法参数，并可能在技能后续持续中使用
    //M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oCastTargetPoint, CastTargetPoint);
    //M_SYNTHESIZE(int, m_iCastTargetUnit, CastTargetUnit);
    
    M_SYNTHESIZE(int, m_iTemplateProjectile, TemplateProjectile);

    M_SYNTHESIZE_BOOL(CastHorizontal);

    void addCastAnimation(int id);
    int getCastRandomAnimation() const;
    
};

class CPassiveAbility : public CAbility
{
public:
    CPassiveAbility(const char* pRootId, const char* pName, float fCoolDown = 0);
    virtual ~CPassiveAbility();

};

class CBuffAbility : public CAbility
{
public:
    CBuffAbility(const char* pRootId, const char* pName, float fDuration, bool bStackable);
    virtual CMultiRefObject* copy() const;

    M_SYNTHESIZE(float, m_fDuration, Duration);
    M_SYNTHESIZE(float, m_fElapsed, Elapsed);
    virtual bool isDone() const;
    
    M_SYNTHESIZE_BOOL(Stackable);
    M_SYNTHESIZE(int, m_iSrcUnit, SrcUnit);
    
};

/////////////////////// ActiveAbilitys ///////////////////////

// 攻击，默认以单位作为目标
class CAttackAct : public CActiveAbility
{
public:
    static const float CONST_MIN_ATTACK_SPEED_INTERVAL;
    static const float CONST_MIN_ATTACK_SPEED_MULRIPLE;
    static const float CONST_MAX_ATTACK_SPEED_MULRIPLE;
    
public:
    CAttackAct(const char* pRootId, const char* pName, float fCoolDown, const CAttackValue& rAttackValue, float fAttackValueRandomRange = 0.15f);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();
    virtual bool checkConditions();
    virtual void onUnitCastAbility();
    virtual void onUnitAbilityProjectileEffect(const CPoint& p, CUnit* pTarget);
        
    M_SYNTHESIZE_PASS_BY_REF(CAttackValue, m_oAttackValue, BaseAttack);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_aoExAttackValue, ExAttackValue);
    M_SYNTHESIZE(float, m_fAttackValueRandomRange, AttackValueRandomRange);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackValueRandomRange, ExAttackValueRandomRange);
    
    float getBaseAttackValue() const;
    float getRealAttackValue(bool bUseRandomRange = true) const;
    
    virtual float getCoolDown() const;
    void setBaseAttackInterval(float fAttackInterval);
    float getBaseAttackInterval() const;
    
    float getBaseAttackSpeed() const;
    
    float getRealAttackInterval() const;
    float getRealAttackSpeed() const;
    
    void setExAttackSpeed(const CExtraCoeff& roExAttackSpeed);
    const CExtraCoeff& getExAttackSpeed() const;
    void updateAttackSpeed();
    
protected:
    CExtraCoeff m_oExAttackSpeed;
    
protected:
    struct TEST_ATTACK_INFO
    {
        int iTarget;
        CAttackData* pAttackData;
    };
    virtual void onTestAttackEffect(CMultiRefObject* pObj, void* pData);
    
};

// 主动型BUFF附加器，支持所有目标种类
class CBuffMakerAct : public CActiveAbility
{
public:
    CBuffMakerAct(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType, uint32_t dwEffectiveTypeFlags, float fChance, int iTemplateBuff);
    virtual CMultiRefObject* copy() const;
    
    virtual bool checkConditions();
    virtual void onUnitCastAbility();
    virtual void onUnitAbilityProjectileEffect(const CPoint& p, CUnit* pTarget);
    
    M_SYNTHESIZE(float, m_fChance, Chance);
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    
protected:
    CUnit* m_pTarget;
};

/////////////////////// PassiveAbilitys & BuffAbilitys ///////////////////////
// 光环，范围型BUFF附加器
class CAuraPas : public CPassiveAbility
{
public:
    CAuraPas(const char* pRootId, const char* pName, float fInterval, int iTemplateBuff, float fRange, uint32_t dwEffectiveTypeFlags);
    virtual ~CAuraPas();
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitInterval();
    
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    M_SYNTHESIZE(float, m_fRange, Range);
    M_SYNTHESIZE(uint32_t, m_dwEffectiveTypeFlags, TargetFlags);
};

// 攻击数据变更，攻击时机会型BUFF附加器
class CAttackBuffMakerPas : public CPassiveAbility
{
public:
    CAttackBuffMakerPas(const char* pRootId, const char* pName, float fChance, int iTemplateBuff, bool bToSelf = false, const CExtraCoeff& roExAttackValue = CExtraCoeff());
    virtual CMultiRefObject* copy() const;
    
    virtual CAttackData* onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    
    M_SYNTHESIZE(float, m_fChance, Chance);
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    M_SYNTHESIZE_BOOL(ToSelf);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackValue, ExAttackValue);
};

class CDamageBuffMakerBuff : public CBuffAbility
{
public:
    CDamageBuffMakerBuff(const char* pName, const CAttackValue& rDamage, float fChance, int iTemplateBuff, bool bToSelf = false, const CExtraCoeff& roExAttackValue = CExtraCoeff());
    virtual CMultiRefObject* copy() const;

    virtual void onUnitAddAbility();

    M_SYNTHESIZE_PASS_BY_REF(CAttackValue, m_oDamage, Damage);
    M_SYNTHESIZE(float, m_fChance, Chance);
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    M_SYNTHESIZE_BOOL(ToSelf);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackValue, ExAttackValue);
};

class CVampirePas : public CPassiveAbility
{
public:
    CVampirePas(const char* pRootId, const char* pName, float fPercentConversion);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitDamageTargetDone(float fDamage, CUnit* pTarget);

    M_SYNTHESIZE(float, m_fPercentConversion, PercentConversion);
};

class CStunBuff : public CBuffAbility
{
public:
    CStunBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();
};

class CDoubleAttackPas : public CAttackBuffMakerPas
{
public:
    CDoubleAttackPas(const char* pRootId, const char* pName, float fChance, const CExtraCoeff& roExAttackValue = CExtraCoeff());
    virtual CMultiRefObject* copy() const;
    
    virtual CAttackData* onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    
};

class CSpeedBuff : public CBuffAbility
{
public:
    CSpeedBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, const CExtraCoeff& roExMoveSpeedDelta, const CExtraCoeff& roExAttackSpeedDelta);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();
    
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExMoveSpeedDelta, ExMoveSpeedDelta);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackSpeedDelta, ExAttackSpeedDelta);
    
};

// hp变化，CExTraCoeff是以MaxHp为基准的
class CChangeHpPas : public CPassiveAbility
{
public:
    CChangeHpPas(const char* pRootId, const char* pName, float fInterval, const CExtraCoeff& roChangeHp, const CExtraCoeff& roMinHp = CExtraCoeff(0.0, -1.0f));
    virtual CMultiRefObject* copy() const;

    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();
    virtual void onUnitInterval();

    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oChangeHp, ChangeHp);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oMinHp, MinHp);

};

class CChangeHpBuff : public CBuffAbility
{
public:
    CChangeHpBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fInterval, const CExtraCoeff& roChangeHp, const CExtraCoeff& roMinHp = CExtraCoeff(0.0, -1.0f));
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();
    virtual void onUnitInterval();
    
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oChangeHp, ChangeHp);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oMinHp, MinHp);
    
};

// 重生
class CRebirthPas : public CPassiveAbility
{
public:
    CRebirthPas(const char* pRootId, const char* pName, float fCoolDown, const CExtraCoeff& rExMaxHp = CExtraCoeff(1.0f, 0.0f));
    virtual CMultiRefObject* copy() const;

    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();
    virtual void onUnitDead();

    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExMaxHp, ExMaxHp)
    M_SYNTHESIZE_BOOL(RevivableBefore);
};

// 闪避
class CEvadePas : public CPassiveAbility
{
public:
    CEvadePas(const char* pRootId, const char* pName, float fChance, int iTemplateBuff = 0);
    virtual CMultiRefObject* copy() const;

    virtual CAttackData* onUnitAttacked(CAttackData* pAttack, CUnit* pSource);

    M_SYNTHESIZE(float, m_fChance, Chance);
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
};

// 闪避
class CEvadeBuff : public CBuffAbility
{
public:
    CEvadeBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fChance);
    virtual CMultiRefObject* copy() const;

    virtual CAttackData* onUnitAttacked(CAttackData* pAttack, CUnit* pSource);

    M_SYNTHESIZE(float, m_fChance, Chance);
};

#endif  /* __ABILITY_H__ */

