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
: m_iLvl(1)
, m_iMaxLvl(1)
, m_iExp(0)
, m_iBaseExp(0)
, m_iMaxExp(1)
, m_pUpdate(NULL)
{
}

CLevelExp::~CLevelExp()
{
}

void CLevelExp::updateExpRange()
{
    if (m_pUpdate)
    {
        m_pUpdate->updateExpRange(this);
    }
    else
    {
        m_iBaseExp = m_iExp;
        m_iMaxExp = m_iExp * 1.5;
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
    if (m_iLvl == m_iMaxLvl)
    {
        return;
    }

    m_iExp += iExp;
    while (m_iExp >= m_iMaxExp && m_iLvl < m_iMaxLvl)
    {
        ++m_iLvl;
        updateExpRange();
        if (m_pUpdate)
        {
            m_pUpdate->onChangeLevel(this, 1);
        }
        onChangeLevel(1);
    }

    if (m_iLvl == m_iMaxLvl)
    {
        m_iExp = m_iBaseExp;
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
            m_pUpdate->onChangeLevel(this, iChanged);
        }
        onChangeLevel(iChanged);
        updateExpRange();
    }
}

void CLevelExp::setMaxLevel(int iMaxLvl)
{
    iMaxLvl <= 0 && (iMaxLvl = 1);
    m_iMaxLvl = iMaxLvl;
    setLevel(m_iLvl);
}

void CLevelExp::setLevelUpdate(CLevelUpdate* pUpdate)
{
    if (pUpdate == m_pUpdate)
    {
        return;
    }

    if (pUpdate == NULL)
    {
        delete m_pUpdate;
    }
    else
    {
    }
    m_pUpdate = pUpdate;
}

bool CLevelExp::canIncreaseExp() const
{
    return m_iLvl < m_iMaxLvl;
}
