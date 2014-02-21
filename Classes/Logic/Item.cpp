/* 
 * File:   Item.cpp
 * Author: thunderliu
 * 
 * Created on 2014年2月11日, 上午1:08
 */

#include "CommInc.h"
#include "Item.h"
#include "Unit.h"
#include "Ability.h"
#include "Application.h"


// CItem
CItem::CItem(const char* pRootId, const char* pName, ITEM_TYPE eItemType, unsigned int uMaxStackSize)
: CONST_ROOT_ID(pRootId)
, m_sName(pName)
, m_eItemType(eItemType)
, m_bEquipped(false)
, m_uStackCount(1)
, m_uMaxStackSize(uMaxStackSize)
, m_pOwner(NULL)
{
    setDbgClassName("CItem");
}

CItem::~CItem()
{
}

const char* CItem::getDbgTag() const
{
    return getName();
}

const char* CItem::getRootId() const
{
    return CONST_ROOT_ID.c_str();
}

unsigned int CItem::getFreeStackSize() const
{
    return max(m_uMaxStackSize - m_uStackCount, (unsigned int)0);
}

unsigned int CItem::incStackCount(unsigned int uIncrease)
{
    unsigned int uRet = min(uIncrease, getFreeStackSize());
    m_uStackCount += uRet;
    return uRet;
}

unsigned int CItem::decStatckCount(unsigned int uDecrease)
{
    unsigned int uRet = min(uDecrease, m_uStackCount);
    m_uStackCount -= uRet;
    return uRet;
}

void CItem::addActiveAbility(int id)
{
    m_vecActAbilityIds.push_back(id);
}

void CItem::addPassiveAbility(int id)
{
    m_vecPasAbilityIds.push_back(id);
}

void CItem::onUnitAddItem()
{
}

void CItem::onUnitDelItem()
{
}

bool CItem::use()
{
    if (checkConditions() == false)
    {
        return false;
    }
    
    CUnit* o = getOwner();

    if (m_eItemType == kEquipment)
    {
        // 属于装备，使用代表进行装备
        // TODO: 装备上
    }
    else
    {
        // 直接可以使用
        LOG("%s使用了%s", o->getName(), getName());
        if (m_vecActAbilitys.front()->cast() == false)
        {
            return false;
        }
        
        onUnitUseItem();
        
        if (m_eItemType == kConsumable)
        {
            // 是消耗品
            decStatckCount(1);
        }
    }
    
    return true;
}

bool CItem::checkConditions()
{
    CUnit* o = getOwner();
    if (o == NULL)
    {
        return false;
    }
    
    if (m_eItemType == kEquipment)
    {
        // 属于装备，使用代表进行装备
        // TODO: 装备上
    }
    else
    {
        // 直接可以使用
        if (m_vecActAbilitys.empty())
        {
            // 无主动技能，直接返回
            return false;
        }
        
        if (m_vecActAbilitys.front()->getCastTargetType() != CCommandTarget::kNoTarget ||
            m_vecActAbilitys.front()->isCoolingDown() ||
            m_vecActAbilitys.front()->checkConditions() == false)
        {
            return false;
        }
    }
    
    return true;
}

void CItem::onUnitUseItem()
{
}

void CItem::onAddToNewSlot(CUnit* pOwner)
{
    setOwner(pOwner);
    
    if (m_eItemType != kEquipment)
    {
        addAbilityToOwner(pOwner);
    }
    
    onUnitAddItem();
}

void CItem::onDelFromSlot()
{
    if (m_eItemType != kEquipment)
    {
        delAbilityFromOwner();
    }
    
    onUnitDelItem();
    
    setOwner(NULL);
}

void CItem::addAbilityToOwner(CUnit* pOwner, bool bNotify)
{
    CWorld* w = pOwner->getWorld();

    if (m_vecActAbilitys.empty())
    {
        for (auto it = m_vecActAbilityIds.begin(); it != m_vecActAbilityIds.end(); ++it)
        {
            CActiveAbility* pActAbility = NULL;
            w->copyAbility(*it)->dcast(pActAbility);
            if (pActAbility != NULL)
            {
                m_vecActAbilitys.addObject(pActAbility);
            }
        }
    }
    
    M_VEC_FOREACH(m_vecActAbilitys)
    {
        CActiveAbility* pActAbility = M_VEC_EACH;
        pOwner->addActiveAbility(pActAbility, bNotify);
        M_VEC_NEXT;
    }
    
    if (m_vecPasAbilitys.empty())
    {
        for (auto it = m_vecPasAbilityIds.begin(); it != m_vecPasAbilityIds.end(); ++it)
        {
            CPassiveAbility* pPasAbility = NULL;
            w->copyAbility(*it)->dcast(pPasAbility);
            if (pPasAbility != NULL)
            {
                m_vecPasAbilitys.addObject(pPasAbility);
            }
        }
    }
    
    M_VEC_FOREACH(m_vecPasAbilitys)
    {
        CPassiveAbility* pPasAbility = M_VEC_EACH;
        pOwner->addPassiveAbility(pPasAbility, bNotify);
        M_VEC_NEXT;
    }
}

void CItem::delAbilityFromOwner(bool bNotify)
{
    CUnit* o = getOwner();
    
    M_VEC_FOREACH(m_vecActAbilitys)
    {
        CActiveAbility* pActAbility = M_VEC_EACH;
        o->delActiveAbility(pActAbility->getId(), bNotify);
        M_VEC_NEXT;
    }
    
    M_VEC_FOREACH(m_vecPasAbilitys)
    {
        CPassiveAbility* pPasAbility = M_VEC_EACH;
        o->delPassiveAbility(pPasAbility->getId(), bNotify);
        M_VEC_NEXT;
    }
}

