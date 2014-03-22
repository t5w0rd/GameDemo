#include "CommHeader.h"

#include "ComponentForCC.h"


// CCProgressBar
const float CCProgressBar::CONST_MAX_PROCESS_BAR_PERCENT = 99.99999f;

CCProgressBar::CCProgressBar()
{
}

CCProgressBar::~CCProgressBar()
{
}

bool CCProgressBar::init(const CCSize& roSize, CCSprite* pFill, CCSprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop)
{
    setAnchorPoint(ccp(0.5f, 0.5f));
    setContentSize(roSize);

    CCSize oFillSz(roSize.width - fVertBorderWidth * 2, roSize.height - fHorizBorderWidth * 2);

    m_pPt = CCProgressTimer::create(pFill);
    if (m_pPt->getParent())
    {
        m_pPt->removeFromParentAndCleanup(true);
    }
    addChild(m_pPt, bFillOnTop);
    CCSize oSz = m_pPt->getContentSize();
    m_pPt->setPosition(getAnchorPointInPoints());
    m_pPt->setScaleX(oFillSz.width / oSz.width);
    m_pPt->setScaleY(oFillSz.height / oSz.height);
    m_pPt->setType(kCCProgressTimerTypeBar);
    m_pPt->setPercentage(0);
    m_pPt->setBarChangeRate(ccp(1, 0));
    m_pPt->setMidpoint(ccp(0, 0));

    if (pBorder)
    {
        addChild(pBorder, !bFillOnTop);
        oSz = pBorder->getContentSize();
        pBorder->setPosition(getAnchorPointInPoints());
        pBorder->setScaleX(roSize.width / oSz.width);
        pBorder->setScaleY(roSize.height / oSz.height);
    }

    return true;
}

void CCProgressBar::setPercentage(float fPercent)
{
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    //m_oHd.setPosition()
    m_pPt->setPercentage(fPercent);
}

void CCProgressBar::setPercentage(float fPercent, float fDuration, CCFiniteTimeAction* pEndAction /*= NULL*/)
{
    m_pPt->stopAllActions();
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    float fWidth = m_pPt->getScaleX() * m_pPt->getContentSize().width;
    if (pEndAction)
    {
        m_pPt->runAction(CCSequence::create(CCProgressTo::create(fDuration, fPercent), pEndAction, NULL));
    }
    else
    {
        m_pPt->runAction(CCSequence::create(CCProgressTo::create(fDuration, fPercent), NULL));
    }

}

CCFiniteTimeAction* CCProgressBar::setPercentageAction(float fPercent, float fDuration, CCFiniteTimeAction* pEndAction /*= NULL*/)
{
    m_pPt->stopAllActions();
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    float fWidth = m_pPt->getScaleX() * m_pPt->getContentSize().width;

    if (pEndAction)
    {
        return CCSequence::create(CCProgressTo::create(fDuration, fPercent), pEndAction, NULL);
    }
    return CCSequence::create(CCProgressTo::create(fDuration, fPercent), NULL);
}

void CCProgressBar::setFillColor(const ccColor3B& roColor)
{
    m_pPt->setColor(roColor);
}

// CCSpriteFrameCacheEx
CCDictionary* CCSpriteFrameCacheEx::getSpriteFrames()
{
    return m_pSpriteFrames;
}

CCDictionary* CCSpriteFrameCacheEx::getSpriteFramesAliases()
{
    return m_pSpriteFramesAliases;
}

SET_STR* CCSpriteFrameCacheEx::getLoadedFileNames()
{
    return m_pLoadedFileNames;
}

// CCGameFile
CCGameFile::CCGameFile(void)
{
}


CCGameFile::~CCGameFile(void)
{
}

bool CCGameFile::init( const char* pFileName, const char* pMode )
{
    M_DEF_FU(pFu);
    m_uPos = m_uSize = 0;
    m_pData = pFu->getFileData(pFu->fullPathForFilename(pFileName).c_str(), pMode, &m_uSize);
    if (!m_pData)
    {
        return false;
    }
    return true;
}

size_t CCGameFile::tell() const
{
    return m_uPos;
}

bool CCGameFile::eof() const
{
    return m_uPos >= m_uSize;
}

bool CCGameFile::seek( long lOffset, FILE_ORIGIN eOrigin )
{
    unsigned long uPos = 0;
    switch (eOrigin)
    {
        case kBegin:
            uPos = lOffset;
            break;

        case kCur:
            uPos = m_uPos + lOffset;
            break;

        case kEnd:
            uPos = m_uSize + lOffset;
            break;

        default:
            return false;
    }

    if (uPos < 0 || uPos > m_uSize)
    {
        return false;
    }

    m_uPos = uPos;
    return true;
}

// CCWinLayer
const float CCWinLayer::CONST_MIN_MOVE_DELTA = 10.0f;
const float CCWinLayer::CONST_MAX_CAN_MOVE_DURATION = 0.15f;

CCWinLayer::CCWinLayer()
    : m_iPendingAbilityOwner(0)
    , m_bIsTouching(false)
    , m_fMoveK(0.0f)
    , m_fBuffRange(0.0f)
    , m_fEdgeK(0.0f)
    , m_fMoveDelta(0.0f)
    , m_fTouchMovedDuration(0.0f)
    , m_fMoveR(0.0f)
    , m_bCanMove(false)
    , m_pBackground(NULL)
{
}

bool CCWinLayer::init()
{
    m_bIsTouching = false;
    return CCLayer::init();
}

void CCWinLayer::setBackGroundSprite(CCSprite* pSprite)
{
    CCSize oSz = pSprite->getContentSize();
    oSz.width *= pSprite->getScaleX();
    oSz.height *= pSprite->getScaleY();
    CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    setContentSize(oSz);
    addChild(pSprite);
    pSprite->setPosition(getAnchorPointInPoints());
    setPosition(ccp((oWinSz.width - oSz.width) / 2, (oWinSz.height - oSz.height) / 2));
    m_pBackground = pSprite;
}

void CCWinLayer::setBackGroundSprite(CCSprite* pSprite, int zOrder, int tag)
{
    CCSize oSz = pSprite->getContentSize();
    oSz.width *= pSprite->getScaleX();
    oSz.height *= pSprite->getScaleY();
    CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    setContentSize(oSz);
    addChild(pSprite, zOrder, tag);
    pSprite->setPosition(getAnchorPointInPoints());
    setPosition(ccp((oWinSz.width - oSz.width) / 2, (oWinSz.height - oSz.height) / 2));
    m_pBackground = pSprite;
}

void CCWinLayer::setBufferEffectParam(float fScale, float fMoveK, float fBuffRange, float fEdgeK)
{
    setScale(fScale);
    m_fMoveK = MIN(1, MAX(0, fMoveK));
    CCSize oSz = getContentSize() * getScale();
    static CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    oSz.width = MAX(0, oSz.width - oWinSz.width);
    oSz.height = MAX(0, oSz.height - oWinSz.height);
    m_fBuffRange = fBuffRange;

    m_fEdgeK = MIN(1, MAX(0, fEdgeK));
}

void CCWinLayer::setScale(float fScale)
{
    static CCSize sz = CCDirector::sharedDirector()->getVisibleSize();
    CCSize oSz = getContentSize();
    CCLayer::setScale(MIN(MAX(MAX(sz.width / oSz.width, sz.height / oSz.height), fScale), 4.0f));
}

float CCWinLayer::getTouchMovedDuration() const
{
    return m_fTouchMovedDuration;
}

float CCWinLayer::getTouchMovedDistance() const
{
    return m_fMoveDelta;
}

float CCWinLayer::getTouchMovedRadian() const
{
    return m_fMoveR;
}

bool CCWinLayer::isSlideAction() const
{
    return m_bCanMove;
}

void CCWinLayer::onEnter()
{
    setTouchEnabled(true);
    schedule(schedule_selector(CCWinLayer::bufferWindowEffect), 1.0f / 200.0f);
    CCLayer::onEnter();
}

void CCWinLayer::onExit()
{
    setTouchEnabled(false);
    unschedule(schedule_selector(CCWinLayer::bufferWindowEffect));
    CCLayer::onExit();
}

void CCWinLayer::bufferWindowEffect(float fDt)
{
    if (m_bIsTouching)
    {
        m_fTouchMovedDuration += fDt;
        return;
    }

    static CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    CCSize oSz = getContentSize() * getScale();
    CCPoint oT = getPosition();

    float fW = MAX(0, oSz.width - oWinSz.width);
    float fH = MAX(0, oSz.height - oWinSz.height);

    // layer用来计算位置的锚点为左下角，但缩放却是以中心进行缩放的，所以窗口的实际位置需要进行修正
    float fX = oT.x + (1 / getScale() - 1) *  0.5 * oSz.width;
    float fY = oT.y + (1 / getScale() - 1) *  0.5 * oSz.height;
    float fBuffRange = MIN(MIN(fW, fH) / 2, MAX(0, m_fBuffRange));

    bool bOut = false;
    float fMax;
    if (fX > -fBuffRange)
    {
        oT.x += MIN((-fX - fBuffRange) * m_fEdgeK, -0.1f);
        !bOut && (bOut = true);
    }
    else if (fX < (fMax = (oWinSz.width - oSz.width + fBuffRange)))
    {
        oT.x += MAX((-fX + fMax) * m_fEdgeK, 0.1f);
        !bOut && (bOut = true);
    }
    if (fY > -fBuffRange)
    {
        oT.y += MIN((-fY - fBuffRange) * m_fEdgeK, -0.1f);
        !bOut && (bOut = true);
    }
    else if (fY < (fMax = (oWinSz.height - oSz.height + fBuffRange)))
    {
        oT.y += MAX((-fY + fMax) * m_fEdgeK, 0.1f);
        !bOut && (bOut = true);
    }
    if (bOut)
    {
        setPosition(oT);
        m_fMoveDelta = 0;
    }
    else if (m_bCanMove)
    {
        m_fMoveDelta *= m_fMoveK;
        if (m_fMoveDelta >= CONST_MIN_MOVE_DELTA)
        {
            float fMove = m_fMoveDelta * fDt / m_fTouchMovedDuration;
            CCPoint oP = ccpAdd(oT, ccp(cos(m_fMoveR) * fMove, sin(-m_fMoveR) * fMove));
            adjustWinPos(oP);
            setPosition(oP);
        }
        else
        {
            m_bCanMove = false;
        }
    }
}

bool CCWinLayer::isClickAction() const
{
    return m_fMoveDelta < CONST_MIN_MOVE_DELTA;
}

void CCWinLayer::adjustWinPos(CCPoint& roPos)
{
    static CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    CCSize oSz = getContentSize() * getScale();
    float fX = (1 - 1 / getScale()) *  0.5 * oSz.width;
    float fY = (1 - 1 / getScale()) *  0.5 * oSz.height;
    roPos.x = MAX(roPos.x, (oWinSz.width - oSz.width) + fX);
    roPos.y = MAX(roPos.y, (oWinSz.height - oSz.height) + fY);
    roPos.x = MIN(roPos.x, fX);
    roPos.y = MIN(roPos.y, fY);
}

int CCWinLayer::touchActionIndex() const
{
    if (isSlideAction())
    {
        return kSlideWindow;
    }
    else if (isClickAction())
    {
        return m_iPendingAbilityOwner != 0 ? kUnitCastTarget : kClickPoint;
    }
    return m_iPendingAbilityOwner != 0 ? kUnitCastTarget : kNormalTouch;
}

void CCWinLayer::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
    CCSetIterator it = pTouches->begin();
    CCTouch* pTouch = ((CCTouch*)(*it));
    if (pTouches->count() == 1)
    {
        m_bIsTouching = true;
        m_fTouchMovedDuration = 0;
        m_oMoveStart = pTouch->getLocation();
        m_fMoveDelta = 0;
        m_bCanMove = false;

        m_fStartScale = getScale();
        m_fStartDis = ccpDistance(m_oMoveStart, m_oLast);
        m_oLast = m_oMoveStart;
    }
}

void CCWinLayer::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
    CCSetIterator it = pTouches->begin();
    CCTouch* pTouch = ((CCTouch*)(*it));
    if (pTouches->count() == 1)
    {
        CCPoint oT = ccpAdd(getPosition(), pTouch->getDelta());
        adjustWinPos(oT);
        setPosition(oT);
    }
    else if (pTouches->count() == 2)
    {
        CCPoint p1 = pTouch->getLocation();
        ++it;
        CCPoint p2 = ((CCTouch*)(*it))->getLocation();
        float fDis = ccpDistance(p2, p1);
        float fScale = fDis / MAX(m_fStartDis, 1.0f) * m_fStartScale;
        CCPoint oT = getPosition();
        setScale(fScale);
        adjustWinPos(oT);
        setPosition(oT);
    }
}

void CCWinLayer::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
    CCSetIterator it = pTouches->begin();
    CCTouch* pTouch = ((CCTouch*)(*it));
    if (pTouches->count() == 1)
    {
        m_bIsTouching = false;
        m_fMoveDelta = ccpDistance(pTouch->getLocation(), m_oMoveStart);
        m_fMoveR = -ccpToAngle(ccpSub(pTouch->getLocation(), m_oMoveStart));
        (m_fTouchMovedDuration <= CONST_MAX_CAN_MOVE_DURATION && !isClickAction()) && (m_bCanMove = true);
    }
}

void CCWinLayer::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{

}

// CCTouchSprite
CCTouchSprite::CCTouchSprite()
: m_state(kStateUngrabbed)
{
}

CCTouchSprite* CCTouchSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != NULL, msg);
#endif
    
    return createWithSpriteFrame(pFrame);
}

CCObject* CCTouchSprite::copyWithZone(CCZone *pZone)
{
    this->retain();
    return this;
}

void CCTouchSprite::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void CCTouchSprite::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}    

bool CCTouchSprite::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_state != kStateUngrabbed || !containsTouchLocation(touch))
    {
        return false;
    }
    
    //m_state = kStateGrabbed;
    
    return true;
}

void CCTouchSprite::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    // If it weren't for the TouchDispatcher, you would need to keep a reference
    // to the touch from touchBegan and check that the current touch is the same
    // as that one.
    // Actually, it would be even more complicated since in the Cocos dispatcher
    // you get CCSets instead of 1 UITouch, so you'd need to loop through the set
    // in each touchXXX method.
    
    CCAssert(m_state == kStateGrabbed, "CCTouchSprite - Unexpected state!");    
    
    CCPoint touchPoint = touch->getLocation();
    
    setPosition( ccp(touchPoint.x, getPosition().y) );
}

void CCTouchSprite::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCAssert(m_state == kStateGrabbed, "CCTouchSprite - Unexpected state!");    
    
    m_state = kStateUngrabbed;
} 

bool CCTouchSprite::containsTouchLocation(CCTouch* touch)
{
    CCSize s = getTexture()->getContentSize();
    CCRect rt(-s.width / 2, -s.height / 2, s.width, s.height);
    return rt.containsPoint(convertTouchToNodeSpaceAR(touch));
}

void CCTouchSprite::touchDelegateRetain()
{
    this->retain();
}

void CCTouchSprite::touchDelegateRelease()
{
    this->release();
}

// CCEffect
CCEffect::CCEffect()
    : m_pAni(NULL)
    , CONST_ACT_TAG(CKeyGen::nextKey())
{
}

CCEffect::~CCEffect()
{
    CC_SAFE_RELEASE(m_pAni);
}

bool CCEffect::initWithPath( const char* path, float delay )
{
    CCSpriteFrameCache* fc = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* sf = NULL;
    bool res = false;
    char sz[256];
    for (int i = 0; ; ++i)
    {
        sprintf(sz, "%s/%02d.png", path, i);
        sf = fc->spriteFrameByName(sz);
        if (sf == NULL)
        {
            if (i == 0)
            {
                return false;
            }
            break;
        }
        if (!m_pAni)
        {
            m_pAni = CCAnimation::create();
            res = initWithSpriteFrame(sf);
        }
        m_pAni->addSpriteFrame(sf);
    }

    m_pAni->setDelayPerUnit(delay);

    M_SAFE_RETAIN(m_pAni);

    return res;
}

void CCEffect::play( float speed /*= 1.0f*/, int times /*= 1.0*/ )
{
    CCAction* act = CCSpeed::create(CCRepeat::create(CCAnimate::create(m_pAni), times), speed);
    act->setTag(CONST_ACT_TAG);
    runAction(act);
}

void CCEffect::playRelease( float speed /*= 1.0f*/, int times /*= 1.0*/ )
{
    //CCAction* act = CCSpeed::create(CCSequence::createWithTwoActions(CCRepeat::create(CCAnimate::create(m_pAni), times), CCRemoveSelf::create(false)), speed);
    CCAction* act = CCSequence::createWithTwoActions(CCRepeat::create(CCAnimate::create(m_pAni), times), CCRemoveSelf::create());
    act->setTag(CONST_ACT_TAG);
    runAction(act);
}

void CCEffect::playForever( float speed /*= 1.0f*/ )
{
    CCAction* act = CCSpeed::create(CCRepeatForever::create(CCAnimate::create(m_pAni)), speed);
    act->setTag(CONST_ACT_TAG);
    runAction(act);
}
