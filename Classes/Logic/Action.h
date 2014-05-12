/* 
 * File:   Action.h
 * Author: thunderliu
 *
 * Created on 2013年12月10日, 下午11:15
 */

#ifndef __ACTION_H__
#define	__ACTION_H__

#include "MultiRefObject.h"
#include "Base.h"


class CUnit;

class CAction : public CMultiRefObject
{
public:
    CAction();
    virtual ~CAction();
    
    M_SYNTHESIZE(CUnit*, m_pTarget, Target);
    virtual void start(CUnit* pTarget);
    virtual void stop();
    
    virtual void step(float fDt);
    virtual void update(float fTimePercent);
    virtual bool isDone() const;
    
    M_SYNTHESIZE(int, m_iTag, Tag);
    
protected:
};


class CFiniteTimeAction : public CAction
{
public:
    CFiniteTimeAction(float fDuration);
    virtual ~CFiniteTimeAction();
    
    M_SYNTHESIZE(float, m_fDuration, Duration);
    M_SYNTHESIZE(float, m_fElapsed, Elapsed);
    
    virtual void start(CUnit* pTarget);
    virtual void step(float fDt);
    virtual bool isDone() const;

protected:
    bool m_bFirstTick;
};


class CCallFunc : public CFiniteTimeAction
{
public:
    CCallFunc(CMultiRefObject* pSel, FUNC_CALLFUNC_ND pCallFunc, void* pData = nullptr);
    virtual ~CCallFunc();
    
    virtual void update(float fTimePercent);
    virtual bool isDone() const;
    
protected:
    CMultiRefObject* m_pSel;
    FUNC_CALLFUNC_ND m_pCallFunc;
    void* m_pData;
};


class CSequence : public CFiniteTimeAction
{
public:
    CSequence(CFiniteTimeAction* pAct1, CFiniteTimeAction* pAct2);
    CSequence(CFiniteTimeAction* pAct1, ...);
    virtual ~CSequence();
    
    M_SYNTHESIZE(CFiniteTimeAction*, m_pFirstAction, FirstAction);
    M_SYNTHESIZE(CFiniteTimeAction*, m_pSecondAction, SecondAction);
    
    virtual void start(CUnit* pTarget);
    virtual void update(float fTimePercent);
    virtual void stop();
    
protected:
    float m_fSplit;
    bool m_bFirst;
};


class CMoveTo : public CFiniteTimeAction
{
public:
    CMoveTo(float fDuration, const CPoint& rPos);
    virtual ~CMoveTo();
    
    virtual void start(CUnit* pTarget);
    virtual void update(float fTimePercent);
    
protected:
    CPoint m_oStartPos;
    CPoint m_oEndPos;
    CPoint m_oDelta;
};


class CSpeed : public CAction
{
public:
    CSpeed(CFiniteTimeAction* pAction, float fSpeed);
    virtual ~CSpeed();
    
    M_SYNTHESIZE(CFiniteTimeAction*, m_pInnerAction, InnerAction);
    M_SYNTHESIZE(float, m_fSpeed, Speed);
    
    virtual void start(CUnit* pTarget);
    virtual void step(float fDt);
    virtual void stop();
    virtual bool isDone() const;
    
};

class CDelayTime : public CFiniteTimeAction
{
public:
    CDelayTime(float fDuration);
};


#endif	/* __ACTION_H__ */

