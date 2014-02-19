/* 
 * File:   UnitDraw.cpp
 * Author: thunderliu
 * 
 * Created on 2014年2月13日, 下午6:34
 */

#include "CommInc.h"
#include "Draw.h"
#include "Skill.h"


// CCallFuncData
CCallFuncData::CCallFuncData( CMultiRefObject* pSel, FUNC_CALLFUNC_ND pCallFunc)
    : m_pSelector(pSel)
    , m_pCallFunc(pCallFunc)
{
    setDbgClassName("CCallFuncData");
}

// CUnitDraw
CUnitDraw::CUnitDraw(const char* pName)
    : m_sName(pName)
    , m_pUnit(NULL)
{
    setDbgClassName("CUnitDraw");
}

CUnitDraw::~CUnitDraw()
{
}

int CUnitDraw::doAnimation( ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone, float fSpeed /*= 1.0f*/ )
{
    return 0;
}

void CUnitDraw::stopAction(int tag)
{
}

void CUnitDraw::setActionSpeed(int tag, float fSpeed)
{
}

bool CUnitDraw::isDoingAction( int id )
{
    return false;
}

void CUnitDraw::stopAllActions()
{

}

void CUnitDraw::setFrame(FRM_ID id)
{
}

void CUnitDraw::setFlipX(bool bFlipX)
{
}

void CUnitDraw::loadAnimation(int id, const char* pName, int iNotifyFrameIndex, float fDelay)
{
}

void CUnitDraw::loadFrame(int id, const char* pName)
{
}

// CUnitDraw2D
CUnitDraw2D::CUnitDraw2D( const char* pName )
    : CUnitDraw(pName)
    , m_fHalfOfWidth(0.0f)
    , m_fHalfOfHeight(0.0f)
    , m_fBaseMoveSpeed(CONST_MIN_MOVE_SPEED)
    , m_bFixed(false)
    , m_iMoveToActionId(0)
    , m_iMoveActionId(0)
{
    setDbgClassName("CUnitDraw2D");
}

CUnitDraw2D::~CUnitDraw2D()
{
}

int CUnitDraw2D::doMoveTo( const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone, float fSpeed /*= 1.0f*/ )
{
    return 0;
}

bool CUnitDraw2D::isFlipX() const
{
    return false;
}

void CUnitDraw2D::setFlipX(bool bFlipX)
{
}

const CUnitDraw2D::UNIT_MOVE_PARAMS CUnitDraw2D::CONST_DEFAULT_MOVE_PARAMS;
const float CUnitDraw2D::CONST_MIN_MOVE_SPEED = 1.0f;
const float CUnitDraw2D::CONST_MAX_MOVE_SPEED = 500.0f;
const float CUnitDraw2D::CONST_MIN_MOVE_SPEED_MULRIPLE = 0.2f; // 最小变为基础速度的20%

void CUnitDraw2D::setBaseMoveSpeed(float fMoveSpeed)
{
    fMoveSpeed = max(fMoveSpeed, CONST_MIN_MOVE_SPEED);
    fMoveSpeed = min(fMoveSpeed, CONST_MAX_MOVE_SPEED);
    m_fBaseMoveSpeed = fMoveSpeed;
	updateMoveSpeedDelta();
}

void CUnitDraw2D::setExMoveSpeed(const CExtraCoeff& roExMoveSpeed)
{
    m_oExMoveSpeed = roExMoveSpeed;
    updateMoveSpeedDelta();
}

float CUnitDraw2D::getRealMoveSpeed() const
{
    float fMoveSpeed = getBaseMoveSpeed();
    float fRealMoveSpeed = m_oExMoveSpeed.getValue(fMoveSpeed);
    // 取最小移动速度和最小减速后速度的最大值作为最小移动速度
    float fMinMoveSpeed = fMoveSpeed * CONST_MIN_MOVE_SPEED_MULRIPLE;
    fMinMoveSpeed = max(CONST_MIN_MOVE_SPEED, fMinMoveSpeed);
    // 计算实际移动速度，不得超过上述计算所得的最小值
    fRealMoveSpeed = max(fRealMoveSpeed, fMinMoveSpeed);
    // 计算实际移动速度，不得超过最大移动速度
    return min(fRealMoveSpeed, CONST_MAX_MOVE_SPEED);
}

void CUnitDraw2D::updateMoveSpeedDelta()
{
    float fMoveSpeed = getBaseMoveSpeed();
    if (fMoveSpeed < FLT_EPSILON)
    {
        stopMove();
        return;
    }
	
}

void CUnitDraw2D::move(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/)
{
    CUnit* u = getUnit();
    if (u->isDead() || u->isSuspended() || isFixed())
    {
        return;
    }
    m_oLastMoveToTarget = roPos;
    
    if (roMoveParams.bAutoFlipX)
    {
        setFlipX(getPosition().x > roPos.x);
    }

    float fMoveSpeed = getBaseMoveSpeed();
    float fDur = getPosition().getDistance(roPos) / max(fMoveSpeed, FLT_EPSILON);
    float fSpeed = getRealMoveSpeed() / fMoveSpeed;

    // 突发移动指令，打断旧移动，打断攻击，打断施法
    if (u->isDoingOr(CUnit::kMoving))
    {
        stopAction(getMoveToActionId());
        setMoveToActionId(0);
    }
    if (u->isDoingOr(CUnit::kCasting) && roMoveParams.bCancelCast)
    {
        //stopCast();
    }
    
    int id = doMoveTo(roPos,
                      fDur,
                      new CCallFuncData(this,
                      (FUNC_CALLFUNC_ND)&CUnitDraw2D::onMoveDone),
                      fSpeed);
    setMoveToActionId(id);
    if (getMoveActionId() == 0 && u->isDoingOr(CUnit::kSpinning) == false)
    {
        int id = doAnimation(kActMove,
                             NULL,
                             INFINITE,
                             NULL,
                             fSpeed);
        setMoveActionId(id);
    }

    u->startDoing(CUnit::kMoving);
    if (roMoveParams.bIntended)
    {
        u->startDoing(CUnit::kIntended);
    }
    else
    {
        u->endDoing(CUnit::kIntended);
    }
}

void CUnitDraw2D::follow(int iTargetUnit, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/)
{
    /*
    if (isDead() || isFixed())
    {
        return;
    }
    M_DEF_GM(pGm);
    CGameUnit* pTarget = getUnitLayer()->getUnitByKey(iTargetKey);
    if (!pTarget)
    {
        return;
    }
    const CCPoint& roPos = pTarget->getPosition();
    m_oLastMoveToTarget = roPos;
    if (isDoingOr(kSuspended))
    {
        return;
    }
    CCPoint roOrg = getPosition();
    if (roMoveParams.bAutoFlipX)
    {
        turnTo(roOrg.x > roPos.x);
    }

    float fMoveSpeed = getBaseMoveSpeed();
    float fDur = pGm->getDistance(roOrg, roPos) / fMoveSpeed;
    CCActionInterval* pSeq = CCSequence::createWithTwoActions(
                                                              CCMoveToNode::create(fDur, pTarget->getSprite(), true, roMoveParams.fMaxOffsetY, 1, pTarget->getHalfOfHeight()),
                                                              CCCallFuncN::create(this, callfuncN_selector(CUnit::onActMoveEnd))
                                                             );
    float fDelta = getRealMoveSpeed() / fMoveSpeed;
    CCSpeed* pActMoveTo = CCSpeed::create(pSeq, fDelta);
    pActMoveTo->setTag(kActMoveTo);
    // 突发移动指令，打断旧移动，打断攻击
    if (isDoingOr(kMoving))
    {
        m_oSprite.stopActionByTag(kActMoveTo);
    }
    if (isDoingOr(kAttacking) && roMoveParams.bCancelAttack)
    {
        stopAttack();
    }
    if (isDoingOr(kCasting) && roMoveParams.bCancelCast)
    {
        stopCast();
    }
    m_oSprite.runAction(pActMoveTo);
    if (!isDoingOr(kSpinning))
    {
        setAnimation(kAnimationMove, -1, fDelta, kActMove, NULL);
    }

    startDoing(kMoving);
    if (roMoveParams.bIntended)
    {
        startDoing(kIntended);
    }
    else
    {
        endDoing(kIntended);
    }
    */
}

//void CUnitDrawForCC::moveAlongPath(CUnitPath* pPath, bool bIntended /*= true*/, bool bRestart /*= false*/, float fBufArrive /*= 5.0*/)
//{
//    if (pPath != m_pMovePath)
//    {
//        CC_SAFE_RETAIN(pPath);
//        CC_SAFE_RELEASE(m_pMovePath);
//        m_pMovePath = pPath;
//    }
//
//    if (!m_pMovePath)
//    {
//        return;
//    }
//
//    if (bRestart)
//    {
//        m_dwPathCurPos = 0;
//    }
//
//    if (m_fPathBufArrive != fBufArrive)
//    {
//        m_fPathBufArrive = MAX(FLT_EPSILON, fBufArrive);
//    }
//
//    setPathIntended(bIntended);
//
//    const CCPoint* pTarget = m_pMovePath->getCurTargetPoint(m_dwPathCurPos);
//    if (pTarget)
//    {
//        UNIT_MOVE_PARAMS oMp;
//        oMp.bIntended = m_bPathIntended;
//        moveTo(*pTarget, oMp);
//    }
//}
#if 0
void CUnitDraw2D::setPathIntended(bool bPathIntended /*= true*/)
{
    m_bPathIntended = bPathIntended;
}

bool CUnitDraw2D::isPathIntended() const
{
    return m_bPathIntended;
}
#endif
void CUnitDraw2D::stopMove()
{
    stopAction(getMoveToActionId());
    setMoveToActionId(0);

    stopAction(getMoveActionId());
    setMoveActionId(0);

    CUnit* u = getUnit();
    u->endDoing(CUnit::kMoving);

    setFrame(kFrmDefault);
}

void CUnitDraw2D::onMoveDone(CMultiRefObject* pUnit, CCallFuncData* pData)
{
    stopMove();
}

void CUnitDraw2D::startMoveAct(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams/* = CONST_DEFAULT_MOVE_PARAMS*/)
{
}

void CUnitDraw2D::stopMoveAct()
{
}

int CUnitDraw2D::cmdCastSpell( int iActiveSkillId )
{
    CUnit* u = getUnit();
    
    // 单位合法性判断
    if (u->isSuspended() || u->isDead())
    {
        // 挂起、死亡
        return -1;
    }

    // 技能合法性判断
    CActiveSkill* pSkill = u->getActiveSkill(iActiveSkillId);
    if (pSkill == NULL)
    {
        // 没找到指定主动技能
        return -1;
    }

    // 技能目标合法性判断
    CUnit* t = NULL;
    CUnitDraw2D* td = NULL;
    switch (pSkill->getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        break;

    case CCommandTarget::kUnitTarget:
        t = u->getUnit(getCastTarget().getTargetUnit());
        if (t == NULL)
        {
            // 目标不存在
            return -1;
        }

        td = DCAST(t->getDraw(), CUnitDraw2D*);
        if (td == NULL)
        {
            // 目标不具备2D性
            return -1;
        }

        getCastTarget().setTargetPoint(td->getPosition());
        
    case CCommandTarget::kPointTarget:
        if (getCastTarget().getTargetType() != pSkill->getCastTargetType())
        {
            // 指定施法目标为点或单位的时候，与技能施法目标不符
            return -1;
        }
        break;
    }

    if (pSkill->checkConditions() == false)
    {
        return -1;
    }

    // 施法距离合法性
    if (checkCastTargetDistance(pSkill, getPosition(), td) == false)
    {
        // 施法移动
        if (isFixed())
        {
            return -1;
        }
        moveToCastPosition(pSkill, td);
    }

    // 施法
    castSpell(pSkill);

    return 0;
}

int CUnitDraw2D::castSpell(CActiveSkill* pSkill)
{
    //stopAction()
    return 0;
}

bool CUnitDraw2D::checkCastTargetDistance( CActiveSkill* pSkill, const CPoint& roPos, CUnitDraw2D* td )
{
    CPoint roPos2;
    switch (getCastTarget().getTargetType())
    {
        case CCommandTarget::kNoTarget:
            return true;

        case CCommandTarget::kUnitTarget:
            roPos2 = td->getPosition();
            break;

        case CCommandTarget::kPointTarget:
            roPos2 = getCastTarget().getTargetPoint();
            break;
    }

                                                              
    //CCLOG("dis: %.2f, %.2f, %.2f | %.2f", pGm->getDistance(roPos, roPos2) - getHalfOfWidth() - pTarget->getHalfOfWidth(), getAttackMinRange(), getAttackRange(), abs(roPos.y - roPos2.y));
    if (pSkill->isHorizontal() && abs(roPos.y - roPos2.y) > CActiveSkill::CONST_MAX_HOR_CAST_Y_RANGE)
    {
        return false;
    }

    float fDis = max(roPos.getDistance(roPos2) - getHalfOfWidth() - (td != NULL ? td->getHalfOfWidth() : 0.0f), 0.5f);
    if (fDis < pSkill->getCastMinRange() || fDis > pSkill->getCastRange())
    {
        return false;
    }

    return true;
}

void CUnitDraw2D::moveToCastPosition(CActiveSkill* pSkill, CUnitDraw2D* td)
{
    //stopAllActions();
    float fDis = td != NULL ? td->getHalfOfWidth() : 0.0f + getHalfOfWidth() + (pSkill->getCastMinRange() + pSkill->getCastRange()) * 0.5;
    const CPoint& roPos1 = getPosition();
    const CPoint& roPos2 = td != NULL ? td->getPosition() : getCastTarget().getTargetPoint();

    UNIT_MOVE_PARAMS oMp;
    oMp.bIntended = false;
    if (pSkill->isHorizontal())
    {
        // 近战攻击位置修正
        move(CPoint(roPos2.x + ((roPos1.x > roPos2.x) ? fDis : -fDis), roPos2.y), oMp);
    }
    else
    {
        // 远程攻击位置修正
        float fA = -(roPos1 - roPos2).getAngle();
        move(roPos2 + CPoint(cos(fA) * fDis, sin(-fA) * fDis), oMp);
    }
}
