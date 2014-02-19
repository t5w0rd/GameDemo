/* 
 * File:   Skill.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午11:45
 */

#ifndef __SKILL_H__
#define	__SKILL_H__

#include "MultiRefObject.h"
#include "Level.h"
#include "Base.h"
#include "Action.h"
#include "Unit.h"


class CSkill : public CMultiRefObject, public CLevelExp
{
protected:
    const string CONST_ROOT_ID;
    
public:
    CSkill(const char* pRootId, const char* pName, float fCoolDown = 0.0f);
    virtual ~CSkill();
    
    virtual const char* getDbgTag() const;
    
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
    
    // 技能持有者事件响应，只覆被注册的触发器相应的事件函数即可
    // @override
    virtual void onUnitAddSkill();
    virtual void onUnitDelSkill();
    virtual void onUnitSkillReady();
    virtual void onUnitRevive();
    virtual void onUnitDie();
    virtual void onUnitChangeHp(float fChanged);
    virtual void onUnitTick(float dt);
    virtual void onUnitInterval();
    virtual CAttackData* onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    virtual CAttackData* onUnitAttacked(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitDamagedDone(float fDamage, CUnit* pSource);
    virtual void onUnitDamageTargetDone(float fDamage, CUnit* pTarget);
    
    virtual void onUnitDestroyProjectile(CProjectile* pProjectile);
    
public:
    // 来自CUnit内部调用，bNotify为false时，不需要通知onUnitAddSkill，通常这种情况在Buff被覆盖的时候发生
    void onAddToUnit(CUnit* pOwner);
    void onDelFromUnit();
    
    M_SYNTHESIZE_READONLY(uint32_t, m_dwTriggerFlags, TriggerFlags);
    virtual void setTriggerFlags(uint32_t dwTriggerFlags);
    virtual void unsetTriggerFlags(uint32_t dwTriggerFlags);
    
};

class CActiveSkill : public CSkill
{
public:
    CActiveSkill(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType = CCommandTarget::kNoTarget, uint32_t dwEffectiveTypeFlags = CUnitForce::kSelf | CUnitForce::kOwn | CUnitForce::kAlly | CUnitForce::kEnemy);
    virtual ~CActiveSkill();
    
    static const float CONST_MAX_CAST_BUFFER_RANGE;
    static const float CONST_MAX_HOR_CAST_Y_RANGE;

    virtual bool cast();
    virtual bool checkConditions();
    virtual void onUnitCastSkill();
    
    // 限定施法参数
    M_SYNTHESIZE(CCommandTarget::TARGET_TYPE, m_eCastTargetType, CastTargetType);
    M_SYNTHESIZE(uint32_t, m_dwEffectiveTypeFlags, EffectiveTypeFlags)
    M_SYNTHESIZE(float, m_fCastRange, CastRange);  // 施法距离
    M_SYNTHESIZE(float, m_fCastMinRange, CastMinRange);  // 最小施法距离
    M_SYNTHESIZE(float, m_fCastTargetRadius, CastTargetRadius);  // 作用范围
    
    // 传递施法参数，并可能在技能后续持续中使用
    //M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oCastTargetPoint, CastTargetPoint);
    //M_SYNTHESIZE(int, m_iCastTargetUnit, CastTargetUnit);
    
    M_SYNTHESIZE(int, m_iTemplateProjectile, TemplateProjectile);

    M_SYNTHESIZE_BOOL(Horizontal);

    void addCastAnimation(int id);
    int getRandomAnimation() const;
    
};

class CPassiveSkill : public CSkill
{
public:
    CPassiveSkill(const char* pRootId, const char* pName, float fCoolDown = 0);
    virtual ~CPassiveSkill();
    
    
};

class CBuffSkill : public CPassiveSkill
{
public:
    CBuffSkill(const char* pRootId, const char* pName, float fDuration, bool bStackable);
    virtual ~CBuffSkill();

    M_SYNTHESIZE(float, m_fDuration, Duration);
    M_SYNTHESIZE(float, m_fElapsed, Elapsed);
    virtual bool isDone() const;
    
    M_SYNTHESIZE_BOOL(Stackable);
    M_SYNTHESIZE(int, m_iSrcUnit, SrcUnit);
    
};

/////////////////////// ActiveSkills ///////////////////////

// 攻击，默认以单位作为目标
class CAttackAct : public CActiveSkill
{
public:
    static const float CONST_MIN_ATTACK_SPEED_INTERVAL;
    static const float CONST_MIN_ATTACK_SPEED_MULRIPLE;
    static const float CONST_MAX_ATTACK_SPEED_MULRIPLE;
    
public:
    CAttackAct(const char* pRootId, const char* pName, float fCoolDown, const CAttackValue& rAttackValue, float fAttackValueRandomRange = 0.15f);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitAddSkill();
    virtual void onUnitDelSkill();
    virtual bool checkConditions();
    virtual void onUnitCastSkill();
        
    M_SYNTHESIZE_PASS_BY_REF(CAttackValue, m_oAttackValue, AttackValue);
    M_SYNTHESIZE(float, m_fAttackValueRandomRange, AttackValueRandomRange);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackValueRandomRange, ExAttackValueRandomRange);
    
    float getBaseAttackValue(CAttackValue::ATTACK_TYPE eAttackType) const;
    void setExAttackValue(CAttackValue::ATTACK_TYPE eAttackType, const CExtraCoeff& roExAttackValue);
    const CExtraCoeff& getExAttackValue(CAttackValue::ATTACK_TYPE eAttackType) const;
    float getRealAttackValue(CAttackValue::ATTACK_TYPE eAttackType, bool bUseRandomRange = true) const;
    
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
    CExtraCoeff m_aoExAttackValue[CAttackValue::CONST_MAX_ATTACK_TYPE];
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
class CBuffMakerAct : public CActiveSkill
{
public:
    CBuffMakerAct(const char* pRootId, const char* pName, float fCoolDown, int iTemplateBuff, CCommandTarget::TARGET_TYPE eCastType = CCommandTarget::kNoTarget, uint32_t dwEffectiveTypeFlags = CUnitForce::kSelf);
    virtual CMultiRefObject* copy() const;
    
    virtual bool checkConditions();
    virtual void onUnitCastSkill();
    
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    
protected:
    CUnit* m_pTarget;
};

/////////////////////// PassiveSkills & BuffSkills ///////////////////////

// 光环，范围型BUFF附加器
class CAuraPas : public CPassiveSkill
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
class CAttackBuffMakerPas : public CPassiveSkill
{
public:
    CAttackBuffMakerPas(const char* pRootId, const char* pName, float fProbability, int iTemplateBuff, bool bToSelf = false, const CExtraCoeff& roExAttackValue = CExtraCoeff());
    virtual CMultiRefObject* copy() const;
    
    virtual CAttackData* onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    
    M_SYNTHESIZE(float, m_fProbability, Probability);
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    M_SYNTHESIZE_BOOL(ToSelf);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackValue, ExAttackValue);
};

class CVampirePas : public CPassiveSkill
{
public:
    CVampirePas(const char* pRootId, const char* pName, float fPercentConversion);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitDamageTargetDone(float fDamage, CUnit* pTarget);

    M_SYNTHESIZE(float, m_fPercentConversion, PercentConversion);
};

class CStunBuff : public CBuffSkill
{
public:
    CStunBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitAddSkill();
    virtual void onUnitDelSkill();
};

class CDoubleAttackBuff : public CBuffSkill
{
public:
    CDoubleAttackBuff(const char* pRootId, const char* pName);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitAddSkill();
    
};

class CSpeedBuff : public CBuffSkill
{
public:
    CSpeedBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, const CExtraCoeff& roExMoveSpeedDelta, const CExtraCoeff& roExAttackSpeedDelta);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitAddSkill();
    virtual void onUnitDelSkill();
    
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExMoveSpeedDelta, ExMoveSpeedDelta);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackSpeedDelta, ExAttackSpeedDelta);
    
};

class CHpChangeBuff : public CBuffSkill
{
public:
    CHpChangeBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fInterval, float fHpChange, bool bPercentile, float fMinHp = -1.0f);
    virtual CMultiRefObject* copy() const;
    
    virtual void onUnitAddSkill();
    virtual void onUnitDelSkill();
    virtual void onUnitInterval();
    
    M_SYNTHESIZE(float, m_fHpChange, HpChange);
    M_SYNTHESIZE_BOOL(Percentile);
    M_SYNTHESIZE(float, m_fMinHp, MinHp);
    
};

#endif	/* __SKILL_H__ */

