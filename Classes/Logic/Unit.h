/*
 * File:   Unit.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午10:55
 */

#ifndef __UNIT_H__
#define __UNIT_H__

#include "MultiRefObject.h"
#include "Level.h"
#include "Base.h"
#include "CommDef.h"
// 禁止在此处包含Unit.h文件


class CTypeValue
{
public:
    CTypeValue(int type, float value);
    M_SYNTHESIZE(int, m_iType, Type);
    M_SYNTHESIZE(float, m_fValue, Value);
    
    void set(int type, float value);
};

class CAttackValue : public CTypeValue
{
public:
    static const int CONST_MAX_ATTACK_TYPE = 4;
    enum ATTACK_TYPE
    {
        kPhysical,
        kMagical,
        kSiege,
        kHoly
    };
    
    static const int CONST_MAX_NAME_INDEX = 2;
    enum NAME_INDEX
    {
        kEnName = 0,
        kCnName = CONST_MAX_NAME_INDEX - 1
    };
    
    CAttackValue(int type = kPhysical, float value = 0.0f);
};

class CArmorValue : public CTypeValue
{
public:
    static const int CONST_MAX_ARMOR_TYPE = 5;
    enum ARMOR_TYPE
    {
        kHeavy,
        kCrystal,
        kWall,
        kHero,
        kHoly
    };
    
    static const int CONST_MAX_NAME_INDEX = 2;
    enum NAME_INDEX
    {
        kEnName = 0,
        kCnName = CONST_MAX_NAME_INDEX - 1
    };
    
    CArmorValue(int type = kHeavy, float value = 0.0f);
};


// 攻击数值，由多种类型的攻击组合而成
class CMultiAttackValue
{
public:
    static const char* CONST_ARR_NAME[CAttackValue::CONST_MAX_ATTACK_TYPE][CAttackValue::CONST_MAX_NAME_INDEX];
    typedef float ARR_ATTACK_VALUES[CAttackValue::CONST_MAX_ATTACK_TYPE];
    
public:
    CMultiAttackValue();
    CMultiAttackValue(int iCount, CAttackValue::ATTACK_TYPE eType1, float fValue1, ...);
    
    float getValue(CAttackValue::ATTACK_TYPE eType) const;
    void setValue(CAttackValue::ATTACK_TYPE eType, float fValue);
    void setAttackValue(const CMultiAttackValue& roAttackValue);
    void setValueZero();
    
    static const char* getName(CAttackValue::ATTACK_TYPE eType, int iIndex = 0);
    
    const CMultiAttackValue& operator=(const CMultiAttackValue& roAttackValue);
    
    ARR_ATTACK_VALUES m_afValues;
};

// 护甲数值，由多种类型的护甲组合而成
class CMultiArmorValue
{
public:
    static const char* CONST_ARR_NAME[CArmorValue::CONST_MAX_ARMOR_TYPE][CArmorValue::CONST_MAX_NAME_INDEX];
    typedef float ARR_ARMOR_VALUES[CArmorValue::CONST_MAX_ARMOR_TYPE];
    
public:
    CMultiArmorValue();
    CMultiArmorValue(int iCount, CArmorValue::ARMOR_TYPE eType1, float fValue1, ...);
    
    float getValue(CArmorValue::ARMOR_TYPE eType) const;
    void setValue(CArmorValue::ARMOR_TYPE eType, float fValue);
    void setArmorValue(const CMultiArmorValue& roArmorValue);
    void setValueZero();
    
    static const char* getName(CArmorValue::ARMOR_TYPE eType, int iIndex = 0);
    
    const CMultiArmorValue& operator=(const CMultiArmorValue& roArmorValue);
    
    ARR_ARMOR_VALUES m_afValues;
};

// 攻击-护甲计算系数
extern float g_afArmorAttackTable[CArmorValue::CONST_MAX_ARMOR_TYPE][CAttackValue::CONST_MAX_ATTACK_TYPE];

class CAttackBuff
{
public:
    CAttackBuff(int iTemplateBuff, int iBuffLevel);
    
    M_SYNTHESIZE(int, m_iTemplateBuff, TemplateBuff);
    M_SYNTHESIZE(int, m_iBuffLevel, BuffLevel);
};

class CAttackData : public CMultiRefObject
{
public:
    typedef vector<CAttackBuff> VEC_ATTACK_BUFF;
    
public:
    CAttackData();
    virtual CAttackData* copy() override;

    M_SYNTHESIZE_PASS_BY_REF(CAttackValue, m_oAtkVal, AttackValue);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_ATTACK_BUFF, m_vecAtkBuffs, AttackBuffs);
    
    void setAttackValue(int eAttackType, float fAttackValue);
    void addAttackBuff(const CAttackBuff& rAttackBuff);
};

// f = ax + b，x为基准值，a和b分为计算乘数和加数，应用情形：触发重击将造成自身攻击力的2倍并额外附加30点的伤害；提升自身20%的基础力量值
// 应用该接结构，可轻易解决装备武器/新增BUFF，卸载武器/删除BUFF后的属性一致
class CExtraCoeff
{
public:
    CExtraCoeff(float fMulriple = 1.0f, float fAddend = 0.0f);
    
    float getValue(float fBase) const;
    
    M_SYNTHESIZE(float, m_fMulriple, Mulriple);
    M_SYNTHESIZE(float, m_fAddend, Addend);
};

class CUnitForce
{
public:
    enum EFFECTIVE_TARGET_FLAG_BIT
    {
        kSelf = 1 << 0,
        kOwn = 1 << 1,
        kAlly = 1 << 2,
        kEnemy = 1 << 3,
    };
    
public:
    CUnitForce();
    
    // it ONLY means that force is my ally
    bool isMyAlly(const CUnitForce* pForce) const;

    // it ONLY means that force is my enemy
    bool isMyEnemy(const CUnitForce* pForce) const;
    
    // the result of 'this->canEffect(force)' is not always same as 'force->canEffect(this)'
    // it ONLY means that 'this' can effect 'force'
    bool canEffect(const CUnitForce* pForce, uint32_t dwEffectiveTypeFlags) const;
    
    void setForceByIndex(int iForceIndex);
    
    M_SYNTHESIZE(uint32_t, m_dwForceFlag, Force);
    M_SYNTHESIZE(uint32_t, m_dwAllyMaskFlag, Ally);
};


class CAction;
class CUnit;

class CActionManager
{
public:
    typedef CMultiRefVec<CAction*> VEC_ACTIONS;
    
public:
    CActionManager();
    
    void addAction(CAction* pAction, CUnit* pTarget);
    CAction* getActionByTag(int iTag);
    
    virtual void onTick(float dt);
    
protected:
    VEC_ACTIONS m_vecActions;
};


class CProjectile;
class CAbility;
class CPassiveAbility;
class CBuffAbility;
class CActiveAbility;
class CWorld;
class CItem;

class CCommandTarget
{
public:
    enum TARGET_TYPE
    {
        kNoTarget = 0,
        kUnitTarget,
        kPointTarget
    };
    
public:
    explicit CCommandTarget();
    explicit CCommandTarget(int iTargetUnit);
    explicit CCommandTarget(const CPoint& rTargetPoint);

    bool operator==(const CCommandTarget& rTarget) const;
    
    M_SYNTHESIZE(TARGET_TYPE, m_eTargetType, TargetType);
    M_SYNTHESIZE_PASS_BY_REF(CPoint, m_oTargetPoint, TargetPoint);
    M_SYNTHESIZE(int, m_iTargetUnit, TargetUnit);

    void setTarget();
    void setTarget(int iTargetUnit);
    void setTarget(const CPoint& rTargetPoint);
};

class CUnitEventAdapter : public CMultiRefObject
{
public:
    inline CUnitEventAdapter() { setDbgClassName("CUnitEventAdapter"); }
    inline virtual ~CUnitEventAdapter() {}
    inline virtual CMultiRefObject* copy() { retain(); return this; }
    
    inline virtual void onUnitChangeLevel(CUnit* pUnit, int iChanged) {}
    inline virtual void onUnitRevive(CUnit* pUnit) {}
    inline virtual void onUnitDying(CUnit* pUnit) {}
    inline virtual void onUnitDead(CUnit* pUnit) {}
    inline virtual void onUnitChangeHp(CUnit* pUnit, float fChanged) {}
    inline virtual void onUnitTick(CUnit* pUnit, float dt) {}
    inline virtual void onUnitAttackTarget(CUnit* pUnit, CAttackData* pAttack, CUnit* pTarget) {}
    inline virtual bool onUnitAttacked(CUnit* pUnit, CAttackData* pAttack, CUnit* pSource) { return true; }
    inline virtual void onUnitDamaged(CUnit* pUnit, CAttackData* pAttack, CUnit* pSource) {}
    inline virtual void onUnitDamagedDone(CUnit* pUnit, float fDamage, CUnit* pSource) {}
    inline virtual void onUnitDamageTargetDone(CUnit* pUnit, float fDamage, CUnit* pTarget) {}
    inline virtual void onUnitProjectileEffect(CUnit* pUnit, CProjectile* pProjectile, CUnit* pTarget) {}
    inline virtual bool onUnitProjectileArrive(CUnit* pUnit, CProjectile* pProjectile) { return true; }
    inline virtual void onUnitAddActiveAbility(CUnit* pUnit, CActiveAbility* pAbility) {}
    inline virtual void onUnitDelActiveAbility(CUnit* pUnit, CActiveAbility* pAbility) {}
    inline virtual void onUnitAddPassiveAbility(CUnit* pUnit, CPassiveAbility* pAbility) {}
    inline virtual void onUnitDelPassiveAbility(CUnit* pUnit, CPassiveAbility* pAbility) {}
    inline virtual void onUnitAddBuffAbility(CUnit* pUnit, CBuffAbility* pAbility) {}
    inline virtual void onUnitDelBuffAbility(CUnit* pUnit, CBuffAbility* pAbility) {}
    inline virtual void onUnitAbilityCD(CUnit* pUnit, CAbility* pAbility) {}
    inline virtual void onUnitAbilityReady(CUnit* pUnit, CAbility* pAbility) {}
    inline virtual void onUnitAddItem(CUnit* pUnit, int iIndex) {}
    inline virtual void onUnitDelItem(CUnit* pUnit, int iIndex) {}
    //inline virtual void onUnitChangeItemStackCount(CUnit* pUnit, CItem* pItem, int iChange) {}
};

class CBaseAI : public CUnitEventAdapter
{
public:
    CBaseAI();
    virtual void onUnitTick(CUnit* pUnit, float dt);
    virtual void onUnitDamagedDone(CUnit* pUnit, float fDamage, CUnit* pSource);
    M_SINGLETON(CBaseAI);
};

class CUnitAI : public CBaseAI
{
public:
    CUnitAI();
    virtual ~CUnitAI();

    virtual void onUnitChangeHp(CUnit* pUnit, float fChanged);
    virtual void onUnitTick(CUnit* pUnit, float dt);
    virtual void onUnitDamagedDone(CUnit* pUnit, float fDamage, CUnit* pSource);
    virtual void onUnitDamageTargetDone(CUnit* pUnit, float fDamage, CUnit* pTarget);
    virtual void onUnitAddBuffAbility(CUnit* pUnit, CBuffAbility* pAbility);
    virtual void onUnitDelBuffAbility(CUnit* pUnit, CBuffAbility* pAbility);
    virtual void onUnitAbilityReady(CUnit* pUnit, CAbility* pAbility);
};

class CUnitDraw;
class CForceResource;
class CUnit : public CLevelExp, public CUnitForce
{
protected:
    const string CONST_ROOT_ID;

public:
    CUnit(CUnitDraw* draw);
    virtual ~CUnit();
    virtual CUnit* copy() override;
    virtual void copyData(const CUnit* from);

    virtual const char* getDbgTag() const;

    M_SYNTHESIZE(CWorld*, m_pWorld, World);
    
    CUnit* getUnit(int id);
    void abilityCD(CAbility* pAbility);
    void updateAbilityCD(int id);
    
    M_SYNTHESIZE_STR(Name);
    
    bool revive(float fHp);
    bool setHp(float fHp);
    void setMaxHp(float fMaxHp);
    bool isDead() const;
    
    M_SYNTHESIZE_READONLY(float, m_fHp, Hp);
    M_SYNTHESIZE_READONLY(float, m_fMaxHp, MaxHp);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CExtraCoeff, m_oExMaxHp, ExMaxHp);
    void setExMaxHp(const CExtraCoeff& var);
    float getRealMaxHp() const;
    
    
    // @override
    
    // 等级变化时被通知，在通过addExp升级的时候，通常来讲iChanged总是为1，尽管经验有时会足够多以至于连升2级
    virtual void onChangeLevel(int iChanged);
    // 复活时被通知
    virtual void onRevive();
    // 死亡时被通知
    virtual void onDying();
    // 死亡后被通知
    virtual void onDead();
    // 血量变化时被通知
    virtual void onChangeHp(float fChanged);
    // 每个游戏刻被通知
    virtual void step(float dt);
    virtual void onTick(float dt);
    // 攻击发出时，攻击者被通知
    virtual void onAttackTarget(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask);
    // 攻击抵达时，受害者被通知
    virtual bool onAttacked(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask);
    // 攻击命中时，受害者被通知
    virtual void onDamaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask);
    // 攻击命中时，受害者被通知
    virtual void onDamagedDone(float fDamage, CUnit* pSource, uint32_t dwTriggerMask);
    // 攻击命中时，攻击者被通知
    virtual void onDamageTargetDone(float fDamage, CUnit* pTarget, uint32_t dwTriggerMask);
    // 攻击数据消除时被通知，通常由投射物携带攻击数据，二者生存期一致
    virtual void onProjectileEffect(CProjectile* pProjectile, CUnit* pTarget);
    
    virtual bool onProjectileArrive(CProjectile* pProjectile);
    
    virtual void onAddActiveAbility(CActiveAbility* pAbility);
    virtual void onDelActiveAbility(CActiveAbility* pAbility);
    virtual void onAddPassiveAbility(CPassiveAbility* pAbility);
    virtual void onDelPassiveAbility(CPassiveAbility* pAbility);
    virtual void onAddBuffAbility(CBuffAbility* pAbility);
    virtual void onDelBuffAbility(CBuffAbility* pAbility);
    
    // 技能CD开始时被通知
    virtual void onAbilityCD(CAbility* pAbility);  // 以后将区分出onItemCD
    // 技能CD结束时被通知
    virtual void onAbilityReady(CAbility* pAbility);  // 以后将区分出onItemReady
    
    virtual void onAddItem(int iIndex);
    virtual void onDelItem(int iIndex);
    //virtual void onChangeItemStackCount(CItem* pItem, int iChange);
    
    M_SYNTHESIZE_READONLY(CBaseAI*, m_pAI, AI);
    void setAI(CBaseAI* pAI);

    M_SYNTHESIZE_READONLY(CUnitEventAdapter*, m_pEventAdapter, EventAdapter);
    void setEventAdapter(CUnitEventAdapter* pEventAdapter);

    ////////////////////////  trigger /////////////////    
    enum TRIGGER_FLAG_BIT
    {
        kOnReviveTrigger = 1 << 0,
        kOnDyingTrigger = 1 << 1,
        kOnDeadTrigger = 1 << 2,
        kOnChangeHpTrigger = 1 << 3,
        kOnTickTrigger = 1 << 4,
        kOnAttackTargetTrigger = 1 << 5,
        kOnAttackedTrigger = 1 << 6,
        kOnDamagedSurfaceTrigger = 1 << 7,
        kOnDamagedInnerTrigger = 1 << 8,
        kOnDamagedDoneTrigger = 1 << 9,
        kOnDamageTargetDoneTrigger = 1 << 10,
        kOnProjectileEffectTrigger = 1 << 11,
        kOnProjectileArriveTrigger = 1 << 12,
        kOnCalcDamageTargetTrigger = 1 << 13
    };
    
    enum TRIGGER_MASK
    {
        kNoMasked = 0,
        kMaskAll = 0xFFFFFFFF,
        kMaskActiveTrigger = kOnAttackTargetTrigger | kOnDamageTargetDoneTrigger
    };
    
    
    //////////////////// attack & damaged ////////////////////////
    
    // 高层攻击函数，用于最初生成攻击数据，一个攻击动作生成的攻击数据，一般调用该函数
    // 攻击动作，可对目标造成动作，如普通攻击、技能等
    // 攻击数据，描述这次攻击的数据体，详见 CAttackData 定义
    // 内部会自行调用中层、底层攻击函数，对攻击数据进行传递并处理，通常返回处理后的攻击数据，也可以返回 nullptr
    // 内部会根据人物属性对攻击数据进行一次变换，如力量加成等
    // 触发 onAttackTarget，
    void attack(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);
    
    // 底层攻击函数，目前无逻辑，只是将传递过来的攻击数据返回给上层
    void attackLow(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);
    
    // 高层伤害函数，攻击者生成的攻击到达目标后，目标将调用该函数，计算自身伤害
    // 内部会对攻击数据进行向下传递
    // 触发 onAttacked，如果onAttacked返回 nullptr，伤害将不会继续向下层函数传递，函数返回false。比如说，闪避成功，伤害无需继续计算
    // 触发 onDamaged
    // 遍历攻击数据携带的BUFF链，根据附着概率对单位自身进行BUFF附加
    // 根据单位属性，进行攻击数据变换，如抗性对攻击数据的影响
    // 根据单位护甲，进行攻击数据中的攻击数值变换
    bool damaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);
    
    // 底层伤害函数，直接扣除指定量的HP值
    // 触发伤害源的 onDamaeTarget
    // 调用 setHp，从而会触发 onChangeHp，可能会触发onDying
    void damagedLow(float fDamage, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);
    
    float calcDamage(CAttackValue::ATTACK_TYPE eAttackType, float fAttackValue, CArmorValue::ARMOR_TYPE eArmorType, float fArmorValue);
    
    
    typedef CMultiRefMap<CActiveAbility*> MAP_ACTIVE_ABILITIES;
    typedef CMultiRefMap<CPassiveAbility*> MAP_PASSIVE_ABILITIES;
    typedef CMultiRefMap<CBuffAbility*> MAP_BUFF_ABILITIES;
    typedef CMultiRefMap<CPassiveAbility*> MAP_SYSTEM_ABILITIES;
    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ACTIVE_ABILITIES, m_mapActAbilities, ActiveAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_PASSIVE_ABILITIES, m_mapPasAbilities, PassiveAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_BUFF_ABILITIES, m_mapBuffAbilities, BuffAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_SYSTEM_ABILITIES, m_mapSysAbilities, SystemAbilities);


    M_SYNTHESIZE(int, m_iAttackAbilityId, AttackAbilityId);
    // 下列函数将安全的增删触发器
    
    void addActiveAbility(CActiveAbility* pAbility, bool bNotify = true);
    void addActiveAbility(int id, int iLevel = 1, bool bNotify = true);
    void delActiveAbility(int id, bool bNotify = true);
    CActiveAbility* getActiveAbility(int id);
    CActiveAbility* getActiveAbility(const char* name);
    
    void addPassiveAbility(CPassiveAbility* pAbility, bool bNotify = true);
    void addPassiveAbility(int id, int iLevel = 1, bool bNotify = true);
    void delPassiveAbility(int id, bool bNotify = true);
    CPassiveAbility* getPassiveAbility(int id);
    
    void addBuffAbility(CBuffAbility* pAbility, bool bNotify = true);
    void addBuffAbility(int id, int iSrcUnit, int iLevel = 1, bool bNotify = true);
    void delBuffAbility(int id, bool bNotify = true);
    CBuffAbility* getBuffAbility(int id);
    CBuffAbility* getBuffAbility(const char* name);

    void addSystemAbility(CPassiveAbility* pAbility);
    void delSystemAbility(const char* name);
    
protected:
    void updateBuffAbilityElapsed(float dt);
    
public:
    typedef CMultiRefMap<CAbility*> MAP_TRIGGER_ABILITIES;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnAttackTargetTriggerAbilities, OnAttackTargetTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnAttackedTriggerAbilities, OnAttackedTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnDamagedSurfaceTriggerAbilities, OnDamagedSurfaceTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnDamagedInnerTriggerAbilities, OnDamagedInnerTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnDamagedDoneTriggerAbilities, OnDamagedDoneTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnDamageTargetDoneTriggerAbilities, OnDamageTargetDoneTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnChangeHpTriggerAbilities, OnChangeHpTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnReviveTriggerAbilities, OnReviveTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnDyingTriggerAbilities, OnDyingTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnDeadTriggerAbilities, OnDeadTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnTickTriggerAbilities, OnTickTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnProjectileEffectTriggerAbilities, OnProjectileEffectTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnProjectileArriveTriggerAbilities, OnProjectileArriveTriggerAbilities);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapOnCalcDamageTargetTriggerAbilities, OnCalcDamageTargetTriggerAbilities);
    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapTriggerAbilitiesToAdd, TriggerAbilitiesToAdd);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_TRIGGER_ABILITIES, m_mapTriggerAbilitiesToDel, TriggerAbilitiesToDel);
    
public:
    // 添加触发器
    void addAbilityToTriggers(CAbility* pAbility);
    
    // 删除触发器
    void delAbilityFromTriggers(CAbility* pAbility);
    
protected:
    // 只能在triggerFree的时候调用
    void updateTriggerAbilitiesWhenTriggerFree();
    
    // trigger之间是有可能存在嵌套关系的
    // 为了安全增删trigger，需要维护一个引用计数
    int m_iTriggerRefCount;
    void beginTrigger();
    void endTrigger();
    bool isTriggerFree() const;
    
    // 触发器链的触发，内部调用
    void triggerOnRevive();
    void triggerOnDying();
    void triggerOnDead();
    void triggerOnChangeHp(float fChanged);
    void triggerOnTick(float dt);
    void triggerOnAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    bool triggerOnAttacked(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedSurface(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedInner(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedDone(float fDamage, CUnit* pSource);
    void triggerOnDamageTargetDone(float fDamage, CUnit* pTarget);
    void triggerOnProjectileEffect(CProjectile* pProjectile, CUnit* pTarget);
    bool triggerOnProjectileArrive(CProjectile* pProjectile);
    void triggerOnCalcDamageTarget(float fDamage, CUnit* pTarget);
    
    // 为单位添加/删除技能
    //void addAbility(CAbility* pAbility);
    //void delAbility(CAbility* pAbility);
    
    // 为单位添加/删除/覆盖删除BUFF
    //void addBuff(CBuffAbility* pBuff, bool bForce = false);
    //void delBuff(CBuffAbility* pBuff, bool bAfterTriggerLoop = true);
    //void coverBuff(CBuffAbility* pBuff);
        
public:
    M_SYNTHESIZE_READONLY(int, m_iSuspendRef, SuspendRef);
    virtual bool isSuspended() const;
    virtual void suspend();
    virtual void resume();
    
    M_SYNTHESIZE_PASS_BY_REF(CArmorValue, m_oBaseArmor, BaseArmor);
    M_SYNTHESIZE_PASS_BY_REF(CExtraCoeff, m_oExArmorValue, ExArmorValue)
    float getRealArmorValue() const;
    
    M_SYNTHESIZE_BOOL(Revivable);

protected:
    int m_iGhostOwner;

public:
    void setGhost(int iGhostOwner);
    bool isGhost() const;
    int getGhostOwner() const;

    M_SYNTHESIZE_READONLY(CForceResource*, m_pResource, Resource);
    void setResource(CForceResource* var);
    M_SYNTHESIZE(int, m_iRewardGold, RewardGold);
    M_SYNTHESIZE(int, m_iRewardExp, RewardExp);
    
    ///////////////////////// item //////////////////////
    typedef CMultiRefVec<CItem*> VEC_ITEMS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_ITEMS, m_vecItems, Items);
    void setPackageSize(int iSize);
    bool addItem(CItem* pItem);
    void delItem(int iIndex);
    CItem* getItem(int iIndex);
    
    virtual int useItem(int iIndex);
    /////////////////////// doing - begin ////////////////////////////
    enum DOING_FLAG_BIT
    {
        kDying = 1 << 16,
        kMoving = 1 << 17,
        kObstinate = 1 << 18,
        kAlongPath = 1 << 19,
        kCasting = 1 << 20,
        kSpinning = 1 << 21
    };
    
    M_SYNTHESIZE_READONLY(uint32_t, m_dwDoingFlags, DoingFlags);
    void startDoing(uint32_t dwFlags);
    void endDoing(uint32_t dwFlags);
    bool isDoingOr(uint32_t dwFlags) const;
    bool isDoingAnd(uint32_t dwFlags) const;
    bool isDoingNothing() const;
    M_SYNTHESIZE(int, m_iPriority, Priority);
    bool tryDoing(int priority);
    
    // --------------- Action ----------------

    M_SYNTHESIZE_READONLY(CUnitDraw*, m_pDraw, Draw);
    virtual void setDraw(CUnitDraw* pDraw);

protected:
    CActionManager m_oActMgr;
    
public:
    enum ACTION_TAG
    {
        kActMoveTo,
        kActMove,
        kActAttack,
        kActAttackEffect,
        kActDying,
        kActCast,
        kActCastEffect,
        kActSpin
    };
    
    void runAction(CAction* pAction);
    CAction* getActionByTag(int iTag);
    
    enum UNIT_SPECIES
    {
        kUnknown = 0,
        kHuman,
        kBeast,
        kMechanical,
        kGod,
        kDemon
    };
    
    M_SYNTHESIZE(UNIT_SPECIES, m_eSpecies, Species);

    M_SYNTHESIZE(int, m_iEnergy, Energy);
    
};

class CWorld : public CUnitEventAdapter
{
public:
    CWorld();
    virtual ~CWorld();
    
    M_SYNTHESIZE(int, m_iScriptHandler, ScriptHandler);
    
    virtual bool onInit();
    virtual void onTick(float dt);
    virtual void onAddUnit(CUnit* pUnit);
    virtual void onDelUnit(CUnit* pUnit);
    virtual void onAddProjectile(CProjectile* pProjectile);
    virtual void onDelProjectile(CProjectile* pProjectile);

    bool init();

    M_SYNTHESIZE(int, m_iControlUnit, ControlUnit);
    
    typedef CMultiRefMap<CUnit*> MAP_UNITS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_UNITS, m_mapUnits, Units);
    void addUnit(CUnit* pUnit);
    void delUnit(int id, bool bRevivable = false);
    CUnit* getUnit(int id) const;
    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_UNITS, m_mapUnitsToRevive, UnitsToRevive);
    void reviveUnit(int id, float fHp);
    CUnit* getUnitToRevive(int id);
    
    typedef CMultiRefMap<CAbility*> MAP_ABILITIES;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ABILITIES, m_mapAbilitiesCD, AbilitiesCD);
    void addAbilityCD(CAbility* pAbility);
    void delAbilityCD(int id);
    bool isAbilityCD(int id) const;
    CAbility* getAbilityCD(int id) const;
    void updateAbilityCD(int id);
    virtual void onAddNormalAttributes(CUnit* pUnit);
    virtual void onDelNormalAttributes(CUnit* pUnit);
    
protected:
    void cleanAbilitiesCD(CUnit* pUnit);
    void abilityReady(CAbility* pAbility);
    
public:
    virtual void step(float dt);

    int addTemplateAbility(CAbility* pAbility);
    int addTemplateAbility(int id, CAbility* pAbility);
    virtual CAbility* copyAbility(int id) const;

    typedef CMultiRefMap<CProjectile*> MAP_PROJECTILES;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_PROJECTILES, m_mapTemplateProjectiles, TemplateProjectiles);
    int addTemplateProjectile(CProjectile* pProjectile);
    virtual CProjectile* copyProjectile(int id) const;

    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_PROJECTILES, m_mapProjectiles, Projectiles);
    void addProjectile(CProjectile* pProjectile);
    void delProjectile(int id);

    virtual void shutdown();

protected:
    bool m_bShutdown;
};

class CForceResource : public CMultiRefObject, public CUnitForce
{
public:
    CForceResource(CMultiRefObject* pSender, FUNC_CALLFUNC_N pFunc);

    M_SYNTHESIZE(int, m_iGold, Gold);
    void changeGold(int iChange);
    virtual void onGoldChange(int iChange);

    M_SYNTHESIZE(CMultiRefObject*, m_pSender, Sender);
    M_SYNTHESIZE(FUNC_CALLFUNC_N, m_pCallback, Callback);
};




// ----------- Inline Implementation--------------


#endif  /* __UNIT_H__ */

