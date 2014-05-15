#include "CommHeader.h"

#include "ActionForCC.h"
#include "DrawForCC.h"
#include "Draw.h"
#include "GameControl.h"


// NotifyAnimate
NotifyAnimate::NotifyAnimate()
: m_iNotifyFrameIndex(0)
, m_bNotified(false)
{
}

NotifyAnimate::~NotifyAnimate()
{
}

bool NotifyAnimate::initWithAnimation(Animation* animation, int notifyFrameIndex, const FUNC_PNODE& func, const char* sound)
{
    if (!Animate::initWithAnimation(animation))
    {
        return false;
    }

    m_iNotifyFrameIndex = notifyFrameIndex;
    m_func = func;

    m_bNotified = false;

    if (sound != nullptr)
    {
        m_sSound = sound;
    }
    
    return true;
}

void NotifyAnimate::update(float t)
{
    // if t==1, ignore. Animation should finish with t==1
    if (t < 1.0f) {
        t *= _animation->getLoops();

        // new loop?  If so, reset frame counter
        unsigned int loopNumber = (unsigned int)t;
        if (loopNumber > _executedLoops) {
            _nextFrame = 0;
            _executedLoops++;
            m_bNotified = false;
        }

        // new t for animations
        t = fmodf(t, 1.0f);
    }

    const Vector<AnimationFrame*>& frames = _animation->getFrames();
    auto numberOfFrames = frames.size();
    SpriteFrame *frameToDisplay = nullptr;

    M_DEF_GC(gc);
    for (int i = _nextFrame; i < numberOfFrames; i++) {
        float splitTime = _splitTimes->at(i);

        if (splitTime <= t) {
            AnimationFrame* frame = frames.at(i);
            frameToDisplay = frame->getSpriteFrame();
            static_cast<Sprite*>(_target)->setSpriteFrame(frameToDisplay);

            const ValueMap& dict = frame->getUserInfo();
            if (!dict.empty())
            {
                //TODO: [[NSNotificationCenter defaultCenter] postNotificationName:AnimationFrameDisplayedNotification object:target_ userInfo:dict];
            }
            _nextFrame = i + 1;

            if (i == 0 && m_iNotifyFrameIndex < 0 && m_sSound.empty() == false)
            {
                gc->playSound(m_sSound.c_str());
            }

            if ((int)i >= m_iNotifyFrameIndex && m_bNotified == false)
            {
                if (m_sSound.empty() == false)
                {
                    gc->playSound(m_sSound.c_str());
                }
                m_bNotified = true;
                if (m_func)
                {
                    m_func(_target);
                }
            }
        }
        // Issue 1438. Could be more than one frame per tick, due to low frame rate or frame delta < 1/FPS
        else {
            break;
        }
    }
}

// FadeInOutScale4
bool FadeInOutScale4::init(float fScaleStart, float fScaleMid, float fScaleNormal, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd)
{
    m_fScaleStart = fScaleStart;
    ActionInterval* pAct0 = Spawn::createWithTwoActions(FadeIn::create(fDurToMid), ScaleTo::create(fDurToMid, fScaleMid));
    ActionInterval* pAct1 = ScaleTo::create(fDurToNormal, fScaleNormal);
    ActionInterval* pAct2 = DelayTime::create(fDurKeep);
    ActionInterval* pAct3 = Spawn::createWithTwoActions(FadeOut::create(fDurToEnd), ScaleTo::create(fDurToEnd, fScaleEnd));

    return Sequence::initWithTwoActions(Sequence::create(pAct0, pAct1, pAct2, nullptr), pAct3);
}

void FadeInOutScale4::startWithTarget(Node *pTarget)
{
    Sequence::startWithTarget(pTarget);
    pTarget->setScale(m_fScaleStart);
}

// SequenceEx
SequenceEx* SequenceEx::createWithTwoActions(FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo)
{
    SequenceEx *pSequence = new SequenceEx();
    pSequence->initWithTwoActions(pActionOne, pActionTwo);
    pSequence->autorelease();

    return pSequence;
}

FiniteTimeAction* SequenceEx::getActionOne()
{
    return _actions[0];
}

FiniteTimeAction* SequenceEx::getActionTwo()
{
    return _actions[1];
}

// MoveToNode
MoveToNode::MoveToNode()
: m_pToNode(nullptr)
, m_eFromType(kNormal)
, m_eToType(kNormal)
, m_fMinSpeed(0.0f)
, m_fFromHeight(0.0f)
, m_fToHeight(0.0f)
, m_fDeltaHeight(0.0f)
, m_fMaxHeightDelta(0.0f)
, m_fA(0.0f)
, m_bFixRotation(false)
{
}

MoveToNode::~MoveToNode()
{
    if (m_pToNode != nullptr)
    {
        switch (m_eToType)
        {
        case kNormal:
            m_pToNode->release();
            break;

        case kDraw:
            m_pToDraw->release();
            break;

        case kProjectile:
            m_pToProjectile->release();
            break;
        }
    }
}

bool MoveToNode::initWithNode(float fDuration, Node* pToNode, bool bFixRotation, float fMaxHeightDelta /*= 0.0f*/)
{
    if (ActionInterval::initWithDuration(fDuration) == false)
    {
        return false;
    }

    assert(pToNode != nullptr);
    m_eToType = kNormal;
    pToNode->retain();
    m_pToNode = pToNode;


    m_bFixRotation = bFixRotation;
    m_fMaxHeightDelta = fMaxHeightDelta;
    m_fMinSpeed = 0;

    return true;
}

bool MoveToNode::initWithDraw(float fDuration, CUnitDrawForCC* pToDraw, bool bFixRotation /*= true*/, float fMaxHeightDelta /*= 0.0f*/)
{
    if (ActionInterval::initWithDuration(fDuration) == false)
    {
        return false;
    }

    assert(pToDraw != nullptr);
    m_eToType = kDraw;
    pToDraw->retain();
    m_pToDraw = pToDraw;

    m_bFixRotation = bFixRotation;
    m_fMaxHeightDelta = fMaxHeightDelta;
    m_fMinSpeed = 0;

    return true;
}

bool MoveToNode::initWithProjectile(float fDuration, CProjectileForCC* pToProjectile, bool bFixRotation /*= true*/, float fMaxHeightDelta /*= 0.0f*/)
{
    if (ActionInterval::initWithDuration(fDuration) == false)
    {
        return false;
    }

    assert(pToProjectile != nullptr);
    m_eToType = kProjectile;
    pToProjectile->retain();
    m_pToProjectile = pToProjectile;

    m_bFixRotation = bFixRotation;
    m_fMaxHeightDelta = fMaxHeightDelta;
    m_fMinSpeed = 0;

    return true;
}

MoveToNode* MoveToNode::clone() const
{
    MoveToNode* ret = nullptr;
    switch (m_eToType)
    {
    case kNormal:
        ret = MoveToNode::createWithNode(getDuration(), m_pToNode, m_bFixRotation, m_fMaxHeightDelta);
        break;

    case kDraw:
        ret = MoveToNode::createWithDraw(getDuration(), m_pToDraw, m_bFixRotation, m_fMaxHeightDelta);
        break;

    case kProjectile:
        ret = MoveToNode::createWithProjectile(getDuration(), m_pToProjectile, m_bFixRotation, m_fMaxHeightDelta);
        break;
    }

    return ret;
}

void MoveToNode::startWithTarget(Node *pTarget)
{
    ActionInterval::startWithTarget(pTarget);

    assert(pTarget != nullptr);
    if (DCAST(pTarget, ProjectileSprite*) != nullptr)
    {
        m_eFromType = kProjectile;
        CProjectileForCC* d = ((ProjectileSprite*)pTarget)->getProjectile();
        const CPoint& p = d->getPosition();
        m_oStartPos.setPoint(p.x, p.y);
        m_fFromHeight = d->getHeight();
    } 
    else if (DCAST(pTarget, UnitSprite*) != nullptr)
    {
        m_eFromType = kDraw;
        CUnitDrawForCC* d = ((UnitSprite*)pTarget)->getDraw();
        const CPoint& p = d->getPosition();
        m_oStartPos.y = p.y;
        m_oStartPos.x = p.x + (d->isFlippedX() ? -d->getFirePoint().x : d->getFirePoint().x);
        m_fFromHeight = d->getHeight() + d->getFirePoint().y;
    }
    else
    {
        m_eFromType = kNormal;
        m_oStartPos = pTarget->getPosition();
        m_fFromHeight = 0.0f;
    }

    m_fMinSpeed = sqrt(m_oDeltaPos.x * m_oDeltaPos.x + m_oDeltaPos.y * m_oDeltaPos.y) / getDuration();
}

void MoveToNode::update(float time)
{
    if (getTarget() == nullptr)
    {
        return;
    }

    switch (m_eToType)
    {
    case kNormal:
        m_oEndPos = m_pToNode->getPosition();
        m_fToHeight = 0.0f;
        break;

    case kDraw:
        {
            const CPoint& p = m_pToDraw->getPosition();
            m_oEndPos.setPoint(p.x, p.y);
            m_fToHeight = m_pToDraw->getHeight() + m_pToDraw->getHalfOfHeight();
        }
        break;

    case kProjectile:
        {
            const CPoint& p = m_pToProjectile->getPosition();
            m_oEndPos.setPoint(p.x, p.y);
            m_fToHeight = m_pToProjectile->getHeight();
        }
        break;
    }

    m_oDeltaPos = m_oEndPos - m_oStartPos;
    m_fDeltaHeight = m_fToHeight - m_fFromHeight;

    setDuration(sqrt(m_oDeltaPos.x * m_oDeltaPos.x + m_oDeltaPos.y * m_oDeltaPos.y) / m_fMinSpeed);

    // 抛物线
    float fA = m_oStartPos.getDistance(m_oEndPos);
    float fX = time * fA - fA / 2;
    fA = -4 * m_fMaxHeightDelta / (fA * fA);
    float fHeightDelta = fA * fX * fX + m_fMaxHeightDelta;

    //m_pTarget->setPosition(Point(m_oStartPos.x + m_oDeltaPos.x * time, m_oStartPos.y + m_oDeltaPos.y * time));
    switch (m_eFromType)
    {
    case kNormal:
        getTarget()->setPosition(Point(m_oStartPos.x + m_oDeltaPos.x * time, m_oStartPos.y + m_oDeltaPos.y * time));
        getTarget()->setLocalZOrder(M_BASE_Z + m_fFromHeight + m_fDeltaHeight * time + fHeightDelta - getTarget()->getPosition().y);
        break;

    case kDraw:
        {
            CUnitDrawForCC* d = ((UnitSprite*)getTarget())->getDraw();
            d->setPosition(CPoint(m_oStartPos.x + m_oDeltaPos.x * time, m_oStartPos.y + m_oDeltaPos.y * time));
            d->setHeight(m_fFromHeight + m_fDeltaHeight * time + fHeightDelta);
        }
        break;

    case kProjectile:
        {
            CProjectileForCC* d = ((ProjectileSprite*)getTarget())->getProjectile();
            d->setPosition(CPoint(m_oStartPos.x + m_oDeltaPos.x * time, m_oStartPos.y + m_oDeltaPos.y * time));
            d->setHeight(m_fFromHeight + m_fDeltaHeight * time + fHeightDelta);
        }
        break;
    }

    if (m_bFixRotation)
    {
        // 修正角度
        float fOffsetR = atan(fA * fX);
        getTarget()->setRotation(CC_RADIANS_TO_DEGREES((m_oStartPos.x > m_oEndPos.x ? fOffsetR : -fOffsetR) - (m_oEndPos - m_oStartPos).getAngle()));
    }
    //m_pTarget->setRotation(CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(m_oEndPos, m_oStartPos))) + (m_oStartPos.x > m_oEndPos.x ? CC_RADIANS_TO_DEGREES(fOffsetR) : -CC_RADIANS_TO_DEGREES(fOffsetR)));
    //CCLOG("%.2f, %.2f", CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(m_oEndPos, m_oStartPos))), (m_oStartPos.x > m_oEndPos.x ? CC_RADIANS_TO_DEGREES(fOffsetR) : CC_RADIANS_TO_DEGREES(fOffsetR)));
}

MoveToNode* MoveToNode::reverse() const
{
    MoveToNode* ret = NULL;
    assert(false);
    return ret;
}

// LinkAnimate
LinkAnimate::LinkAnimate()
: m_eFromType(kDraw)
, m_eToType(kDraw)
, m_eTargetType(kProjectile)
, m_pFromNode(nullptr)
, m_pToNode(nullptr)
, m_bFireFrom(false)
{
}

LinkAnimate::~LinkAnimate()
{
    if (m_pFromNode != nullptr)
    {
        switch (m_eFromType)
        {
        case kNormal:
            m_pFromNode->release();
            break;

        case kDraw:
            m_pFromDraw->release();
            break;

        case kProjectile:
            m_pFromProjectile->release();
            break;
        }
    }

    if (m_pToNode != nullptr)
    {
        switch (m_eToType)
        {
        case kNormal:
            m_pToNode->release();
            break;

        case kDraw:
            m_pToDraw->release();
            break;

        case kProjectile:
            m_pToProjectile->release();
            break;
        }
    }
}

bool LinkAnimate::initWithDrawToDraw(Animation* pAnimation, int iNotifyFrameIndex, const FUNC_PNODE& func, CUnitDrawForCC* pFromDraw, CUnitDrawForCC* pToDraw)
{
    if (NotifyAnimate::initWithAnimation(pAnimation, iNotifyFrameIndex, func) == false)
    {
        return false;
    }

    assert(pFromDraw != nullptr);
    m_eFromType = kDraw;
    pFromDraw->retain();
    m_pFromDraw = pFromDraw;

    assert(pToDraw != nullptr);
    m_eToType = kDraw;
    pToDraw->retain();
    m_pToDraw = pToDraw;

    return true;
}

LinkAnimate* LinkAnimate::clone() const
{
    LinkAnimate* ret = nullptr;

    switch (m_eFromType)
    {
    case kDraw:
        switch (m_eToType)
        {
        case kDraw:
            ret = LinkAnimate::createWithDrawToDraw((Animation*)getAnimation()->clone(), m_iNotifyFrameIndex, m_func, m_pFromDraw, m_pToDraw);
            break;

        }
        break;

    }

    return ret;
}

void LinkAnimate::startWithTarget(Node *pTarget)
{
    NotifyAnimate::startWithTarget(pTarget);

    if (DCAST(pTarget, ProjectileSprite*) != nullptr)
    {
        m_eTargetType = kProjectile;
        
        ProjectileSprite* sp = DCAST(getTarget(), ProjectileSprite*);
        CProjectileForCC* p = sp->getProjectile();
        if (m_eFromType == kDraw && m_pFromDraw->getUnit()->getId() == p->getSrcUnit())
        {
            m_bFireFrom = true;
        }
    } 
    else if (DCAST(pTarget, Node*) != nullptr)
    {
        m_eTargetType = kNormal;
    }
    else
    {
        assert(false);
    }

    pTarget->setVisible(false);
}

void LinkAnimate::update(float t)
{
    if (getTarget())
    {
        if (getTarget()->isVisible() == false)
        {
            getTarget()->setVisible(true);
        }
        fixTargetPosition(getTarget());
    }

    NotifyAnimate::update(t);
}

void LinkAnimate::fixTargetPosition(Node* pTarget)
{
    float fFromOffsetX = 0.0f;
    float fFromHeight = 0.0f;
    bool bFlipX = m_oFromPoint.x > m_oToPoint.x;

    float fToHeight = 0.0f;

    switch (m_eFromType)
    {
    case kNormal:
        m_oFromPoint = m_pFromNode->getPosition();
        break;

    case kDraw:
        {
            m_oFromPoint = m_pFromDraw->getSprite()->getPosition();
            if (m_bFireFrom)
            {
                // from端为发射端
                fFromOffsetX = (bFlipX ? -m_pFromDraw->getFirePoint().x : m_pFromDraw->getFirePoint().x);
                fFromHeight = m_pFromDraw->getHeight() + m_pFromDraw->getFirePoint().y;
            }
            else
            {
                fFromHeight = m_pFromDraw->getHeight() + m_pFromDraw->getHalfOfHeight();
            }
        }

        break;

    case kProjectile:
        {
            m_oFromPoint = m_pFromProjectile->getSprite()->getPosition();
            fFromHeight = m_pFromProjectile->getHeight();
        }

        break;
    }

    switch (m_eToType)
    {
    case kNormal:
        m_oToPoint = m_pToNode->getPosition();
        break;

    case kDraw:
        {
            m_oToPoint = m_pToDraw->getSprite()->getPosition();
            fToHeight = m_pToDraw->getHeight() + m_pToDraw->getHalfOfHeight();
        }

        break;

    case kProjectile:
        {
            m_oToPoint = m_pToProjectile->getSprite()->getPosition();
            fToHeight = m_pToProjectile->getHeight();
        }

        break;
    }
    
    m_oFromPoint.x += fFromOffsetX;
    m_oFromPoint.y += fFromHeight;

    m_oToPoint.y += fToHeight;

    if (m_eTargetType == kProjectile)
    {
        ((ProjectileSprite*)getTarget())->getProjectile()->setHeight((fFromHeight + fToHeight) / 2);
    }
    
    Point oDelta = m_oToPoint - m_oFromPoint;
    float fR = CC_RADIANS_TO_DEGREES(-oDelta.getAngle());
    
    float fScale = sqrt(oDelta.x * oDelta.x + oDelta.y * oDelta.y) / pTarget->getContentSize().width;
    pTarget->setPosition(Point((m_oFromPoint.x + m_oToPoint.x) / 2, (m_oFromPoint.y + m_oToPoint.y) / 2));
    pTarget->setRotation(fR);
    pTarget->setScaleX(fScale);
}

// Shake
Shake::Shake()
{
}

bool Shake::init(float fDuration, int iTimes, float fRange)
{
    float fDur = fDuration / max(iTimes, 1) / 4;
    
    return Repeat::initWithAction(
        Sequence::create(
            MoveBy::create(fDur, Point(fRange, 0.0f)),
            MoveBy::create(fDur, Point(0.0f, -fRange)),
            MoveBy::create(fDur, Point(-fRange, 0.0f)),
            MoveBy::create(fDur, Point(0.0f, fRange)),
            nullptr),
        iTimes);
}

void Shake::startWithTarget(Node* pTarget)
{
    m_oLoc = pTarget->getPosition();
    Repeat::startWithTarget(pTarget);
}

void Shake::stop(void)
{
    if (getTarget())
    {
        getTarget()->setPosition(m_oLoc);
    }
    Repeat::stop();
}

// ScaleSizeTo
bool ScaleSizeTo::initWithDuration(float duration, const Size& sz)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _endScaleX = sz;

        return true;
    }

    return false;
}

ScaleSizeTo* ScaleSizeTo::clone(void) const
{
    // no copy constructor
    return ScaleSizeTo::create(_duration, _endScaleX);
}

ScaleSizeTo* ScaleSizeTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in ScaleSizeTo");
    return nullptr;
}

void ScaleSizeTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _startScaleX = target->getContentSize();
    _deltaX = _endScaleX - _startScaleX;
}

void ScaleSizeTo::update(float time)
{
    if (_target)
    {
        _target->setContentSize(_startScaleX + _deltaX * time);
    }
}
