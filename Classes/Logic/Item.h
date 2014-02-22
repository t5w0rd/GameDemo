/* 
 * File:   Item.h
 * Author: thunderliu
 *
 * Created on 2014年2月11日, 上午1:08
 */

#ifndef __ITEM_H__
#define	__ITEM_H__


#include "MultiRefObject.h"
#include "Level.h"
#include "Base.h"
#include "Ability.h"


class CPackage;
class CAbility;
class CPassAbility;
class CActiveAbility;

class CItem : public CMultiRefObject
{
protected:
    const string CONST_ROOT_ID;
    
public:
    enum ITEM_TYPE
    {
        kNormal,
        kEquipment,
        kConsumable
    };
    
public:
    // uMaxStackSize可以取INFINITE，无限堆叠；eItemType为kConsumable时，uMaxStackSize可以取0，代表拾起立即消耗
    CItem(const char* pRootId, const char* pName, ITEM_TYPE eItemType, unsigned int uMaxStackSize);
    virtual ~CItem();

    virtual const char* getDbgTag() const;

    const char* getRootId() const;
    M_SYNTHESIZE_STR(Name);
    
    M_SYNTHESIZE(ITEM_TYPE, m_eItemType, ItemType);
    
    M_SYNTHESIZE_BOOL(Equipped);
    M_SYNTHESIZE(unsigned int, m_uStackCount, StackCount);
    M_SYNTHESIZE(unsigned int, m_uMaxStackSize, MaxStackSize);
    unsigned int getFreeStackSize() const;
    unsigned int incStackCount(unsigned int uIncrease);
    unsigned int decStatckCount(unsigned int uDecrease);
    
protected:
    typedef vector<int> VEC_ABILITYIDS;
    VEC_ABILITYIDS m_vecActAbilityIds;
    VEC_ABILITYIDS m_vecPasAbilityIds;
    
public:
    typedef CMultiRefVec<CActiveAbility*> VEC_ACT_ABILITYS;
    typedef CMultiRefVec<CPassiveAbility*> VEC_PAS_ABILITYS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_ACT_ABILITYS, m_vecActAbilitys, ActiveAbilitys);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_PAS_ABILITYS, m_vecPasAbilitys, PassiveAbilitys);
    void addActiveAbility(int id);
    void addPassiveAbility(int id);
    M_SYNTHESIZE(CUnit*, m_pOwner, Owner);
    
    // @overide
    virtual void onUnitAddItem();
    virtual void onUnitDelItem();
    
    // 调用完之后
    virtual bool use();
    virtual bool checkConditions();
    virtual void onUnitUseItem();
    
    // 外部调用
    void onAddToNewSlot(CUnit* pOwner);
    void onDelFromSlot();
    
protected:
    void addAbilityToOwner(CUnit* pOwner, bool bNotify = false);
    void delAbilityFromOwner(bool bNotify = false);
    
};


#endif	/* __ITEM_H__ */

