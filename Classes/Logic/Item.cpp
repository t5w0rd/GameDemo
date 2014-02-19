/* 
 * File:   Item.cpp
 * Author: thunderliu
 * 
 * Created on 2014年2月11日, 上午1:08
 */

#include "CommInc.h"
#include "Item.h"
#include "Unit.h"
#include "Skill.h"
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

void CItem::addActiveSkill(int id)
{
    m_vecActSkillIds.push_back(id);
}

void CItem::addPassiveSkill(int id)
{
    m_vecPasSkillIds.push_back(id);
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
        if (m_vecActSkills.front()->cast() == false)
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
        if (m_vecActSkills.empty())
        {
            // 无主动技能，直接返回
            return false;
        }
        
        if (m_vecActSkills.front()->getCastTargetType() != CCommandTarget::kNoTarget ||
            m_vecActSkills.front()->isCoolingDown() ||
            m_vecActSkills.front()->checkConditions() == false)
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
        addSkillToOwner(pOwner);
    }
    
    onUnitAddItem();
}

void CItem::onDelFromSlot()
{
    if (m_eItemType != kEquipment)
    {
        delSkillFromOwner();
    }
    
    onUnitDelItem();
    
    setOwner(NULL);
}

void CItem::addSkillToOwner(CUnit* pOwner, bool bNotify)
{
    CWorld* w = pOwner->getWorld();

    if (m_vecActSkills.empty())
    {
        for (auto it = m_vecActSkillIds.begin(); it != m_vecActSkillIds.end(); ++it)
        {
            CActiveSkill* pActSkill = NULL;
            w->copySkill(*it)->dcast(pActSkill);
            if (pActSkill != NULL)
            {
                m_vecActSkills.addObject(pActSkill);
            }
        }
    }
    
    M_VEC_FOREACH(m_vecActSkills)
    {
        CActiveSkill* pActSkill = M_VEC_EACH;
        pOwner->addActiveSkill(pActSkill, bNotify);
        M_VEC_NEXT;
    }
    
    if (m_vecPasSkills.empty())
    {
        for (auto it = m_vecPasSkillIds.begin(); it != m_vecPasSkillIds.end(); ++it)
        {
            CPassiveSkill* pPasSkill = NULL;
            w->copySkill(*it)->dcast(pPasSkill);
            if (pPasSkill != NULL)
            {
                m_vecPasSkills.addObject(pPasSkill);
            }
        }
    }
    
    M_VEC_FOREACH(m_vecPasSkills)
    {
        CPassiveSkill* pPasSkill = M_VEC_EACH;
        pOwner->addPassiveSkill(pPasSkill, bNotify);
        M_VEC_NEXT;
    }
}

void CItem::delSkillFromOwner(bool bNotify)
{
    CUnit* o = getOwner();
    
    M_VEC_FOREACH(m_vecActSkills)
    {
        CActiveSkill* pActSkill = M_VEC_EACH;
        o->delActiveSkill(pActSkill->getId(), bNotify);
        M_VEC_NEXT;
    }
    
    M_VEC_FOREACH(m_vecPasSkills)
    {
        CPassiveSkill* pPasSkill = M_VEC_EACH;
        o->delPassiveSkill(pPasSkill->getId(), bNotify);
        M_VEC_NEXT;
    }
}

