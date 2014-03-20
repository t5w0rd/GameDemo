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
    CCAction* act = CCSpeed::create(CCSequence::createWithTwoActions(CCRepeat::create(CCAnimate::create(m_pAni), times), CCRemoveSelf::create()), speed);
    act->setTag(CONST_ACT_TAG);
    runAction(act);
}

void CCEffect::playForever( float speed /*= 1.0f*/ )
{
    CCAction* act = CCSpeed::create(CCRepeatForever::create(CCAnimate::create(m_pAni)), speed);
    act->setTag(CONST_ACT_TAG);
    runAction(act);
}
