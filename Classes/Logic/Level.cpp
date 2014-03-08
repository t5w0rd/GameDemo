/* 
 * File:   Level.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午10:31
 */

#include "CommInc.h"
#include "Level.h"



// CLevelExp
CLevelExp::CLevelExp()
: m_iLvl(0)
, m_iMaxLvl(0)
, m_iExp(0)
, m_iMaxExp(0)
, m_pUpdate(NULL)
{
}

CLevelExp::~CLevelExp()
{
}

void CLevelExp::updateMaxExp()
{
    if (m_pUpdate)
    {
        m_pUpdate->updateMaxExp(this);
    }
    else
    {
        m_iMaxExp = m_iExp + 1;
    }
}

void CLevelExp::onChangeLevel(int32_t iChanged)
{
}

void CLevelExp::addLevel(int iLvl)
{
    setLevel(m_iLvl + iLvl);
}

void CLevelExp::addExp(int iExp)
{
    int iOldMaxExp;
    m_iExp += iExp;
    while (m_iExp >= m_iMaxExp)
    {
        ++m_iLvl;
        iOldMaxExp = m_iMaxExp;
        updateMaxExp();
        m_iExp -= iOldMaxExp;
        if (m_pUpdate)
        {
            m_pUpdate->onLevelChange(this, 1);
        }
        onChangeLevel(1);
    }
}

void CLevelExp::setLevel(int iLvl)
{
    int iOldLvl = m_iLvl;

    if (m_iLvl >= m_iMaxLvl)
    {
        m_iLvl = m_iMaxLvl;
    }
    else if (m_iLvl < 1)
    {
        m_iLvl = 1;
    }
    else
    {
        m_iLvl = iLvl;
    }

    int32_t iChanged = m_iLvl - iOldLvl;

    if (iChanged)
    {
        if (m_iLvl == m_iMaxLvl)
        {
            m_iExp = 0;
        }
        if (m_pUpdate)
        {
            m_pUpdate->onLevelChange(this, iChanged);
        }
        onChangeLevel(iChanged);
        updateMaxExp();
    }
}

void CLevelExp::setMaxLevel(int iMaxLvl)
{
    iMaxLvl <= 0 && (iMaxLvl = 1);
    m_iMaxLvl = iMaxLvl;
    setLevel(m_iLvl);
}

int CLevelExp::getLevel() const
{
    return m_iLvl;
}

int CLevelExp::getMaxLevel() const
{
    return m_iMaxLvl;
}

int CLevelExp::getExp() const
{
    return m_iExp;
}

int CLevelExp::getMaxExp() const
{
    return m_iMaxExp;
}

void CLevelExp::setLevelUpdate(CLevelUpdate* pUpdate)
{
    m_pUpdate = pUpdate;
}
