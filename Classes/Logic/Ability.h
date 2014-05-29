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


class CAbility : public CLevelExp
{
protected:
    const string CONST_ROOT_ID;
    
public:
    CAbility(const char* pRootId, const char* pName, float fCoolDown = 0.0f);
    virtual ~CAbility();
    
    virtual const char* getDbgTag() const;
    virtual CAbility* copy() override;
    virtual void copyData(CAbility* from);

protected:
    void copyScriptHandler(int iScriptHandler);
    
public:
    M_SYNTHESIZE(int, m_iScriptHandler, ScriptHandler);
    
    const char* getRootId() const;
    M_SYNTHESIZE_STR(Name);
    
    M_SYNTHESIZE(CUnit*, m_pOwner, Owner);
    
    M_SYNTHESIZE(float, m_fCoolDown, CoolDown);
    M_SYNTHESIZE(float, m_fCoolingDownElapsed, CoolingDownElapsed);
    virtual float getRealCoolDown() const;
    
    virtual bool isCoolingDown() const;
    virtual void resetCD();
    virtual void coolDown();
    
    M_SYNTHESIZE_READONLY(float, m_fInterval, Interval);
    virtual void setInterval(float fInterval);
    M_SYNTHESIZE(float, m_fIntervalElapsed, IntervalElapsed);

    typedef vector<int> VEC_CAST_ANIS;
    M_SYNTHESIZE_PASS_BY_REF(VEC_CAST_ANIS, m_vecCastAnis, CastAnimations);
    void addCastAnimation(int id);
    int getCastRandomAnimation() const;

    // 技能持有者事件响应，只覆被注册的触发器相应的事件函数即可
    // @override
    virtual void onChangeLevel(int iChanged) override;
    virtual void onUnitAddAbility();
    virtual void onUnitDelAbility();
    virtual void onUnitAbilityReady();
    virtual void onUnitRevive();
    virtual void onUnitDying();
    virtual void onUnitDead();
    virtual void onUnitChangeHp(float fChanged);
    virtual void onUnitTick(float dt);
    virtual void onUnitInterval();
    virtual void onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    virtual bool onUnitAttacked(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitDamagedDone(float fDamage, CUnit* pSource);
    virtual void onUnitDamageTargetDone(float fDamage, CUnit* pTarget);
    virtual void onUnitProjectileEffect(CProjectile* pProjectile, CUnit* pTarget);
    virtual bool onUnitProjectileArrive(CProjectile* pProjectile);
    virtual void onUnitAbilityEffect(CProjectile* pProjectile, CUnit* pTarget);  // no need to register this trigger
    
public:
    // 来自CUnit内部调用，bNotify为false时，不需要通知onUnitAddAbility，通常这种情况在Buff被覆盖的时候发生
    void onAddToUnit(CUnit* pOwner);
    void onDelFromUnit();
    
    M_SYNTHESIZE_READONLY(uint32_t, m_dwTriggerFlags, TriggerFlags);
    virtual void setTriggerFlags(uint32_t dwTriggerFlags);
    virtual void unsetTriggerFlags(uint32_t dwTriggerFlags);

    typedef vector<string> VEC_SOUNDS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_SOUNDS, m_vecEffectSounds, EffectSounds);
    void addEffectSound(const char* pSounds);
    virtual void playEffectSound();

    M_SYNTHESIZE_STR(ImageName);
    enum GRADE
    {
        kNormal,
        kRare,
        kEpic,
        kLegend
    };

    M_SYNTHESIZE(GRADE, m_iGrade, Grade);
    M_SYNTHESIZE(int, m_iCost, Cost);
    M_SYNTHESIZE_STR(Describe);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(vector<string>, m_vecLevelDescribe, LevelDescribes);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(vector<int>, m_vecLevelType, LevelTypes);
    const char* getLevelDescribe();
    const char* getLevelDescribe(int level);
    int getLevelType();
    int getLevelType(int level);
    void setLevelInfo(int level, int type, const char* desc);

    M_SYNTHESIZE_BOOL(Temporary);
};

class CActiveAbility : public CAbility
{
public:
    CActiveAbility(const char* pRootId, const char* pName, float fCoolDown, CCommandTarget::TARGET_TYPE eCastType = CCommandTarget::kNoTarget, uint32_t dwEffectiveTypeFlags = CUnitForce::kSelf | CUnitForce::kOwn | CUnitForce::kAlly | CUnitForce::kEnemy);
    virtual ~CActiveAbility();

    virtual CActiveAbility* copy() override;
    virtual void copyData(CAbility* from) override;
    
    static const float CONST_MAX_CAST_BUFFER_RANGE;
    static const float CONST_MAX_HOR_CAST_Y_RANGE;

    bool cast();  // 将不再被使用
    virtual bool checkConditions(const CCommandTarget& rTarget);
    virtual void onUnitCastAbility();  // 施法动作到达奏效点后触发

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

    const CPoint& getAbilityEffectPoint(CProjectile* pProjectile, CUnit* pTarget);

};

class CPassiveAbility : public CAbility
{
public:
    CPassiveAbility(const char* pRootId, const char* pName, float fCoolDown = 0);
    virtual ~CPassiveAbility();

    virtual CPassiveAbility* copy() override;

};

class CBuffAbility : public CAbility
{
public:
    CBuffAbility(const char* pRootId, const char* pName, float fDuration, bool bStackable);
    virtual CBuffAbility* copy() override;
    virtual void copyData(CAbility* from) override;

    virtual void onUnitDisplaceAbility();

    M_SYNTHESIZE(float, m_fDuration, Duration);
    M_SYNTHESIZE(float, m_fElapsed, Elapsed);
    virtual bool isDone() const;
    
    M_SYNTHESIZE_BOOL(Stackable);
    M_SYNTHESIZE(int, m_iSrcUnit, SrcUnit);

    M_SYNTHESIZE(int, m_iAppendBuff, AppendBuff);
    
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
    virtual CAttackAct* copy() override;
    
    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    virtual bool checkConditions(const CCommandTarget& rTarget) override;
    virtual void onUnitAbilityEffect(CProjectile* pProjectile, CUnit* pTarget) override;
        
    M_SYNTHESIZE_PASS_BY_REF(CAttackValue, m_oAttackValue, BaseAttack);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_aoExAttackValue, ExAttackValue);
    M_SYNTHESIZE(float, m_fAttackValueRandomRange, AttackValueRandomRange);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackValueRandomRange, ExAttackValueRandomRange);
    
    float getBaseAttackValue() const;
    float getRealAttackValue(bool bUseRandomRange = true) const;
    
    virtual float getRealCoolDown() const override;
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
    virtual CBuffMakerAct* copy() override;
    
    virtual bool checkConditions(const CCommandTarget& rTarget) override;
    virtual void onUnitAbilityEffect(CProjectile* pProjectile, CUnit* pTarget) override;
    
    M_SYNTHESIZE(float, m_fChance, Chance);
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);

    M_SYNTHESIZE_BOOL(EffectFixed);
    M_SYNTHESIZE_BOOL(EffectDead);
    
protected:
    CUnit* m_pTarget;
};

/////////////////////// PassiveAbilitys & BuffAbilitys ///////////////////////
// 光环，范围型BUFF附加器
class CAuraPas : public CPassiveAbility
{
public:
    CAuraPas(const char* pRootId, const char* pName, float fInterval, int iTemplateBuff, float fRange, uint32_t dwEffectiveTypeFlags, bool bEffectFixed);
    virtual ~CAuraPas();
    virtual CAuraPas* copy() override;
    
    virtual void onUnitInterval() override;
    
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    M_SYNTHESIZE(float, m_fRange, Range);
    M_SYNTHESIZE(uint32_t, m_dwEffectiveTypeFlags, TargetFlags);
    M_SYNTHESIZE_BOOL(EffectFixed);
    
    M_SYNTHESIZE(float, m_fEffectCD, EffectCD);
};

// 攻击数据变更，攻击时机会型BUFF附加器
class CAttackBuffMakerPas : public CPassiveAbility
{
public:
    CAttackBuffMakerPas(const char* pRootId, const char* pName, float fChance, int iTemplateBuff, bool bToSelf = false, const CExtraCoeff& roExAttackValue = CExtraCoeff(), int iTemplateAct = 0);
    virtual ~CAttackBuffMakerPas();
    
    virtual CAttackBuffMakerPas* copy() override;
    
    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    virtual void onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget) override;
    
    M_SYNTHESIZE(float, m_fChance, Chance);
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    M_SYNTHESIZE_BOOL(ToSelf);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackValue, ExAttackValue);

    M_SYNTHESIZE(int, m_iTemplateAct, TemplateAct);
    M_SYNTHESIZE_READONLY(CActiveAbility*, m_pInnerAct, ActiveAbility);
    bool castInnerSpell(CUnit* pTarget);
};

class CDamageBuff : public CBuffAbility
{
public:
    CDamageBuff(const char* pName, const CAttackValue& rDamage, float fChance, bool bToSelf = false, const CExtraCoeff& roExAttackValue = CExtraCoeff());
    virtual CDamageBuff* copy() override;

    virtual void onUnitAddAbility() override;

    M_SYNTHESIZE_PASS_BY_REF(CAttackValue, m_oDamage, Damage);
    M_SYNTHESIZE(float, m_fChance, Chance);
    M_SYNTHESIZE_BOOL(ToSelf);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackValue, ExAttackValue);
};

class CVampirePas : public CPassiveAbility
{
public:
    CVampirePas(const char* pRootId, const char* pName, float fPercentConversion);
    virtual CVampirePas* copy() override;
    
    virtual void onUnitDamageTargetDone(float fDamage, CUnit* pTarget) override;

    M_SYNTHESIZE(float, m_fPercentConversion, PercentConversion);
};

class CStunBuff : public CBuffAbility
{
public:
    CStunBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable);
    virtual CStunBuff* copy() override;
    
    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
};

class CDoubleAttackPas : public CAttackBuffMakerPas
{
public:
    CDoubleAttackPas(const char* pRootId, const char* pName, float fChance, const CExtraCoeff& roExAttackValue = CExtraCoeff());
    virtual CDoubleAttackPas* copy() override;
    
    virtual void onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget) override;
    
};

class CSpeedBuff : public CBuffAbility
{
public:
    CSpeedBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, const CExtraCoeff& roExMoveSpeedDelta, const CExtraCoeff& roExAttackSpeedDelta);
    virtual CSpeedBuff* copy() override;
    
    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExMoveSpeedDelta, ExMoveSpeedDelta);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExAttackSpeedDelta, ExAttackSpeedDelta);
    
};

// hp变化，CExTraCoeff是以MaxHp为基准的
class CChangeHpPas : public CPassiveAbility
{
public:
    CChangeHpPas(const char* pRootId, const char* pName, float fInterval, const CExtraCoeff& roChangeHp, const CExtraCoeff& roMinHp = CExtraCoeff(0.0, -1.0f));
    virtual CChangeHpPas* copy() override;

    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    virtual void onUnitInterval() override;

    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oChangeHp, ChangeHp);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oMinHp, MinHp);

};

class CChangeHpBuff : public CBuffAbility
{
public:
    CChangeHpBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fInterval, const CExtraCoeff& roChangeHp, const CExtraCoeff& roMinHp = CExtraCoeff(0.0, -1.0f));
    virtual CChangeHpBuff* copy() override;
    
    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    virtual void onUnitInterval() override;
    
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oChangeHp, ChangeHp);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oMinHp, MinHp);
    
};

// 重生
class CRebirthPas : public CPassiveAbility
{
public:
    CRebirthPas(const char* pRootId, const char* pName, float fCoolDown, const CExtraCoeff& rExMaxHp = CExtraCoeff(1.0f, 0.0f));
    virtual CRebirthPas* copy() override;

    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    virtual void onUnitDead() override;

    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExMaxHp, ExMaxHp)
    M_SYNTHESIZE_BOOL(RevivableBefore);
};

// 闪避
class CEvadePas : public CPassiveAbility
{
public:
    CEvadePas(const char* pRootId, const char* pName, float fChance, int iTemplateBuff = 0);
    virtual CEvadePas* copy() override;

    virtual bool onUnitAttacked(CAttackData* pAttack, CUnit* pSource) override;

    M_SYNTHESIZE(float, m_fChance, Chance);
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
};

// 闪避
class CEvadeBuff : public CBuffAbility
{
public:
    CEvadeBuff(const char* pRootId, const char* pName, float fDuration, bool bStackable, float fChance);
    virtual CEvadeBuff* copy() override;

    virtual bool onUnitAttacked(CAttackData* pAttack, CUnit* pSource) override;

    M_SYNTHESIZE(float, m_fChance, Chance);
};

// 闪电链
class CTransitiveLinkBuff : public CBuffAbility
{
public:
    typedef map<int, int> MAP_DAMAGED;

public:    
    CTransitiveLinkBuff(const char* pName, float fDuration, float fRange, int iMaxTimes, int iMinIntervalTimes, uint32_t dwEffectiveTypeFlags = CUnitForce::kEnemy);
    virtual CTransitiveLinkBuff* copy() override;
    virtual void copyData(CAbility* from) override;

    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    virtual void onUnitDead() override;

    void TransmitNext();

    M_SYNTHESIZE(float, m_fRange, Range);
    M_SYNTHESIZE(uint32_t, m_dwEffectiveTypeFlags, EffectiveTypeFlags);

    M_SYNTHESIZE(int, m_iMaxTimes, MaxTimes);
    M_SYNTHESIZE(int, m_iTimesLeft, TimesLeft);
    M_SYNTHESIZE(int, m_iFromUnit, FromUnit);
    M_SYNTHESIZE(int, m_iToUnit, ToUnit);

    M_SYNTHESIZE(int, m_iTemplateProjectile, TemplateProjectile);

    M_SYNTHESIZE(int, m_iMinIntervalTimes, OneMaxTimes);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_DAMAGED, m_mapTransmited, UnitsTransmited);

    bool checkConditions(CUnit* pUnit);

protected:
    bool m_bTransmited;
};

// 单位跳跃
class CTransitiveBlinkBuff : public CBuffAbility
{
public:
    typedef map<int, int> MAP_DAMAGED;

public:
    CTransitiveBlinkBuff(const char* pName, float fRange, int iMaxTimes, int iMinIntervalTimes, int iCastAnimation);
    virtual CTransitiveBlinkBuff* copy() override;
    virtual void copyData(CAbility* from) override;

    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    virtual void onUnitDead() override;

    void TransmitNext();

    M_SYNTHESIZE(float, m_fRange, Range);
    M_SYNTHESIZE(uint32_t, m_dwEffectiveTypeFlags, EffectiveTypeFlags);

    M_SYNTHESIZE(int, m_iMaxTimes, MaxTimes);
    M_SYNTHESIZE(int, m_iTimesLeft, TimesLeft);
    M_SYNTHESIZE(int, m_iFromUnit, FromUnit);
    M_SYNTHESIZE(int, m_iToUnit, ToUnit);

    M_SYNTHESIZE(int, m_iMinIntervalTimes, OneMaxTimes);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_DAMAGED, m_mapTransmited, UnitsTransmited);

    bool checkConditions(CUnit* pUnit);

    void souceUnitRunActions(bool bFirstTime);
    void resumeSourceUnit();

protected:
    bool m_bTransmited;
};

// 分裂
class CSplashPas : public CPassiveAbility
{
public:
    CSplashPas(const char* pName, float fNearRange, const CExtraCoeff& roExNearDamage, float fFarRange, const CExtraCoeff& roExFarDamage);
    virtual CSplashPas* copy() override;

    virtual void onUnitDamageTargetDone(float fDamage, CUnit* pTarget) override;

    M_SYNTHESIZE(float, m_fNearRange, NearRange);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExNearDamage, ExNearDamage);
    M_SYNTHESIZE(float, m_fFarRange, FarRange);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExFarDamage, ExFarDamage);

};

// 击退
class CKnockBackBuff : public CStunBuff
{
protected:
    static const int CONST_ACT_TAG;

public:
    CKnockBackBuff(const char* pRootId, const char* pName, float fDuration, float fDistance);
    virtual CKnockBackBuff* copy() override;

    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    virtual void onUnitDisplaceAbility() override;

    void knockBack();

    M_SYNTHESIZE(float, m_fDistance, Distance);

};

// 吸引
class CAttractBuff : public CStunBuff
{
protected:
    static const int CONST_ACT_TAG;

public:
    CAttractBuff(const char* pRootId, const char* pName, float fDuration, float fDistance);

    virtual CAttractBuff* copy() override;

    virtual void onUnitAddAbility() override;
    virtual void onUnitDelAbility() override;
    virtual void onUnitDisplaceAbility() override;

    void attract();

    M_SYNTHESIZE(float, m_fDistance, Distance);
};

// 反弹抛射物
class CReflectBuff : public CBuffAbility
{
public:
    CReflectBuff(const char* pRootId, const char* pName, float fDuration);
    virtual CReflectBuff* copy() override;

    virtual bool onUnitProjectileArrive(CProjectile* pProjectile) override;

};

// 时限生命
class CLimitedLifeBuff : public CBuffAbility
{
public:
    CLimitedLifeBuff(const char* pRootId, const char* pName, float fDuration);
    virtual CLimitedLifeBuff* copy() override;

    virtual void onUnitDelAbility() override;
};


#endif  /* __ABILITY_H__ */

