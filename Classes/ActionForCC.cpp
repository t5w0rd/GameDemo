#include "CommHeader.h"

#include "ActionForCC.h"
#include "DrawForCC.h"
#include "Draw.h"


// CCCallFuncNMultiObj
CCCallFuncNMultiObj::~CCCallFuncNMultiObj()
{
    M_SAFE_RELEASE((CMultiRefObject*)m_pData);
}

bool CCCallFuncNMultiObj::initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncND selector, CCallFuncData* d)
{
    M_SAFE_RETAIN(d);

    return CCCallFuncND::initWithTarget(pSelectorTarget, selector, d);
}

CCObject* CCCallFuncNMultiObj::copyWithZone(CCZone *zone)
{
    CCZone* pNewZone = NULL;
    CCCallFuncNMultiObj* pRet = NULL;

    if (zone && zone->m_pCopyObject) {
        //in case of being called at sub class
        pRet = (CCCallFuncNMultiObj*) (zone->m_pCopyObject);
    } else {
        pRet = new CCCallFuncNMultiObj();
        zone = pNewZone = new CCZone(pRet);
    }

    CCCallFunc::copyWithZone(zone);
    pRet->initWithTarget(m_pSelectorTarget, m_pCallFuncND, (CCallFuncData*)m_pData);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

// CCNotifyAnimate
CCNotifyAnimate::~CCNotifyAnimate()
{
    M_SAFE_RELEASE(m_pData);
}

bool CCNotifyAnimate::initWithAnimation(CCAnimation* pAnimation, int iNotifyFrameIndex, CCObject* pSelector, SEL_CallFuncND pCallback, CCallFuncData* pData)
{
    if (!CCAnimate::initWithAnimation(pAnimation))
    {
        return false;
    }

    m_iNotifyFrameIndex = iNotifyFrameIndex;
    m_pSelector = pSelector;
    m_pCallback = pCallback;

    M_SAFE_RETAIN(pData);
    m_pData = pData;
    
    return true;
}

void CCNotifyAnimate::update(float t)
{
    // if t==1, ignore. Animation should finish with t==1
    if(t < 1.0f) {
        t *= getAnimation()->getLoops();

        // new loop?  If so, reset frame counter
        unsigned int loopNumber = (unsigned int)t;
        if(loopNumber > m_uExecutedLoops) {
            m_nNextFrame = 0;
            m_uExecutedLoops++;
        }

        // new t for animations
        t = fmodf(t, 1.0f);
    }

    CCArray* frames = getAnimation()->getFrames();
    unsigned int numberOfFrames = frames->count();
    CCSpriteFrame *frameToDisplay = NULL;

    for(unsigned int i=m_nNextFrame; i < numberOfFrames; i++) {
        float splitTime = m_pSplitTimes->at(i);

        if(splitTime <= t) {
            CCAnimationFrame* frame = (CCAnimationFrame*)frames->objectAtIndex(i);
            frameToDisplay = frame->getSpriteFrame();
            ((CCSprite*)m_pTarget)->setDisplayFrame(frameToDisplay);

            CCDictionary* dict = frame->getUserInfo();
            if(dict)
            {
                //TODO: [[NSNotificationCenter defaultCenter] postNotificationName:CCAnimationFrameDisplayedNotification object:target_ userInfo:dict];
            }
            m_nNextFrame = i+1;

            if (i == m_iNotifyFrameIndex && m_pSelector && m_pCallback)
            {
                (m_pSelector->*m_pCallback)(m_pTarget, m_pData);
            }

            break;
        }
    }
}

CCObject* CCNotifyAnimate::copyWithZone(CCZone* pZone)
{
    CCZone* pNewZone = NULL;
    CCNotifyAnimate* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCNotifyAnimate*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCNotifyAnimate();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithAnimation((CCAnimation*)getAnimation()->copy()->autorelease(), m_iNotifyFrameIndex, m_pSelector, m_pCallback, m_pData);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

// CCFadeInOutScale4
bool CCFadeInOutScale4::init(float fScaleStart, float fScaleMid, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd)
{
    m_fScaleStart = fScaleStart;
    CCActionInterval* pAct0 = CCSpawn::createWithTwoActions(CCFadeIn::create(fDurToMid), CCScaleTo::create(fDurToMid, fScaleMid));
    CCActionInterval* pAct1 = CCScaleTo::create(fDurToNormal, 1.0);
    CCActionInterval* pAct2 = CCScaleBy::create(fDurKeep, 1.0);
    CCActionInterval* pAct3 = CCSpawn::createWithTwoActions(CCFadeOut::create(fDurToEnd), CCScaleTo::create(fDurToEnd, fScaleEnd));

    return CCSequence::initWithTwoActions(CCSequence::create(pAct0, pAct1, pAct2, NULL), pAct3);
}

void CCFadeInOutScale4::startWithTarget(CCNode *pTarget)
{
    CCSequence::startWithTarget(pTarget);
    m_pTarget->setScale(m_fScaleStart);
}

// CCSequenceEx
CCSequenceEx* CCSequenceEx::createWithTwoActions( CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo )
{
    CCSequenceEx *pSequence = new CCSequenceEx();
    pSequence->initWithTwoActions(pActionOne, pActionTwo);
    pSequence->autorelease();

    return pSequence;
}

CCFiniteTimeAction* CCSequenceEx::getActionOne()
{
    return m_pActions[0];
}

CCFiniteTimeAction* CCSequenceEx::getActionTwo()
{
    return m_pActions[1];
}

// CCMoveToNode
CCMoveToNode::CCMoveToNode()
    : m_pToNode(NULL)
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

CCMoveToNode::~CCMoveToNode()
{
    if (m_pToNode != NULL)
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

bool CCMoveToNode::initWithNode(float fDuration, CCNode* pToNode, bool bFixRotation, float fMaxHeightDelta /*= 0.0f*/)
{
    if (CCActionInterval::initWithDuration(fDuration) == false)
    {
        return false;
    }

    assert(pToNode != NULL);
    m_eToType = kNormal;
    pToNode->retain();
    m_pToNode = pToNode;


    m_bFixRotation = bFixRotation;
    m_fMaxHeightDelta = fMaxHeightDelta;
    m_fMinSpeed = 0;

    return true;
}

bool CCMoveToNode::initWithDraw(float fDuration, CUnitDrawForCC* pToDraw, bool bFixRotation /*= true*/, float fMaxHeightDelta /*= 0.0f*/)
{
    if (CCActionInterval::initWithDuration(fDuration) == false)
    {
        return false;
    }

    assert(pToDraw != NULL);
    m_eToType = kDraw;
    pToDraw->retain();
    m_pToDraw = pToDraw;

    m_bFixRotation = bFixRotation;
    m_fMaxHeightDelta = fMaxHeightDelta;
    m_fMinSpeed = 0;

    return true;
}

bool CCMoveToNode::initWithProjectile(float fDuration, CProjectileForCC* pToProjectile, bool bFixRotation /*= true*/, float fMaxHeightDelta /*= 0.0f*/)
{
    if (CCActionInterval::initWithDuration(fDuration) == false)
    {
        return false;
    }

    assert(pToProjectile != NULL);
    m_eToType = kProjectile;
    pToProjectile->retain();
    m_pToProjectile = pToProjectile;

    m_bFixRotation = bFixRotation;
    m_fMaxHeightDelta = fMaxHeightDelta;
    m_fMinSpeed = 0;

    return true;
}

void CCMoveToNode::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);

    assert(pTarget != NULL);
    if (DCAST(pTarget, CCProjectileSprite*) != NULL)
    {
        m_eFromType = kProjectile;
        CProjectileForCC* d = ((CCProjectileSprite*)pTarget)->getProjectile();
        const CPoint& p = d->getPosition();
        m_oStartPos.setPoint(p.x, p.y);
        m_fFromHeight = d->getHeight();
    } 
    else if (DCAST(pTarget, CCUnitSprite*) != NULL)
    {
        m_eFromType = kDraw;
        CUnitDrawForCC* d = ((CCUnitSprite*)pTarget)->getDraw();
        const CPoint& p = d->getPosition();
        m_oStartPos.y = p.y;
        m_oStartPos.x = p.x + (d->isFlipX() ? -d->getFirePoint().x : d->getFirePoint().x);
        m_fFromHeight = d->getHeight() + d->getFirePoint().y;
    }
    else
    {
        m_eFromType = kNormal;
        m_oStartPos = pTarget->getPosition();
        m_fFromHeight = 0.0f;
    }

    m_fMinSpeed = sqrt(m_oDeltaPos.x * m_oDeltaPos.x + m_oDeltaPos.y * m_oDeltaPos.y) / m_fDuration;
}

void CCMoveToNode::update(float time)
{
    if (m_pTarget == NULL)
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

    m_oDeltaPos = ccpSub(m_oEndPos, m_oStartPos);
    m_fDeltaHeight = m_fToHeight - m_fFromHeight;

    setDuration(sqrt(m_oDeltaPos.x * m_oDeltaPos.x + m_oDeltaPos.y * m_oDeltaPos.y) / m_fMinSpeed);

    // 抛物线
    float fA = ccpDistance(m_oStartPos, m_oEndPos);
    float fX = time * fA - fA / 2;
    fA = -4 * m_fMaxHeightDelta / (fA * fA);
    float fHeightDelta = fA * fX * fX + m_fMaxHeightDelta;

    //m_pTarget->setPosition(ccp(m_oStartPos.x + m_oDeltaPos.x * time, m_oStartPos.y + m_oDeltaPos.y * time));
    switch (m_eFromType)
    {
    case kNormal:
        m_pTarget->setPosition(ccp(m_oStartPos.x + m_oDeltaPos.x * time, m_oStartPos.y + m_oDeltaPos.y * time));
        m_pTarget->setZOrder(M_BASE_Z + m_fFromHeight + m_fDeltaHeight * time + fHeightDelta - m_pTarget->getPosition().y);
        break;

    case kDraw:
        {
            CUnitDrawForCC* d = ((CCUnitSprite*)m_pTarget)->getDraw();
            d->setPosition(CPoint(m_oStartPos.x + m_oDeltaPos.x * time, m_oStartPos.y + m_oDeltaPos.y * time));
            d->setHeight(m_fFromHeight + m_fDeltaHeight * time + fHeightDelta);
        }
        break;

    case kProjectile:
        {
            CProjectileForCC* d = ((CCProjectileSprite*)m_pTarget)->getProjectile();
            d->setPosition(CPoint(m_oStartPos.x + m_oDeltaPos.x * time, m_oStartPos.y + m_oDeltaPos.y * time));
            d->setHeight(m_fFromHeight + m_fDeltaHeight * time + fHeightDelta);
        }
        break;
    }

    if (m_bFixRotation)
    {
        // 修正角度
        float fOffsetR = atan(fA * fX);
        m_pTarget->setRotation(CC_RADIANS_TO_DEGREES((m_oStartPos.x > m_oEndPos.x ? fOffsetR : -fOffsetR) - ccpToAngle(ccpSub(m_oEndPos, m_oStartPos))));
    }
    //m_pTarget->setRotation(CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(m_oEndPos, m_oStartPos))) + (m_oStartPos.x > m_oEndPos.x ? CC_RADIANS_TO_DEGREES(fOffsetR) : -CC_RADIANS_TO_DEGREES(fOffsetR)));
    //CCLOG("%.2f, %.2f", CC_RADIANS_TO_DEGREES(-ccpToAngle(ccpSub(m_oEndPos, m_oStartPos))), (m_oStartPos.x > m_oEndPos.x ? CC_RADIANS_TO_DEGREES(fOffsetR) : CC_RADIANS_TO_DEGREES(fOffsetR)));
}

CCObject* CCMoveToNode::copyWithZone(CCZone* pZone)
{
    CCZone* pNewZone = NULL;
    CCMoveToNode* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCMoveToNode*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCMoveToNode();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    switch (m_eToType)
    {
    case kNormal:
        pCopy->initWithNode(m_fDuration, m_pToNode, m_bFixRotation, m_fMaxHeightDelta);
        break;

    case kDraw:
        pCopy->initWithDraw(m_fDuration, m_pToDraw, m_bFixRotation, m_fMaxHeightDelta);
        break;

    case kProjectile:
        pCopy->initWithProjectile(m_fDuration, m_pToProjectile, m_bFixRotation, m_fMaxHeightDelta);
        break;
    }

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

// CCLink
CCLink::CCLink()
    : m_eFromType(kDraw)
    , m_eToType(kDraw)
    , m_eTargetType(kProjectile)
    , m_pFromNode(NULL)
    , m_pToNode(NULL)
    , m_bFireFrom(false)
{
}

CCLink::~CCLink()
{
    if (m_pFromNode != NULL)
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

    if (m_pToNode != NULL)
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

bool CCLink::initWithDrawToDraw(CCAnimation* pAnimation, int iNotifyFrameIndex, CCObject* pSelector, SEL_CallFuncND pCallback, CCallFuncData* pData, CUnitDrawForCC* pFromDraw, CUnitDrawForCC* pToDraw)
{
    if (CCNotifyAnimate::initWithAnimation(pAnimation, iNotifyFrameIndex, pSelector, pCallback, pData) == false)
    {
        return false;
    }

    assert(pFromDraw != NULL);
    m_eFromType = kDraw;
    pFromDraw->retain();
    m_pFromDraw = pFromDraw;

    assert(pToDraw != NULL);
    m_eToType = kDraw;
    pToDraw->retain();
    m_pToDraw = pToDraw;

    return true;
}

CCObject* CCLink::copyWithZone(CCZone* pZone)
{
    CCZone* pNewZone = NULL;
    CCLink* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCLink*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCLink();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    switch (m_eFromType)
    {
    case kDraw:
        switch (m_eToType)
        {
        case kDraw:
            pCopy->initWithDrawToDraw((CCAnimation*)getAnimation()->copy()->autorelease(), m_iNotifyFrameIndex, m_pSelector, m_pCallback, m_pData, m_pFromDraw, m_pToDraw);
            break;

        }
        break;

    }

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCLink::startWithTarget(CCNode *pTarget)
{
    CCNotifyAnimate::startWithTarget(pTarget);

    if (DCAST(pTarget, CCProjectileSprite*) != NULL)
    {
        m_eTargetType = kProjectile;
        
        CCProjectileSprite* sp = DCAST(m_pTarget, CCProjectileSprite*);
        CProjectileForCC* p = sp->getProjectile();
        if (m_eFromType == kDraw && m_pFromDraw->getUnit()->getId() == p->getSrcUnit())
        {
            m_bFireFrom = true;
        }
    } 
    else if (DCAST(pTarget, CCNode*) != NULL)
    {
        m_eTargetType = kNormal;
    }
    else
    {
        assert(false);
    }

    pTarget->setVisible(false);
}

void CCLink::update(float t)
{
    if (m_pTarget)
    {
        if (m_pTarget->isVisible() == false)
        {
            m_pTarget->setVisible(true);
        }
        fixTargetPosition(m_pTarget);
    }

    CCNotifyAnimate::update(t);
}

void CCLink::fixTargetPosition(CCNode* pTarget)
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
        ((CCProjectileSprite*)m_pTarget)->getProjectile()->setHeight((fFromHeight + fToHeight) / 2);
    }
    
    CCPoint oDelta = ccpSub(m_oToPoint, m_oFromPoint);
    float fR = CC_RADIANS_TO_DEGREES(-ccpToAngle(oDelta));
    
    float fScale = sqrt(oDelta.x * oDelta.x + oDelta.y * oDelta.y) / pTarget->getContentSize().width;
    pTarget->setPosition(ccp((m_oFromPoint.x + m_oToPoint.x) / 2, (m_oFromPoint.y + m_oToPoint.y) / 2));
    pTarget->setRotation(fR);
    pTarget->setScaleX(fScale);
}

// CCShake
CCShake::CCShake()
{
}

bool CCShake::init( float fDuration, int iTimes, float fRange )
{
    float fDur = m_fDuration / max(iTimes, 1) / 4;
    
    return CCSequence::initWithTwoActions(CCSequence::create(CCMoveBy::create(fDur, ccp(fRange, 0.0f)), CCMoveBy::create(fDur, ccp(0.0f, -fRange)), CCMoveBy::create(fDur, ccp(-fRange, 0.0f)), NULL), CCMoveBy::create(fDur, ccp(0.0f, fRange)));
}

void CCShake::startWithTarget(CCNode* pTarget)
{
    m_oLoc = pTarget->getPosition();
    CCSequence::startWithTarget(pTarget);
}

void CCShake::stop(void)
{
    if (m_pTarget)
    {
        m_pTarget->setPosition(m_oLoc);
    }
    CCSequence::stop();
}
