/*
* File:   UnitDraw.cpp
* Author: thunderliu
* 
* Created on 2014年2月13日, 下午6:34
*/

#include "CommInc.h"
#include "Draw.h"
#include "Ability.h"


// CUnitDraw
CUnitDraw::CUnitDraw()
: m_pUnit(nullptr)
{
    setDbgClassName("CUnitDraw");
}

CUnitDraw::~CUnitDraw()
{
}

CUnitDraw* CUnitDraw::copy()
{
    CUnitDraw* d = new CUnitDraw;
    d->copyData(this);
    return d;
}

void CUnitDraw::copyData(CUnitDraw* from)
{
}

int CUnitDraw::doAnimation(int id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone, float fSpeed /*= 1.0f*/)
{
    return 0;
}

void CUnitDraw::stopAction(int tag)
{
}

void CUnitDraw::setActionSpeed(int tag, float fSpeed)
{
}

bool CUnitDraw::isDoingAction(int id)
{
    return false;
}

void CUnitDraw::stopAllActions()
{
}

void CUnitDraw::setVisible(bool bVisible /*= true*/)
{
}

void CUnitDraw::onUnitRevive()
{
}

void CUnitDraw::onUnitDying()
{
}

void CUnitDraw::onUnitTick(float dt)
{
}

void CUnitDraw::setFrame(int id)
{
}

void CUnitDraw::setFrameByAnimation(int id, int index)
{
}

void CUnitDraw::setFlippedX(bool bFlipX)
{
}

void CUnitDraw::loadAnimation(int id, const char* pName, int iNotifyFrameIndex, float fDelay)
{
}

void CUnitDraw::loadFrame(int id, const char* pName)
{
}

void CUnitDraw::say(const char* words)
{
}

// CUnitDraw2D
CUnitDraw2D::CUnitDraw2D()
: m_fHalfOfWidth(0.0f)
, m_fHalfOfHeight(0.0f)
, m_fHeight(0.0f)
, m_fBaseMoveSpeed(CONST_MIN_MOVE_SPEED)
, m_bFixed(false)
, m_fHostilityRange(300.0f)
, m_iMoveToActionId(0)
, m_iMoveActionId(0)
, m_pMovePath(nullptr)
, m_dwPathCurPos(0)
, m_bPathObstinate(false)
, m_fPathBufArrive(5.0f)
, m_iCastActionId(0)
, m_iCastActiveAbilityId(0)
, m_iWaitForCastTargetActiveAbilityId(0)
{
    setDbgClassName("CUnitDraw2D");
}

CUnitDraw2D::~CUnitDraw2D()
{
    M_SAFE_RELEASE(m_pMovePath);
}

CUnitDraw2D* CUnitDraw2D::copy()
{
    CUnitDraw2D* ret = new CUnitDraw2D;
    ret->copyData(this);
    return ret;
}

void CUnitDraw2D::copyData(CUnitDraw* from)
{
    CUnitDraw::copyData(from);
    CUnitDraw2D* d = DCAST(from, CUnitDraw2D*);
    m_fHalfOfWidth = d->m_fHalfOfWidth;
    m_fHalfOfHeight = d->m_fHalfOfHeight;
    m_oFirePoint = d->m_oFirePoint;
    m_fHeight = d->m_fHeight;
    m_fBaseMoveSpeed = d->m_fBaseMoveSpeed;
    m_oExMoveSpeed = d->m_oExMoveSpeed;
    m_bFixed = d->m_bFixed;
    m_fHostilityRange = d->m_fHostilityRange;
}

void CUnitDraw2D::onUnitRevive()
{
    setFrame(kFrmDefault);
}

void CUnitDraw2D::onUnitDying()
{
    CUnit* u = getUnit();
    if (u->isDead() == false)
    {
        return;
    }

    die();
}

void CUnitDraw2D::onUnitTick(float dt)
{
    // 为了沿路径移动以及校正施法位置
    CUnit* u = getUnit();
    if (u->isSuspended())
    {
        return;
    }

    if (m_pMovePath && !u->isDoingOr(CUnit::kMoving | CUnit::kCasting))
    {
        u->startDoing(CUnit::kAlongPath);
        if (m_bPathObstinate)
        {
            u->startDoing(CUnit::kObstinate);
        }
    }
    
    // 路径逻辑
    if (m_pMovePath != nullptr && u->isDoingOr(CUnit::kAlongPath))
    {
        // 正在运行路径
        const CPoint* pTarget = m_pMovePath->getCurTargetPoint(m_dwPathCurPos);
        assert(pTarget != nullptr);
        bool bArrive = false;
        bool bPathEnd = false;
        if (pTarget->getDistance(getPosition()) < m_fPathBufArrive)
        {
            bArrive = true;
            bPathEnd = m_pMovePath->arriveCurTargetPoint(m_dwPathCurPos);
            pTarget->getDistance(getPosition());
        }

        if (bPathEnd)
        {
            m_pMovePath->release();
            m_pMovePath = nullptr;
            u->endDoing(CUnit::kAlongPath);
        }
        else if ((*pTarget != getLastMoveToTarget() || u->isDoingOr(CUnit::kMoving) == false) && u->isDoingOr(CUnit::kCasting) == false)
        {
            // 单位没有施法，并且当前路径目标点不是移动目标点 或 单位没在移动，则继续沿路径行进
            if (m_bPathObstinate)
            {
                u->startDoing(CUnit::kObstinate);
            }
            else
            {
                u->endDoing(CUnit::kObstinate);
            }

            move(*pTarget);
        }
    }

    if (u->isDoingAnd(CUnit::kCasting) && isDoingAction(getCastActionId()) == false)
    {
        // 正在施法，且不是在施法动画中
        CUnit* u = getUnit();
        CUnitDraw2D* td = nullptr;
        CActiveAbility* pAbility = u->getActiveAbility(getCastActiveAbilityId());
        if (pAbility != nullptr)
        {
            // 如果施法技能仍存在
            bool bUnitTarget = getCastTarget().getTargetType() == CCommandTarget::kUnitTarget;
            if (bUnitTarget)
            {
                // 如果是以单位为目标的技能
                CUnit* t = u->getUnit(getCastTarget().getTargetUnit());
                if (t != nullptr && t->isDead() == false)
                {
                    // 单位存在且单位没有死亡
                    td = DCAST(t->getDraw(), CUnitDraw2D*);
                    assert(td != nullptr);
                    getCastTarget().setTargetPoint(td->getPosition());
                }
                else
                {
                    //u->endDoing(CUnit::kCasting);
                    //setCastActiveAbilityId(0);
                    stopCast(false);
                    u->endDoing(CUnit::kObstinate);  // 没有成功施法，需要取出固执状态
                    return;
                }
            }

            if (checkCastTargetDistance(pAbility, getPosition(), getCastTarget(), td))
            {
                // 施法
                if (!isFixed())
                {
                    setFlippedX(getCastTarget().getTargetPoint().x < getPosition().x);
                }
                
                castSpell(pAbility);
            }
            else if (pAbility->isCoolingDown() == false && (u->isDoingOr(CUnit::kMoving) == false || checkCastTargetDistance(pAbility, getLastMoveToTarget(), getCastTarget(), td) == false))
            {
                if (!isFixed())
                {
                    moveToCastPosition(pAbility, td);
                }
                else
                {
                    //u->endDoing(CUnit::kCasting | CUnit::kObstinate);
                    //setCastActiveAbilityId(0);
                    stopCast(false);
                    u->endDoing(CUnit::kObstinate);
                }
                
            }
            return;
        }
        else
        {
            u->endDoing(CUnit::kObstinate);  // 没有成功施法，需要去除固执状态
        }
    }

}

int CUnitDraw2D::doMoveTo(const CPoint& rPos, float fDuration, const FUNC_VOID& onMoveToDone, float fSpeed /*= 1.0f*/)
{
    return 0;
}

void CUnitDraw2D::updateMoveTo(const CPoint& rPos)
{
    return;
}

bool CUnitDraw2D::isFlippedX() const
{
    return false;
}

void CUnitDraw2D::setFlippedX(bool bFlipX)
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
    float spd = getRealMoveSpeed() / fMoveSpeed;
    setActionSpeed(getMoveActionId(), spd);
    setActionSpeed(getMoveToActionId(), spd);
}

void CUnitDraw2D::cmdMove(const CPoint& roPos, bool bObstinate)
{
    CUnit* u = getUnit();
    if (u->isDead() || u->isSuspended() || isFixed())
    {
        return;
    }

    if (u->isDoingOr(CUnit::kCasting))
    {
        //u->endDoing(CUnit::kCasting);
        //setCastActiveAbilityId(0);
        stopCast(false);
    }

    if (u->isDoingOr(CUnit::kAlongPath))
    {
        u->endDoing(CUnit::kAlongPath);
    }

    if (bObstinate)
    {
        u->startDoing(CUnit::kObstinate);
    }
    else
    {
        u->endDoing(CUnit::kObstinate);
    }

//     UNIT_MOVE_PARAMS mp;
//     mp.bAutoFlipX = true;
    move(roPos);
}

void CUnitDraw2D::move(const CPoint& roPos)  //, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/)
{
    CUnit* u = getUnit();

    if (u->isSuspended())
    {
        return;
    }

    m_oLastMoveToTarget = roPos;

//     if (roMoveParams.bAutoFlipX)
//     {
//         setFlippedX(getPosition().x > roPos.x);
//     }

    setFlippedX(getPosition().x > roPos.x);
    float fMoveSpeed = getBaseMoveSpeed();
    float fDur = getPosition().getDistance(roPos) / max(fMoveSpeed, FLT_EPSILON);
    float fSpeed = getRealMoveSpeed() / fMoveSpeed;

    // 突发移动指令，打断旧移动，打断攻击，打断施法
    if (isDoingAction(getMoveToActionId()))
    {
        stopAction(getMoveToActionId());
        setMoveToActionId(0);
    }
    if (isDoingAction(getCastActionId()))
    {
        stopAction(getCastActionId());
        setCastActionId(0);
    }
    u->startDoing(CUnit::kMoving);

    if (getMoveToActionId() == 0)
    {
        int id = doMoveTo(
            roPos,
            fDur,
            CC_CALLBACK_0(CUnitDraw2D::onMoveDone, this),
            fSpeed);
        setMoveToActionId(id);
    }

    if (getMoveActionId() == 0 && u->isDoingOr(CUnit::kSpinning) == false)
    {
        int id = doAnimation(
            kAniMove,
            nullptr,
            INFINITE,
            nullptr,
            fSpeed);
        setMoveActionId(id);
    }
}

void CUnitDraw2D::follow(int iTargetUnit)  //, const UNIT_MOVE_PARAMS& roMoveParams /*= CONST_DEFAULT_MOVE_PARAMS*/)
{
}

void CUnitDraw2D::cmdMoveAlongPath(CUnitPath* pPath, bool bObstinate /*= true*/, float fBufArrive /*= 5.0*/)
{
    CUnit* u = getUnit();
    if (u->isDead() || u->isSuspended() || isFixed())
    {
        return;
    }

    if (pPath != m_pMovePath)
    {
        M_SAFE_RETAIN(pPath);
        M_SAFE_RELEASE(m_pMovePath);
        m_pMovePath = pPath;
    }

    if (u->isDoingOr(CUnit::kAlongPath) == false)
    {
        u->startDoing(CUnit::kAlongPath);
    }

    m_bPathObstinate = bObstinate;

    if (!m_pMovePath)
    {
        return;
    }

    m_dwPathCurPos = 0;

    if (m_fPathBufArrive != fBufArrive)
    {
        m_fPathBufArrive = max(FLT_EPSILON, fBufArrive);
    }

    if (u->isDoingOr(CUnit::kCasting))
    {
        //u->endDoing(CUnit::kCasting);
        //setCastActiveAbilityId(0);
        stopCast(false);
    }

    if (bObstinate)
    {
        u->startDoing(CUnit::kObstinate);
    }
    else
    {
        u->endDoing(CUnit::kObstinate);
    }

    const CPoint* pTarget = m_pMovePath->getCurTargetPoint(m_dwPathCurPos);
    if (pTarget)
    {
        move(*pTarget);
    }
}

void CUnitDraw2D::stopMove()
{
    stopAction(getMoveToActionId());
    setMoveToActionId(0);

    stopAction(getMoveActionId());
    setMoveActionId(0);

    CUnit* u = getUnit();
    u->endDoing(CUnit::kMoving | CUnit::kAlongPath);

    setFrame(kFrmDefault);
}

void CUnitDraw2D::onMoveDone()
{
    CUnit* u = getUnit();
    u->endDoing(CUnit::kObstinate);  // 移动自行停止后，需要取出固执状态
    stopMove();
}

int CUnitDraw2D::cmdCastSpell(const CCommandTarget& rTarget, int iActiveAbilityId, bool bObstinate)
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
    if (pAbility == nullptr)
    {
        // 没找到指定主动技能
        return -1;
    }

    // 技能目标合法性判断
    CUnit* t = nullptr;
    CUnitDraw2D* td = nullptr;
    bool bFlippedX = isFlippedX();

    switch (pAbility->getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        break;

    case CCommandTarget::kUnitTarget:
        if (rTarget.getTargetType() != CCommandTarget::kUnitTarget)
        {
            return -1;
        }

        t = u->getUnit(rTarget.getTargetUnit());
        if (t == nullptr)
        {
            // 目标不存在
            return -1;
        }

        td = DCAST(t->getDraw(), CUnitDraw2D*);
        assert(td != nullptr);

        bFlippedX = (td->getPosition().x < getPosition().x);

        break;

    case CCommandTarget::kPointTarget:
        if (rTarget.getTargetType() != CCommandTarget::kPointTarget)
        {
            // 指定施法目标为点或单位的时候，与技能施法目标不符
            return -1;
        }

        bFlippedX = (rTarget.getTargetPoint().x < getPosition().x);

        break;
    }

    if (pAbility->checkConditions(rTarget) == false)
    {
        return -1;
    }

    // 施法距离合法性
    bool bDisOk = checkCastTargetDistance(pAbility, getPosition(), rTarget, td);
    if (bDisOk == false && isFixed())
    {
        return -1;
    }

    bool bSameTarget = false;
    if (rTarget.getTargetType() != CCommandTarget::kNoTarget)
    {
        // 如果不是无目标，需要改变目标
        setCastTarget(rTarget);
        bSameTarget = (rTarget == getCastTarget());
    }
    else
    {
        bSameTarget = true;
    }

    // 可以进行施法动画动作 或 追逐动作
    if (bObstinate)
    {
        u->startDoing(CUnit::kObstinate);
    }
    else
    {
        u->endDoing(CUnit::kObstinate);
    }

    bool bSameAbility = (getCastActiveAbilityId() == iActiveAbilityId);
    setCastActiveAbilityId(iActiveAbilityId);
    u->startDoing(CUnit::kCasting);

    if (bDisOk == false)
    {
        moveToCastPosition(pAbility, td);
        return 1;
    }

    // 施法
    if (isDoingAction(getCastActionId()) &&
        bSameAbility &&
        bSameTarget &&
        bFlippedX == isFlippedX())
    {
        // 如果是同一个技能正在释放，并且面向同一个方向，则不用重新施法
        return 0;
    }

    if (bFlippedX != isFlippedX() && !isFixed())
    {
        setFlippedX(bFlippedX);
    }

    return castSpell(pAbility);
}

int CUnitDraw2D::castSpell(CActiveAbility* pAbility)
{
    // 可以施展动画并作用
    if (isDoingAction(getCastActionId()))
    {
        stopAction(getCastActionId());
    }

    CUnit* u = getUnit();
    if (isDoingAction(getMoveActionId()))
    {
        stopMove();
    }

    if (pAbility->isCoolingDown())
    {
        // 该判断不能放在前面，否则攻击将会动作异常
        return -1;
    }

    float spd = 1.0f;
    if (getCastActiveAbilityId() == u->getAttackAbilityId())
    {
        CAttackAct* atk = nullptr;
        u->getActiveAbility(getCastActiveAbilityId())->dcast(atk);
        spd = atk->getBaseAttackInterval() / max(FLT_EPSILON, atk->getRealAttackInterval());
    }

    int aniId = pAbility->getCastRandomAnimation();
    if (aniId != -1)
    {
        int id = doAnimation(
            aniId,
            CC_CALLBACK_0(CUnitDraw2D::onCastEffect, this),
            1,
            CC_CALLBACK_0(CUnitDraw2D::onCastDone, this),
            spd);
        setCastActionId(id);
    }
    else
    {
        onCastEffect();
        onCastDone();
        setCastActionId(0);
    }

    return 0;
}

bool CUnitDraw2D::checkCastTargetDistance(CActiveAbility* pAbility, const CPoint& roPos, const CCommandTarget& rTarget, CUnitDraw2D* td)
{
    CPoint roPos2;
    switch (pAbility->getCastTargetType())
    {
    case CCommandTarget::kNoTarget:
        return true;

    case CCommandTarget::kUnitTarget:
        roPos2 = td->getPosition();
        break;

    case CCommandTarget::kPointTarget:
        roPos2 = rTarget.getTargetPoint();
        break;
    }

    if (pAbility->isCastHorizontal() && fabs(roPos.y - roPos2.y) > CActiveAbility::CONST_MAX_HOR_CAST_Y_RANGE)
    {
        return false;
    }

    float fDis = roPos.getDistance(roPos2) - getHalfOfWidth() - (td != nullptr ? td->getHalfOfWidth() : 0.0f);
    if (fDis < pAbility->getCastMinRange() || fDis > pAbility->getCastRange())
    {
        return false;
    }

    return true;
}

void CUnitDraw2D::moveToCastPosition(CActiveAbility* pAbility, CUnitDraw2D* td)
{
    float fDis = (td != nullptr ? td->getHalfOfWidth() : 0.0f) + getHalfOfWidth() + (pAbility->getCastMinRange() + pAbility->getCastRange()) * 0.5;
    const CPoint& roPos1 = getPosition();
    const CPoint& roPos2 = (td != nullptr ? td->getPosition() : getCastTarget().getTargetPoint());

    if (pAbility->isCastHorizontal())
    {
        // 近战施法位置修正
        move(CPoint(roPos2.x + ((roPos1.x > roPos2.x) ? fDis : -fDis), roPos2.y));
    }
    else
    {
        // 远程施法位置修正
        //float fA = -(roPos1 - roPos2).getAngle();
        //move(roPos2 + CPoint(cos(fA) * fDis, sin(-fA) * fDis));
        move(roPos2.getForwardPoint(roPos1, fDis));
    }
}

void CUnitDraw2D::onCastEffect()
{
    CUnit* u = getUnit();
    CActiveAbility* pAbility = u->getActiveAbility(getCastActiveAbilityId());
    if (pAbility == nullptr)
    {
        return;
    }

    LOG("%s%s%s..", getUnit()->getName(), getUnit()->getAttackAbilityId() == pAbility->getId() ? "的" : "施放了", pAbility->getName());
    pAbility->effect();
}

void CUnitDraw2D::onCastDone()
{
    CUnit* u = getUnit();
    CActiveAbility* pAbility = u->getActiveAbility(getCastActiveAbilityId());
    if (pAbility == nullptr)
    {
        return;
    }

    if (u->getAttackAbilityId() != 0 && pAbility->getId() == u->getAttackAbilityId())
    {
        // 拥有攻击技能，正在释放的技能就是攻击技能
        setFrame(kFrmDefault);
        setCastActionId(0);
        return;
    }

    int iCastedActiveAbilityId = getCastActiveAbilityId();
    stopCast();

    if (iCastedActiveAbilityId != u->getAttackAbilityId() ||
        cmdCastSpell(getCastTarget(), iCastedActiveAbilityId, u->isDoingOr(CUnit::kObstinate)) < 0)
    {
        // 如果刚释放完毕的技能不是攻击技能 或 攻击技能无法继续释放在同一个目标身上，则
        // 施法(非攻击)结束，去除固执状态
        u->endDoing(CUnit::kObstinate);
    }
}

void CUnitDraw2D::stopCast(bool bDefaultFrame)
{
    stopAction(getCastActionId());
    setCastActionId(0);
    setCastActiveAbilityId(0);

    CUnit* u = getUnit();
    u->endDoing(CUnit::kCasting);
    
    if (bDefaultFrame)
    {
        setFrame(kFrmDefault);
    }
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

    CUnit* u = getUnit();
    u->endDoing(CUnit::kMoving);

    stopCast(false);

    if (bDefaultFrame)
    {
        setFrame(kFrmDefault);
    }
}

void CUnitDraw2D::die()
{
    stop();
    CUnit* u = getUnit();
    u->startDoing(CUnit::kDying);
    stopAllActions();
    doAnimation(
        kAniDie,
        nullptr,
        1,
        CC_CALLBACK_0(CUnitDraw2D::onDyingDone, this),
        1.0f);
}

void CUnitDraw2D::onDyingDone()
{
    CUnit* u = getUnit();
    CWorld* w = u->getWorld();
    if (u->isDead() == false)
    {
        return;
    }

    u->endDoing(0xFFFFFFFF);
    int id = u->getId();

    u->onDead();
    //     if (w->getUnitToRevive(id) != nullptr)
    //     {
    //         // !!! 只有能重生的单位才会触发此事件，不合理
    //         u->onDead();
    //     }

    if (u->isDead())
    {
        w->delUnit(id, u->isRevivable());
    }
}

// CUnitPath
CUnitPath::CUnitPath()
{
    m_vecPoints.clear();
}

CUnitPath::CUnitPath(const VEC_POINTS& roVecPoints)
{
    m_vecPoints = roVecPoints;
}

void CUnitPath::addPoint(const CPoint& roPos)
{
    m_vecPoints.push_back(roPos);
}

const CPoint* CUnitPath::getCurTargetPoint(uint32_t dwCurPos)
{
    if (dwCurPos < m_vecPoints.size())
    {
        return &m_vecPoints[dwCurPos];
    }
    return nullptr;
}

bool CUnitPath::arriveCurTargetPoint(uint32_t& rCurPos)
{
    if (rCurPos < m_vecPoints.size())
    {
        ++rCurPos;
    }

    if (rCurPos < m_vecPoints.size())
    {
        return false;
    }

    return true;
}

// CUnitGroup
CUnitGroup::CUnitGroup()
{
    setDbgClassName("CUnitGroup");
}

CUnitGroup::CUnitGroup(CWorld* pWorld, const CPoint& roPos, float fRadius, int iMaxCount /*= INFINITE*/, const FUNC_MATCH& match /*= nullptr*/)
{
    setDbgClassName("CUnitGroup");

    if (fRadius < FLT_EPSILON)
    {
        return;
    }

    CWorld::MAP_UNITS& units = pWorld->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if (m_vecUnits.size() >= (size_t)iMaxCount)
        {
            return;
        }
        if (d->getPosition().getDistance(roPos) < fRadius && (!match || (match(u))))
        {
            m_vecUnits.addObject(u);
        }

        M_MAP_NEXT;
    }
}

CUnitGroup::CUnitGroup(CWorld* pWorld, int iMaxCount /*= INFINITE*/, const FUNC_MATCH& match /*= nullptr*/)
{
    setDbgClassName("CUnitGroup");

    CWorld::MAP_UNITS& units = pWorld->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        if ((int)m_vecUnits.size() >= (size_t)iMaxCount)
        {
            return;
        }

        if (!match || (match(u)))
        {
            m_vecUnits.addObject(u);
        }

        M_MAP_NEXT;
    }
}

CUnit* CUnitGroup::getUnitByIndex(int iIndex)
{
    if (iIndex < 0 || iIndex >= (int)m_vecUnits.size())
    {

        return nullptr;
    }

    return m_vecUnits[iIndex];
}

CUnit* CUnitGroup::getRandomUnit()
{
    return m_vecUnits.empty() ? nullptr : m_vecUnits[rand() % m_vecUnits.size()];
}

CUnit* CUnitGroup::getNearestUnitInRange(const CPoint& roPos, float fRadius, const FUNC_MATCH& match /*= nullptr*/)
{
    CUnit* pTarget = nullptr;
    float fMinDis = FLT_MAX;
    float fDis;

    for (auto& u : m_vecUnits)
    {
        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if ((fDis = d->getPosition().getDistance(roPos)) < fRadius && fMinDis > fDis && (!match || (match(u))))
        {
            pTarget = u;
            fMinDis = fDis;
        }
    }

    return pTarget;
}

void CUnitGroup::addUnit(CUnit* pUnit)
{
    m_vecUnits.addObject(pUnit);
}

CUnit* CUnitGroup::getNearestUnitInRange(CWorld* pWorld, const CPoint& roPos, float fRadius, const FUNC_MATCH& match /*= nullptr*/)
{
    CUnit* pTarget = nullptr;
    float fMinDis = FLT_MAX;
    float fDis;

    CWorld::MAP_UNITS& units = pWorld->getUnits();
    M_MAP_FOREACH(units)
    {
        CUnit* u = M_MAP_EACH;
        if (u->isGhost())
        {
            M_MAP_NEXT;
            continue;
        }

        CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
        if ((fDis = d->getPosition().getDistance(roPos)) < fRadius && fMinDis > fDis && (!match || (match(u))))
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

void CUnitGroup::damaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask)
{
    CAttackData* ad = nullptr;
    M_VEC_FOREACH(m_vecUnits)
    {
        auto u = M_VEC_EACH;
        M_VEC_NEXT;
        ad = ad ? ad->copy() : pAttack;
        u->damaged(ad, pSource, dwTriggerMask);
    }
}

CUnitGroup::FUNC_MATCH CUnitGroup::matchLivingAlly(CUnitForce* ref)
{
    return bind(&CUnitGroup::funcMatchLivingAlly, placeholders::_1, ref);
}

CUnitGroup::FUNC_MATCH CUnitGroup::matchLivingEnemy(CUnitForce* ref)
{
    return bind(&CUnitGroup::funcMatchLivingEnemy, placeholders::_1, ref);
}

CUnitGroup::FUNC_MATCH CUnitGroup::matchLivingAlly(CUnit* ref)
{
    return bind(&CUnitGroup::funcMatchLivingAlly, placeholders::_1, DCAST(ref, CUnitForce*));
}

CUnitGroup::FUNC_MATCH CUnitGroup::matchLivingEnemy(CUnit* ref)
{
    return bind(&CUnitGroup::funcMatchLivingEnemy, placeholders::_1, DCAST(ref, CUnitForce*));
}

bool CUnitGroup::funcMatchLivingAlly(CUnit* pUnit, CUnitForce* pParam)
{
    return !pUnit->isDead() && pUnit->isMyAlly(pParam);
}

bool CUnitGroup::funcMatchLivingEnemy(CUnit* pUnit, CUnitForce* pParam)
{
    return !pUnit->isDead() && pParam->isMyEnemy(pUnit);
}

// CProjectile
CProjectile::CProjectile()
: m_pWorld(nullptr)
, m_fHeight(0.0f)
, m_fMoveSpeed(300.0f)
, m_fMaxHeightDelta(0.0f)
, m_iSrcUnit(0)
, m_iFromUnit(0)
, m_iToUnit(0)
, m_pAttackData(nullptr)
, m_dwTriggerMask(CUnit::kNoMasked)
, m_pSrcAbility(nullptr)
, m_dwEffectiveTypeFlags(CUnitForce::kEnemy)
, m_eFromToType(kPointToPoint)
, m_eFireType(kFireFollow)
, m_iContactLeft(-1)
{
    setDbgClassName("CProjectile");
}

CProjectile::~CProjectile()
{
    setAttackData(nullptr);
    setSrcAbility(nullptr);
}

CMultiRefObject* CProjectile::copy()
{
    CProjectile* ret = new CProjectile;
    ret->copyData(ret);
    return ret;
}

void CProjectile::copyData(const CProjectile* from)
{
    setMoveSpeed(from->getMoveSpeed());
    setMaxHeightDelta(from->getMaxHeightDelta());
    setSrcUnit(from->getSrcUnit());
    setEffectiveTypeFlags(from->getEffectiveTypeFlags());
    setPenaltyFlags(from->getPenaltyFlags());
    setFromToType(from->getFromToType());
    setFireType(from->getFireType());
    m_vecFireSounds = from->m_vecFireSounds;
    m_vecEffectSounds = from->m_vecEffectSounds;
}

float CProjectile::getRadius() const
{
    return 0.0f;
}

int CProjectile::doLinkUnitToUnit(CUnit* pFromUnit, CUnit* pToUnit, ANI_ID id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone)
{
    return 0;
}

int CProjectile::doMoveToUnit(CUnit* pToUnit, bool bFixRotation, float fMaxHeightDelta, float fDuration, const FUNC_VOID& onMoveToDone)
{
    return 0;
}

int CProjectile::doMoveTo(const CPoint& rPos, float fDuration, const FUNC_VOID& onMoveToDone)
{
    return 0;
}

int CProjectile::doAnimation(int id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone)
{
    return 0;
}

void CProjectile::stopAction(int tag)
{
}

bool CProjectile::isDoingAction(int id)
{
    return false;
}

void CProjectile::stopAllActions()
{
}

void CProjectile::setVisible(bool bVisible /*= true*/)
{
}

void CProjectile::die()
{
    stopAllActions();

    doAnimation(
        kAniDie,
        hasPenaltyType(kOnDying) ? CC_CALLBACK_0(CProjectile::onEffect, this) : FUNC_VOID(),
        1,
        CC_CALLBACK_0(CProjectile::onDyingDone, this));
}

void CProjectile::effect(CUnit* pTarget)
{
    CWorld* w = getWorld();
    assert(w != nullptr);
    CUnit* s = w->getUnit(getSrcUnit());
    if (s == nullptr)
    {
        return;
    }

    if (pTarget != nullptr && getAttackData() != nullptr && pTarget->canEffect(DCAST(s, CUnitForce*), getEffectiveTypeFlags()))
    {
        pTarget->damaged(getAttackData(), s, getTriggerMask());
        decContactLeft();
    }

    if (getSrcAbility() != nullptr)
    {
        getSrcAbility()->playEffectSound();
        getSrcAbility()->onUnitAbilityEffect(this, pTarget);
    }

    s->onProjectileEffect(this, pTarget);
}

void CProjectile::step(float dt)
{
    onTick(dt);
}

void CProjectile::onTick(float dt)
{
    if (hasPenaltyType(kOnContact))
    {
        CWorld::MAP_UNITS& units = getWorld()->getUnits();
        M_MAP_FOREACH(units)
        {
            CUnit* u = M_MAP_EACH;
            CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
            if (d->getPosition().getDistance(getPosition()) - d->getHalfOfWidth() - getRadius() <= 0 && m_mapContactedUnits.getObject(u->getId()) == nullptr)
            {
                if (u->onProjectileArrive(this) == false)
                {
                    M_MAP_NEXT;
                    continue;
                }

                m_mapContactedUnits.addObject(u);
                effect(u);

                if (m_iContactLeft == 0)
                {
                    m_dwPenaltyFlags &= (~kOnContact);
                    die();
                    return;
                }
            }
            M_MAP_NEXT;
        }
    }
}

void CProjectile::onMoveDone()
{
    if (getFromToType() == kPointToUnit || getFromToType() == kUnitToUnit)
    {
        CWorld* w = getWorld();
        assert(w != nullptr);
        CUnit* t = w->getUnit(getToUnit());
        if (t != nullptr && t->onProjectileArrive(this) == false)
        {
            return;
        }
    }

    die();
}

void CProjectile::onEffect()
{
    playEffectSound();
    CUnit* t = (getFromToType() == kPointToUnit || getFromToType() == kUnitToUnit) ? getWorld()->getUnit(getToUnit()) : nullptr;
    effect(t);
}

void CProjectile::onDyingDone()
{
    stopAllActions();

    CWorld* w = getWorld();
    assert(w != nullptr);

    w->delProjectile(getId());
}

void CProjectile::setAttackData(CAttackData* pAttackData)
{
    if (pAttackData == m_pAttackData)
    {
        return;
    }

    M_SAFE_RETAIN(pAttackData);
    M_SAFE_RELEASE(m_pAttackData);
    m_pAttackData = pAttackData;
}

void CProjectile::setSrcAbility(CAbility* pSrcAbility)
{
    if (pSrcAbility == m_pSrcAbility)
    {
        return;
    }

    M_SAFE_RETAIN(pSrcAbility);
    M_SAFE_RELEASE(m_pSrcAbility);
    m_pSrcAbility = pSrcAbility;
}

bool CProjectile::hasPenaltyType(PENALTY_FLAG_BIT ePenaltyType) const
{
    return (m_dwPenaltyFlags & (uint32_t)ePenaltyType) != 0;
}

void CProjectile::fireFollow(const CPoint& rFromPoint, int iToUnit, float fDuration, float fMaxHeightDelta)
{
    //setFromToType(kPointToUnit);

    CWorld* w = getWorld();
    assert(w != nullptr);

    setFromPoint(rFromPoint);
    CUnit* t = w->getUnit(iToUnit);
    assert(t != nullptr);
    setToUnit(iToUnit);

    setPosition(rFromPoint);

    stopAllActions();

    doMoveToUnit(
        t,
        true,
        fMaxHeightDelta,
        fDuration,
        CC_CALLBACK_0(CProjectile::onMoveDone, this));

    doAnimation(
        kAniMove,
        nullptr,
        INFINITE,
        nullptr);
}

void CProjectile::fireLink(int iFromUnit, int iToUnit)
{
    //setFromToType(kUnitToUnit);

    CWorld* w = getWorld();
    assert(w != nullptr);

    setFromUnit(iFromUnit);
    setToUnit(iToUnit);
    
    CUnit* u = w->getUnit(iFromUnit);
    CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);
    
    CUnit* t = w->getUnit(iToUnit);
    CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);
    
    assert(u != nullptr && t != nullptr && d != nullptr && td != nullptr);

    setFromPoint(d->getPosition());
    setToPoint(td->getPosition());

    stopAllActions();

    doLinkUnitToUnit(
        u,
        t,
        kAniDie,
        hasPenaltyType(kOnDying) ? CC_CALLBACK_0(CProjectile::onEffect, this) : FUNC_VOID(),
        1,
        CC_CALLBACK_0(CProjectile::onDyingDone, this));
}

void CProjectile::fireLink(int iFromUnit, const CPoint& rToPoint)
{

}

void CProjectile::fireLink(const CPoint& rFromPoint, int iToUnit)
{

}

void CProjectile::fireLink(const CPoint& rFromPoint, const CPoint& rToPoint)
{

}

void CProjectile::fireStraight(const CPoint& rFromPoint, const CPoint& rToPoint, float fDuration, float fMaxHeightDelta)
{
    //setFromToType(kPointToPoint);
    m_mapContactedUnits.clear();

    CWorld* w = getWorld();
    assert(w != nullptr);

    setFromPoint(rFromPoint);
    setToPoint(rToPoint);

    setPosition(rFromPoint);

    stopAllActions();

    doMoveTo(
        rToPoint,
        fDuration,
        CC_CALLBACK_0(CProjectile::onMoveDone, this));

    doAnimation(
        kAniMove,
        nullptr,
        INFINITE,
        nullptr);
}

void CProjectile::fire()
{
    playFireSound();
    CWorld* w = getWorld();

    switch (getFireType())
    {
    case kFireFollow:
        {
            assert(getFromToType() == kUnitToUnit || getFromToType() == kPointToUnit);

            if (getFromToType() == kUnitToUnit)
            {
                CUnit* u = w->getUnit(getFromUnit());
                CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);

                setHeight(d->getHeight() + d->getFirePoint().y);
                setFromPoint(CPoint(d->getPosition() + CPoint(d->isFlippedX() ? -d->getFirePoint().x : d->getFirePoint().x, 0)));
            }

            CUnit* t = w->getUnit(getToUnit());
            CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);

            float fDis = getFromPoint().getDistance(td->getPosition() + CPoint(0.0f, td->getHalfOfHeight()));
            fireFollow(getFromPoint(), getToUnit(), fDis / max(FLT_EPSILON, getMoveSpeed()), getMaxHeightDelta());
        }

        break;

    case kFireLink:
        {
            switch (getFromToType())
            {
            case kUnitToUnit:
                fireLink(getFromUnit(), getToUnit());
                break;
                
            default:
                assert(false);
            }
            
        }
        break;

    case kFireStraight:
        {
            assert(getFromToType() == kUnitToPoint || getFromToType() == kPointToPoint);

            if (getFromToType() == kUnitToPoint)
            {
                CUnit* u = w->getUnit(getFromUnit());
                CUnitDraw2D* d = DCAST(u->getDraw(), CUnitDraw2D*);

                setHeight(d->getHeight() + d->getFirePoint().y);
                setFromPoint(CPoint(d->getPosition() + CPoint(d->isFlippedX() ? -d->getFirePoint().x : d->getFirePoint().x, 0.0f)));
            }

            float fDis = getFromPoint().getDistance(getToPoint());
            fireStraight(getFromPoint(), getToPoint(), fDis / max(FLT_EPSILON, getMoveSpeed()), getMaxHeightDelta());
        }

        break;
    }
}

void CProjectile::redirect()
{
    stopAllActions();
    playFireSound();
    CWorld* w = getWorld();

    switch (getFireType())
    {
    case kFireFollow:
        {
            assert(getFromToType() == kUnitToUnit);

            setFromPoint(getPosition());

            CUnit* t = w->getUnit(getToUnit());
            if (t != nullptr)
            {
                CUnitDraw2D* td = DCAST(t->getDraw(), CUnitDraw2D*);

                float fDis = getFromPoint().getDistance(td->getPosition() + CPoint(0.0f, td->getHalfOfHeight()));
                fireFollow(getFromPoint(), getToUnit(), fDis / max(FLT_EPSILON, getMoveSpeed()), getMaxHeightDelta());
            }
        }

        break;

    case kFireLink:
        {
            switch (getFromToType())
            {
            case kUnitToUnit:
                fireLink(getFromUnit(), getToUnit());
                break;

            default:
                assert(false);
            }

        }
        break;

    case kFireStraight:
        {
            assert(getFromToType() == kUnitToPoint || getFromToType() == kPointToPoint);

            setFromPoint(getPosition());

            float fDis = getFromPoint().getDistance(getToPoint());
            fireStraight(getFromPoint(), getToPoint(), fDis / max(FLT_EPSILON, getMoveSpeed()), getMaxHeightDelta());
        }

        break;
    }
}

void CProjectile::addFireSound(const char* pSounds)
{
    m_vecFireSounds.push_back(pSounds);
}

void CProjectile::addEffectSound(const char* pSounds)
{
    m_vecEffectSounds.push_back(pSounds);
}

void CProjectile::playFireSound()
{
}

void CProjectile::playEffectSound()
{
}

void CProjectile::decContactLeft(int dec)
{
    if (m_iContactLeft > 0)
    {
        m_iContactLeft -= dec;
    }
}
