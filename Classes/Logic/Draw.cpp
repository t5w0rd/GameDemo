/* 
 * File:   UnitDraw.cpp
 * Author: thunderliu
 * 
 * Created on 2014年2月13日, 下午6:34
 */

#include "CommInc.h"
#include "Draw.h"
#include "Ability.h"


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

void CUnitDraw::onTick( float dt )
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
    , m_iCastActionId(0)
    , m_iCastActiveAbilityId(0)
{
    setDbgClassName("CUnitDraw2D");
}

CUnitDraw2D::~CUnitDraw2D()
{
}

void CUnitDraw2D::onTick(float dt)
{
    CUnit* u = getUnit();
    if (u->isDoingAnd(CUnit::kMoving))
    {
        return;
    }
    if (u->isDoingAnd(CUnit::kCasting) && isDoingAction(getCastActionId()) == false)
    {
        LOG("move to cast..");
        CUnit* u = getUnit();
        CUnitDraw2D* td = NULL;
        CActiveAbility* pAbility = u->getActiveAbility(getCastActiveAbilityId());
        if (pAbility != NULL)
        {
            if (getCastTarget().getTargetType() == CCommandTarget::kUnitTarget)
            {
                CUnit* t = u->getUnit(getCastTarget().getTargetUnit());
                td = DCAST(t->getDraw(), CUnitDraw2D*);
                if (checkCastTargetDistance(pAbility, getPosition(), td))
                {
                    // 施法
                    setFlipX(getCastTarget().getTargetPoint().x < getPosition().x);
                    castSpell(pAbility);
                }
                else if (checkCastTargetDistance(pAbility, getLastMoveToTarget(), td) == false)
                {
                    if(getPosition().getDistance(getCastTarget().getTargetPoint()) < 100)
                        moveToCastPosition(pAbility, td);
                }
            }
            
        }
        
    }

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

void CUnitDraw2D::cmdMove( const CPoint& roPos )
{
    CUnit* u = getUnit();
    if (u->isDead() || u->isSuspended() || isFixed())
    {
        return;
    }

    UNIT_MOVE_PARAMS mp;
    mp.bAutoFlipX = true;
    move(roPos);
}

void CUnitDraw2D::move(const CPoint& roPos, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/)
{
    CUnit* u = getUnit();

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
        int id = doAnimation(kAniMove,
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
    CUnit* pTarget = getUnitLayer()->getUnitByKey(iTargetKey);
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

int CUnitDraw2D::cmdCastSpell( int iActiveAbilityId )
{
    CUnit* u = getUnit();
    
    // 单位合法性判断
    if (u->isSuspended() || u->isDead())
    {
        // 挂起、死亡
        return -1;
    }

    // 技能合法性判断
    CActiveAbility* pAbility = u->getActiveAbility(iActiveAbilityId);
    if (pAbility == NULL)
    {
        // 没找到指定主动技能
        return -1;
    }

    // 技能目标合法性判断
    CUnit* t = NULL;
    CUnitDraw2D* td = NULL;
    switch (pAbility->getCastTargetType())
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
        if (getCastTarget().getTargetType() != pAbility->getCastTargetType())
        {
            // 指定施法目标为点或单位的时候，与技能施法目标不符
            return -1;
        }
        break;
    }

    if (pAbility->checkConditions() == false)
    {
        return -1;
    }

    setCastActiveAbilityId(iActiveAbilityId);
    // 施法距离合法性
    if (checkCastTargetDistance(pAbility, getPosition(), td) == false)
    {
        // 施法移动
        if (isFixed())
        {
            return -1;
        }
        moveToCastPosition(pAbility, td);
        return 0;
    }

    // 施法
    if (getCastTarget().getTargetType() != CCommandTarget::kNoTarget)
    {
        setFlipX(getCastTarget().getTargetPoint().x < getPosition().x);
    }
    castSpell(pAbility);

    return 0;
}

int CUnitDraw2D::castSpell(CActiveAbility* pAbility)
{
    if (pAbility->isCoolingDown())
    {
        return -1;
    }

    CUnit* u = getUnit();

    stopMove();
    
    CUnitDraw::ANI_ID aniId = (CUnitDraw::ANI_ID)pAbility->getCastRandomAnimation();
    int id = doAnimation(aniId,
                         new CCallFuncData(this,
                                           (FUNC_CALLFUNC_ND)&CUnitDraw2D::onCastEffect),
                         1,
                         new CCallFuncData(this,
                                           (FUNC_CALLFUNC_ND)&CUnitDraw2D::onCastDone),
                         1.0f);

    
    setCastActionId(id);

    u->startDoing(CUnit::kCasting);
    

    return 0;
}

bool CUnitDraw2D::checkCastTargetDistance( CActiveAbility* pAbility, const CPoint& roPos, CUnitDraw2D* td )
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
    if (pAbility->isCastHorizontal() && abs(roPos.y - roPos2.y) > CActiveAbility::CONST_MAX_HOR_CAST_Y_RANGE)
    {
        return false;
    }

    float fDis = roPos.getDistance(roPos2) - getHalfOfWidth() - (td != NULL ? td->getHalfOfWidth() : 0.0f);
    if (fDis < pAbility->getCastMinRange() || fDis > pAbility->getCastRange())
    {
        return false;
    }

    return true;
}

void CUnitDraw2D::moveToCastPosition(CActiveAbility* pAbility, CUnitDraw2D* td)
{
    //stopAllActions();
    float fDis = (td != NULL ? td->getHalfOfWidth() : 0.0f) + getHalfOfWidth() + (pAbility->getCastMinRange() + pAbility->getCastRange()) * 0.5;
    const CPoint& roPos1 = getPosition();
    const CPoint& roPos2 = (td != NULL ? td->getPosition() : getCastTarget().getTargetPoint());

    UNIT_MOVE_PARAMS oMp;
    oMp.bIntended = false;
    if (pAbility->isCastHorizontal())
    {
        // 近战施法位置修正
        move(CPoint(roPos2.x + ((roPos1.x > roPos2.x) ? fDis : -fDis), roPos2.y), oMp);
    }
    else
    {
        // 远程施法位置修正
        float fA = -(roPos1 - roPos2).getAngle();
        move(roPos2 + CPoint(cos(fA) * fDis, sin(-fA) * fDis), oMp);
    }

    CUnit* u = getUnit();
    u->startDoing(CUnit::kCasting);
}

void CUnitDraw2D::onCastEffect( CMultiRefObject* pUnit, CCallFuncData* pData )
{
    CUnit* u = getUnit();
    CActiveAbility* pAbility = u->getActiveAbility(getCastActiveAbilityId());
    if (pAbility == NULL)
    {
        return;
    }
    pAbility->cast();
}

void CUnitDraw2D::onCastDone( CMultiRefObject* pUnit, CCallFuncData* pData )
{
    CUnit* u = getUnit();
    CActiveAbility* pAbility = u->getActiveAbility(getCastActiveAbilityId());
    if (pAbility == NULL)
    {
        return;
    }

    if (u->getAttackAbilityId() != 0 && pAbility->getId() == u->getAttackAbilityId())
    {
        return;
    }

    stopCast();
}

void CUnitDraw2D::stopCast()
{
    stopAction(getCastActionId());
    setCastActionId(0);
    setCastActiveAbilityId(0);

    CUnit* u = getUnit();
    u->endDoing(CUnit::kCasting);
    setFrame(kFrmDefault);
}

void CUnitDraw2D::cmdStop()
{
    CUnit* u = getUnit();
    if (u->isDead() || u->isSuspended() || isFixed())
    {
        return;
    }

    stop();
}

void CUnitDraw2D::stop(bool bDefaultFrame)
{
    stopAction(getMoveToActionId());
    setMoveToActionId(0);

    stopAction(getMoveActionId());
    setMoveActionId(0);

    stopAction(getCastActionId());
    setCastActionId(0);
    setCastActiveAbilityId(0);

    CUnit* u = getUnit();
    u->endDoing(CUnit::kMoving | CUnit::kCasting);

    if (bDefaultFrame)
    {
        setFrame(kFrmDefault);
    }
    
}

CUnitGroup::CUnitGroup()
{
}

CUnitGroup::CUnitGroup( CWorld* pWorld, const CPoint& roPos, float fRadius, int iMaxCount /*= INFINITE*/, FUNC_UNIT_CONDITION pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    if (fRadius < FLT_EPSILON)
    {
        return;
    }

    CWorld::MAP_UNITS& units = pWorld->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if ((int)m_vecUnits.size() >= iMaxCount)
        {
            return;
        }
        if (d->getPosition().getDistance(roPos) < fRadius && (!pBoolFunc || (pBoolFunc && pBoolFunc(u, pParam))))
        {
            m_vecUnits.addObject(u);
        }

        M_MAP_NEXT;
    }

    return;
}

CUnitGroup::CUnitGroup( CWorld* pWorld, int iMaxCount /*= INFINITE*/, FUNC_UNIT_CONDITION pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    CWorld::MAP_UNITS& units = pWorld->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if ((int)m_vecUnits.size() >= iMaxCount)
        {
            return;
        }

        if (!pBoolFunc || (pBoolFunc && pBoolFunc(u, pParam)))
        {
            m_vecUnits.addObject(u);
        }

        M_MAP_NEXT;
    }
}

CUnit* CUnitGroup::getUnitByIndex( int iIndex )
{
    if (iIndex < 0 || iIndex >= (int)m_vecUnits.size())
    {

        return NULL;
    }

    return m_vecUnits[iIndex];
}

CUnit* CUnitGroup::getRandomUnit()
{
    return m_vecUnits[rand() % m_vecUnits.size()];
}

CUnit* CUnitGroup::getNearestUnitInRange( const CPoint& roPos, float fRadius, FUNC_UNIT_CONDITION pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    CUnit* pTarget = NULL;
    float fMinDis = FLT_MAX;
    float fDis;

    M_VEC_FOREACH(m_vecUnits)
    {
        CUnit* u = M_VEC_EACH;
        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if ((fDis = d->getPosition().getDistance(roPos)) < fRadius && fMinDis > fDis && (!pBoolFunc || (pBoolFunc && pBoolFunc(u, pParam))))
        {
            pTarget = u;
            fMinDis = fDis;
        }
        M_VEC_NEXT;
    }

    return pTarget;
}

void CUnitGroup::addUnit( CUnit* pUnit )
{
    m_vecUnits.addObject(pUnit);
}

CUnit* CUnitGroup::getNearestUnitInRange( CWorld* pWorld, const CPoint& roPos, float fRadius, FUNC_UNIT_CONDITION pBoolFunc /*= NULL*/, void* pParam /*= NULL*/ )
{
    CUnit* pTarget = NULL;
    float fMinDis = FLT_MAX;
    float fDis;

    CWorld::MAP_UNITS& units = pWorld->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if ((fDis = d->getPosition().getDistance(roPos)) < fRadius && fMinDis > fDis && (!pBoolFunc || (pBoolFunc && pBoolFunc(u, pParam))))
        {
            pTarget = u;
            fMinDis = fDis;
        }
        M_MAP_NEXT;
    }

    return pTarget;
}

void CUnitGroup::cleanUnits()
{
    m_vecUnits.delAllObjects();
}

int CUnitGroup::getUnitsCount()
{
    return m_vecUnits.size();
}

bool CUnitGroup::isLivingAllyOf( CUnit* pUnit, CUnitForce* pParam )
{
    return !pUnit->isDead() && pUnit->isAllyOf(pParam);
}

bool CUnitGroup::isLivingEnemyOf( CUnit* pUnit, CUnitForce* pParam )
{
    return !pUnit->isDead() && pUnit->isEnemyOf(pParam);
}
