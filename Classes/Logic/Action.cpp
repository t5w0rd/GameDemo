/* 
 * File:   Action.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月10日, 下午11:15
 */

#include "CommInc.h"
#include "Action.h"
#include "Unit.h"
#include "Draw.h"


// CAction
CAction::CAction()
: m_pTarget(NULL)
, m_iTag(0)
{
    setDbgClassName("CAction");
}


CAction::~CAction()
{
}

void CAction::start(CUnit* pTarget)
{
    m_pTarget = pTarget;
}

void CAction::stop()
{
    m_pTarget = NULL;
}

void CAction::step(float dt)
{
}

void CAction::update(float time)
{
}

bool CAction::isDone() const
{
    return true;
}

// CFiniteTimeAction
CFiniteTimeAction::CFiniteTimeAction(float fDuration)
: m_fDuration(fDuration)
, m_fElapsed(0.0f)
, m_bFirstTick(true)
{
    setDbgClassName("CFiniteTimeAction");
}

CFiniteTimeAction::~CFiniteTimeAction()
{
}

void CFiniteTimeAction::start(CUnit* pTarget)
{
    m_bFirstTick = true;
    CAction::start(pTarget);
}

void CFiniteTimeAction::step(float fDt)
{
    if (m_bFirstTick)
    {
        m_bFirstTick = false;
        m_fElapsed = fDt;
    }
    else
    {
        m_fElapsed += fDt;
    }
    
    update(max(0.0f,
               min(1.0f, m_fElapsed / max(m_fDuration, FLT_EPSILON))));
}

bool CFiniteTimeAction::isDone() const
{
    return m_fElapsed >= m_fDuration;
}

// CCallFunc
CCallFunc::CCallFunc(CMultiRefObject* pSel, FUNC_CALLFUNC_ND pCallFunc, void* pData)
: CFiniteTimeAction(0.0f)
, m_pSel(pSel)
, m_pCallFunc(pCallFunc)
, m_pData(pData)
{
    setDbgClassName("CCallFunc");
}

CCallFunc::~CCallFunc()
{
}

void CCallFunc::update(float fTimePercent)
{
    if (!m_pTarget || !m_pSel || !m_pCallFunc)
    {
        return;
    }
    (m_pSel->*m_pCallFunc)(m_pTarget, m_pData);
}

bool CCallFunc::isDone() const
{
    return true;
}

// CSequence
CSequence::CSequence(CFiniteTimeAction* pAct1, CFiniteTimeAction* pAct2)
: CFiniteTimeAction(pAct1->getDuration() + pAct2->getDuration())
, m_pFirstAction(pAct1)
, m_pSecondAction(pAct2)
, m_fSplit(0.0f)
, m_bFirst(true)
{
    setDbgClassName("CSequence");
    m_pFirstAction->retain();
    m_pSecondAction->retain();
}

CSequence::CSequence(CFiniteTimeAction* pAct1, ...)
: CFiniteTimeAction(0.0f)
, m_pFirstAction(pAct1)
, m_pSecondAction(NULL)
, m_fSplit(0.0f)
, m_bFirst(true)
{
    setDbgClassName("CSequence");
    
    va_list params;
    va_start(params, pAct1);
    
    while (pAct1)
    {
        m_pSecondAction = va_arg(params, CFiniteTimeAction*);
        if (m_pFirstAction)
        {
            m_pFirstAction = new CSequence(m_pFirstAction, m_pSecondAction);
        }
        else
        {
            break;
        }
    }
    
    setDuration(m_pFirstAction->getDuration() + m_pSecondAction->getDuration());
    m_pFirstAction->retain();
    m_pSecondAction->retain();
}

CSequence::~CSequence()
{
    m_pFirstAction->release();
    m_pSecondAction->release();
}

void CSequence::start(CUnit* pTarget)
{
    m_fSplit = m_pFirstAction->getDuration() / m_fDuration;
    m_bFirst = true;
    m_pFirstAction->start(pTarget);
    CFiniteTimeAction::start(pTarget);
}

void CSequence::update(float fTimePercent)
{
    if (!m_pTarget)
    {
        return;
    }
    
    float fSplitTimePercent = 0.0f;
    if (fTimePercent < m_fSplit)
    {
        fSplitTimePercent = fTimePercent / m_fSplit;
        m_pFirstAction->update(fSplitTimePercent);
    }
    else
    {
        if (m_bFirst)
        {
            m_pFirstAction->update(1.0f);
            m_bFirst = false;
            m_pSecondAction->start(m_pTarget);
        }
        fSplitTimePercent = (fTimePercent - m_fSplit) / (1 - m_fSplit);
        m_pSecondAction->update(fSplitTimePercent);
    }
}

void CSequence::stop()
{
    m_pFirstAction->stop();
    m_pSecondAction->stop();
    CFiniteTimeAction::stop();
}

// CMoveTo
CMoveTo::CMoveTo(float fDuration, const CPoint& rPos)
: CFiniteTimeAction(fDuration)
, m_oEndPos(rPos)
{
    setDbgClassName("CMoveTo");
}

CMoveTo::~CMoveTo()
{
}

void CMoveTo::start(CUnit* pTarget)
{
    CUnitDraw2D* d = DCAST(pTarget->getDraw(), CUnitDraw2D*);
    assert(d != NULL);
    m_oStartPos = d->getPosition();
    m_oDelta = m_oEndPos - m_oStartPos;
    CFiniteTimeAction::start(pTarget);
}

void CMoveTo::update(float fTimePercent)
{
    if (!m_pTarget)
    {
        return;
    }
    
    CUnitDraw2D* d = DCAST(m_pTarget->getDraw(), CUnitDraw2D*);
    d->setPosition(m_oDelta * fTimePercent + m_oStartPos);
}

// CSpeed
CSpeed::CSpeed(CFiniteTimeAction* pAction, float fSpeed)
: m_pInnerAction(pAction)
, m_fSpeed(fSpeed)
{
    setDbgClassName("CSpeed");
    m_pInnerAction->retain();
}

CSpeed::~CSpeed()
{
    m_pInnerAction->release();
}

void CSpeed::start(CUnit* pTarget)
{
    m_pInnerAction->start(pTarget);
    CAction::start(pTarget);
}

void CSpeed::step(float fDt)
{
    m_pInnerAction->step(m_fSpeed * fDt);
}

void CSpeed::stop()
{
    m_pInnerAction->stop();
    CAction::stop();
}

bool CSpeed::isDone() const
{
    return m_pInnerAction->isDone();
}

// CDelayTime
CDelayTime::CDelayTime(float fDuration)
: CFiniteTimeAction(fDuration)
{
}

