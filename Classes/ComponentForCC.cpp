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
    : m_bMoveEnabled(true)
    , m_iPendingAbilityOwner(0)
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

void CCWinLayer::setPosition( const CCPoint& newPosition )
{
    static CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    CCSize oSz = getContentSize() * getScale();
    float fX = (1 - 1 / getScale()) *  0.5 * oSz.width;
    float fY = (1 - 1 / getScale()) *  0.5 * oSz.height;
    CCLayer::setPosition(ccp(min(max(newPosition.x, (oWinSz.width - oSz.width) + fX), fX), min(max(newPosition.y, (oWinSz.height - oSz.height) + fY), fY)));
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
        CCLayer::setPosition(oT);
        m_fMoveDelta = 0;
    }
    else if (m_bCanMove && m_bMoveEnabled)
    {
        m_fMoveDelta *= m_fMoveK;
        if (m_fMoveDelta >= CONST_MIN_MOVE_DELTA)
        {
            float fMove = m_fMoveDelta * fDt / m_fTouchMovedDuration;
            CCPoint oP = ccpAdd(oT, ccp(cos(m_fMoveR) * fMove, sin(-m_fMoveR) * fMove));
            //adjustWinPos(oP);
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
/*
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
*/
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
    if (m_bMoveEnabled == false)
    {
        return;
    }

    CCSetIterator it = pTouches->begin();
    CCTouch* pTouch = ((CCTouch*)(*it));
    if (pTouches->count() == 1)
    {
        CCPoint oT = ccpAdd(getPosition(), pTouch->getDelta());
        //adjustWinPos(oT);
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
        //adjustWinPos(oT);
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
        m_bMoveEnabled && (m_fTouchMovedDuration <= CONST_MAX_CAN_MOVE_DURATION && !isClickAction()) && (m_bCanMove = true);
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
    : CONST_ACT_TAG(CKeyGen::nextKey())
{
}

CCEffect::~CCEffect()
{
}

bool CCEffect::initWithPath( const char* path, float delay )
{
    CCSpriteFrameCache* fc = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* sf = NULL;
    bool res = false;
    char sz[256];
    CCAnimation* pAni = NULL;
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
        if (!pAni)
        {
            pAni = CCAnimation::create();
            res = initWithSpriteFrame(sf);
        }
        pAni->addSpriteFrame(sf);
    }

    pAni->setDelayPerUnit(delay);
    CCAnimationCache::sharedAnimationCache()->addAnimation(pAni, path);
    m_vecAnis.push_back(path);

    return res;
}

void CCEffect::play( int index /*= 0*/, float speed /*= 1.0f*/, int times /*= 1.0*/, CCObject* target /*= NULL*/, SEL_CallFuncN done /*= NULL*/ )
{
    if (index >= (int)m_vecAnis.size())
    {
        return;
    }
    CCAnimation* pAni = CCAnimationCache::sharedAnimationCache()->animationByName(m_vecAnis[index].c_str());

    CCActionInterval* actInner = CCRepeat::create(CCAnimate::create(pAni), times);
    if (target != NULL && done != NULL)
    {
        actInner = CCSequence::createWithTwoActions(actInner, CCCallFuncN::create(target, done));
    }

    CCAction* act = CCSpeed::create(actInner, speed);
    act->setTag(CONST_ACT_TAG);
    stop();
    runAction(act);
}

void CCEffect::playRelease( int index, float speed /*= 1.0f*/, int times /*= 1.0*/ )
{
    if (index >= (int)m_vecAnis.size())
    {
        return;
    }
    CCAnimation* pAni = CCAnimationCache::sharedAnimationCache()->animationByName(m_vecAnis[index].c_str());

    CCAction* act = CCSpeed::create(CCSequence::createWithTwoActions(CCRepeat::create(CCAnimate::create(pAni), times), CCRemoveSelf::create()), speed);
    act->setTag(CONST_ACT_TAG);
    stop();
    runAction(act);
}

void CCEffect::playForever( int index, float speed /*= 1.0f*/ )
{
    if (index >= (int)m_vecAnis.size())
    {
        return;
    }
    CCAnimation* pAni = CCAnimationCache::sharedAnimationCache()->animationByName(m_vecAnis[index].c_str());

    CCAction* act = CCSpeed::create(CCRepeatForever::create(CCAnimate::create(pAni)), speed);
    act->setTag(CONST_ACT_TAG);
    stop();
    runAction(act);
}

CCAnimation* CCEffect::addAnimation( const char* path, float delay )
{
    CCSpriteFrameCache* fc = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCSpriteFrame* sf = NULL;
    bool res = false;
    char sz[256];
    CCAnimation* pAni = NULL;
    for (int i = 0; ; ++i)
    {
        sprintf(sz, "%s/%02d.png", path, i);
        sf = fc->spriteFrameByName(sz);
        if (sf == NULL)
        {
            if (i == 0)
            {
                return NULL;
            }
            break;
        }
        if (!pAni)
        {
            pAni = CCAnimation::create();
        }
        pAni->addSpriteFrame(sf);
    }

    pAni->setDelayPerUnit(delay);
    CCAnimationCache::sharedAnimationCache()->addAnimation(pAni, path);
    m_vecAnis.push_back(path);

    return pAni;
}

void CCEffect::stop()
{
    stopActionByTag(CONST_ACT_TAG);
}

// CCSkillButtonBase
CCSkillButtonBase::CCSkillButtonBase()
    : m_pTarget(NULL)
    , m_iClickRetCode(0)
{
}

CCSkillButtonBase::~CCSkillButtonBase()
{

}

void CCSkillButtonBase::onCDBlickDone(CCNode* pNode)
{
    if (m_pBlink)
    {
        m_pBlink->setVisible(false);
    }
}

bool CCSkillButtonBase::init(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished)
{
    CCSprite* pNormalSprite = NULL;
    CCSprite* pSelectedSprite = NULL;
    CCSprite* pDisabledSprite = NULL;
    m_pDisableImageFrameName = pDisabledImage;
    m_pNormalImageFrameName = pNormalImage;
    m_pSelectImageFrameName = pSelectedImage;

    pNormalImage && (pNormalSprite = CCSprite::createWithSpriteFrameName(pNormalImage));
    pSelectedImage && (pSelectedSprite = CCSprite::createWithSpriteFrameName(pSelectedImage));
    pDisabledImage && (pDisabledSprite = CCSprite::createWithSpriteFrameName(pDisabledImage));
    if (!initWithNormalSprite(pNormalSprite, pSelectedSprite, pDisabledSprite, this, menu_selector(CCSkillButtonBase::onClick)))
    {
        return false;
    }
    m_pTarget = pTarget;
    m_pOnClick = pOnClick;
    m_pOnFinished = pOnFinished;
    setCoolDown(fCoolDown);
    CCSize sz = getContentSize();
    CCPoint oAp = getAnchorPointInPoints();

    // Create ProgressTimer
    m_pPt = CCProgressTimer::create((CCSprite*)getNormalImage());
    addChild(m_pPt);
    m_pPt->setPosition(oAp);
    m_pPt->setVisible(false);
    m_pPt->setType(kCCProgressTimerTypeRadial);

    // Create Blink Image
    if (pBlinkImage)
    {
        m_pBlink = CCSprite::createWithSpriteFrameName(pBlinkImage);
        addChild(m_pBlink, 10);
        m_pBlink->setPosition(oAp);
        m_pBlink->setVisible(false);
    }
    else
    {
        m_pBlink = NULL;
    }

    if (pMaskImage)
    {
        m_pMask = CCSprite::createWithSpriteFrameName(pMaskImage);
        addChild(m_pMask);
        m_pMask->setPosition(oAp);
        m_pMask->setVisible(false);
    }
    else
    {
        m_pMask = NULL;
    }

    // Create Label
    //m_pLabel = CCLabelTTF::create("5", "", 24, CCSizeMake(sz.width - 8, 32), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom); //CCLabelBMFont::create("88", "fonts/futura-48.fnt", sz.width - 6, kCCTextAlignmentRight);//, CCSizeMake(getContentSize().width - 6, 16.0), kCCTextAlignmentRight, "", 16.0
    //addChild(m_pLabel);
    //CCSize szLbl = m_pLabel->getContentSize();
    //m_pLabel->setPosition(ccp(sz.width - szLbl.width / 2 - 4, szLbl.height / 2));
    //m_pLabel->setColor(ccRED);

    return true;
}

void CCSkillButtonBase::onClick(CCObject* pObject)
{
    setClickRetCode(0);
    if (m_pTarget && m_pOnClick)
    {
        (m_pTarget->*m_pOnClick)(this);
    }
    if (getClickRetCode() < 0)
    {
        return;
    }

}

void CCSkillButtonBase::onCoolDownDone(CCNode* pNode)
{
    if (getCoolDown())
    {
        if (m_pMask)
        {
            m_pMask->setVisible(false);
        }
        
        m_pPt->setVisible(false);
        this->setEnabled(true);
        if (m_pBlink)
        {
            m_pBlink->setVisible(true);
            m_pBlink->runAction(CCSequence::create(CCFadeIn::create(0.25f), CCFadeOut::create(0.25f), CCCallFuncN::create(this, callfuncN_selector(CCSkillButtonBase::onCDBlickDone)), NULL));
        }
    }

    if (m_pTarget && m_pOnFinished)
    {
        (m_pTarget->*m_pOnFinished)(this);
    }
}

void CCSkillButtonBase::coolDown(float fFromPercent)
{
    if (getCoolDown())
    {
        float fCoolDownReal = getCoolDown() * (100.0 - fFromPercent) / 100.0;
        if (m_pMask)
        {
            m_pMask->setVisible(true);
        }
        
        m_pPt->setVisible(true);
        this->setEnabled(false);
        if (m_pDisabledImage)
        {
            DCAST(m_pDisabledImage, CCSprite*)->setOpacity(0x50 + (0xFF - 0x50) * fFromPercent / 100.0);
            m_pDisabledImage->runAction(CCFadeTo::create(fCoolDownReal, 0xFF));
        }
        

        m_pPt->setOpacity(0x7F + (0xFF - 0x7F) * fFromPercent / 100.0);
        CCProgressFromTo* pPro = CCProgressFromTo::create(fCoolDownReal, fFromPercent, 100.0f);

        //m_pPt->runAction();


        m_pPt->runAction(CCSpawn::createWithTwoActions(
            CCFadeTo::create(fCoolDownReal, 0xFF),
            CCSequence::create(pPro, CCCallFuncN::create(this, callfuncN_selector(CCSkillButtonBase::onCoolDownDone)), NULL)
            ));
    }
    else
    {
        onCoolDownDone(this);
    }
}

float CCSkillButtonBase::getPercentage() const
{
    return m_pPt->getPercentage();
}

// CCSkillButtonNormal
CCSkillButtonNormal* CCSkillButtonNormal::copyImg(CCObject* target, SEL_CallFuncN callOnClick, SEL_CallFuncN callOnFinished )
{
    CCSkillButtonNormal* pButtonRet = CCSkillButtonNormal::create(this->m_pNormalImageFrameName,this->m_pSelectImageFrameName,this->m_pDisableImageFrameName,NULL,NULL,this->getCoolDown(),target,callOnClick,callOnFinished);
    // 	pButtonRet->setNormalImage(this->getNormalImage());
    // 	pButtonRet->setSelectedImage(this->getSelectedImage());
    // 	pButtonRet->setDisabledImage(this->getDisabledImage());
    return pButtonRet;
}

// CCButtonPanel
CCButtonPanel::CCButtonPanel()
    : m_ppBtnPos(NULL)
    , m_pRetain(NULL)
{
}

CCButtonPanel::~CCButtonPanel()
{
    if (m_ppBtnPos)
    {
        delete[] m_ppBtnPos;
    }
    CC_SAFE_RELEASE(m_pRetain);
}

const float CCButtonPanel::CONST_ACTION_DURATION = 0.25;

bool CCButtonPanel::init( int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName )
{
    m_iRow = iRow;
    m_iLine = iLine;
    m_fButtonWidth = fButtonWidth;
    m_fBorderWidth = fBorderWidth;
    m_fInnerBorderWidth = fInnerBorderWidth;
    if (pBackgroundFrameName)
    {
        m_pBackground = CCSprite::createWithSpriteFrameName(pBackgroundFrameName);
        addChild(m_pBackground);
        m_pBackground->setPosition(getAnchorPointInPoints());
    }
    else
    {
        m_pBackground = NULL;
    }

    CCSize oSz = CCSizeMake(m_fBorderWidth * 2 + m_fInnerBorderWidth * (m_iLine - 1) + m_fButtonWidth * m_iLine, m_fBorderWidth * 2 + m_fInnerBorderWidth * (m_iRow - 1) + m_fButtonWidth * m_iRow);
    setContentSize(oSz);

    m_iOwnerKey = 0;

    m_pSkillMenu = CCMenu::create();
    addChild(m_pSkillMenu);
    m_pSkillMenu->setContentSize(getContentSize());
    m_pSkillMenu->setPosition(CCPointZero);

    if (m_ppBtnPos)
    {
        delete[] m_ppBtnPos;
    }
    CC_SAFE_RELEASE(m_pRetain);

    size_t uCount = iRow * iLine;
    m_ppBtnPos = new CCSkillButtonBase*[uCount];
    memset(m_ppBtnPos, 0, sizeof(CCSkillButtonBase*) * uCount);

    m_iCount = 0;

    return true;
}

void CCButtonPanel::addButton( CCSkillButtonBase* pButton, int iIndex )
{
    CCAssert(iIndex < m_iRow * m_iLine, "Break Bounds");
    CCAssert(m_iCount <= m_iRow * m_iLine, "already full");
    CCSkillButtonBase* pBtn = getButton(iIndex);
    if (pBtn)
    {
        delButton(iIndex);
    }

    m_pSkillMenu->addChild(pButton);
    pButton->setPosition(index2Point(iIndex));

    m_ppBtnPos[iIndex] = pButton;

    ++m_iCount;
}

void CCButtonPanel::addButton( CCSkillButtonBase* pButton, int iX, int iY )
{
    CCAssert(iY < m_iRow && iX < m_iLine, "Break Bounds");
    addButton(pButton, toIndex(iX, iY));
}

void CCButtonPanel::delButton( int iIndex )
{
    --m_iCount;
    m_pSkillMenu->removeChild(m_ppBtnPos[iIndex], true);
    m_ppBtnPos[iIndex] = NULL;
}
void CCButtonPanel::delButton( CCSkillButtonBase* pButton )
{
    int iIndex = getButtonIndex(pButton);
    CCAssert(iIndex >= 0, "button not found");
    delButton(iIndex);
}

void CCButtonPanel::delButton(int iX, int iY)
{
    CCAssert(iY < m_iRow && iX < m_iLine, "Break Bounds");
    delButton(toIndex(iX, iY));
}

int CCButtonPanel::allotSlot( ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iStartY = bY ? 0 : (m_iRow - 1);
    int iStartX = bX ? 0 : (m_iLine - 1);

    int iEndY = bY ? (m_iRow - 1) : 0;
    int iEndX = bX ? (m_iLine - 1) : 0;

    return allotSlot(iStartX, iStartY, iEndX, iEndY, eVer, eHor);
}

int CCButtonPanel::allotSlot( int iStartX, int iStartY, int iEndX, int iEndY, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor )
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iY = bY ? 1 : -1;
    int iX = bX ? 1 : -1;

    int iIndex;
    for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
    {
        int iStartX0 = (y == iStartY ? iStartX : (bX ? 0 : (m_iLine - 1)));
        int iEndX0 = (y == iEndY ? iEndX : (bX ? (m_iLine - 1) : 0));
        for (int x = iStartX0; bX ? (x <= iEndX0) : (x >= iEndX0); x += iX)
        {
            iIndex = toIndex(x, y);
            if (!getButton(iIndex))
            {
                return iIndex;
            }
        }
    }

    return -1;
}

void CCButtonPanel::clearUpSlot( ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iY = bY ? 1 : -1;
    int iX = bX ? 1 : -1;

    int iStartY = bY ? 0 : (m_iRow - 1);
    int iEndY = bY ? (m_iRow - 1) : 0;

    int iStartX = bX ? 0 : (m_iLine - 1);
    int iEndX = bX ? (m_iLine - 1) : 0;

    int iEmpty = -1;
    int iIndex;
    for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
    {
        for (int x = iStartX; bX ? (x <= iEndX) : (x >= iEndX); x += iX)
        {
            iIndex = toIndex(x, y);
            if (getButton(iIndex))
            {
                if (iEmpty >= 0)
                {
                    // 遇到按钮后且前面有空槽，移动按钮
                    moveButton(iIndex, iEmpty);
                    // 在前面找一块新空槽
                    iEmpty = allotSlot(index2X(iEmpty), index2Y(iEmpty), x, y, eVer, eHor);
                }
            }
            else
            {
                if (iEmpty < 0)
                {
                    // 首次遇到空槽
                    iEmpty = iIndex;
                }
            }
        }
    }
}

CCSkillButtonBase* CCButtonPanel::getButton(int iX, int iY) const
{
    return getButton(toIndex(iX, iY));
}

CCSkillButtonBase* CCButtonPanel::getButton( int iIndex ) const
{
    CCAssert(iIndex < m_iRow * m_iLine, "Break Bounds");
    return m_ppBtnPos[iIndex];
}

int CCButtonPanel::getButtonIndex(CCSkillButtonBase* pButton) const
{
    int n = m_iRow * m_iLine;
    for (int i = 0; i < n; ++i)
    {
        if (m_ppBtnPos[i] == pButton)
        {
            return i;
        }
    }

    return -1;
}

bool CCButtonPanel::isFull()
{
    return m_iCount == m_iRow * m_iLine;
}

int CCButtonPanel::index2Y( int iIndex ) const
{
    return iIndex / m_iLine;
}

int CCButtonPanel::index2X( int iIndex ) const
{
    return iIndex % m_iLine;
}

void CCButtonPanel::retainButton( CCSkillButtonBase* pButton )
{
    CC_SAFE_RETAIN(pButton);
    CC_SAFE_RELEASE(m_pRetain);
    m_pRetain = pButton;
}

CCSkillButtonBase* CCButtonPanel::getRetainButton() const
{
    return m_pRetain;
}

int CCButtonPanel::toIndex( int iX, int iY ) const
{
    return iY * m_iLine + iX;
}

void CCButtonPanel::addButtonEx( CCSkillButtonBase* pButton, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    addButton(pButton, allotSlot(eVer, eHor));
}

void CCButtonPanel::moveButton( int iIndexSrc, int iIndexDst )
{
    CCSkillButtonBase* pSrc = getButton(iIndexSrc);
    m_ppBtnPos[iIndexDst] = pSrc;
    m_ppBtnPos[iIndexSrc] = NULL;
    pSrc->setPosition(index2Point(iIndexDst));
}

CCPoint CCButtonPanel::index2Point( int iIndex )
{
    float fTmp = m_fButtonWidth + m_fInnerBorderWidth;
    float fTmp2 = m_fBorderWidth + m_fButtonWidth / 2;
    const CCSize& roSz = getContentSize();
    int iX = index2X(iIndex);
    int iY = index2Y(iIndex);
    return ccp(iX * fTmp - roSz.width / 2 + fTmp2, iY * fTmp - roSz.height / 2 + fTmp2);
}

void CCButtonPanel::pushAction( const ACTION_NODE& roAct )
{
    m_lstActs.push_back(roAct);
    if (m_lstActs.size() == 1)
    {
        onPrevActEnd(NULL);
    }
}

void CCButtonPanel::onPrevActEnd( CCNode* pNode )
{
    // 动作前半段，上一个动作即将结束，进行一些收尾工作
    CCAssert(!m_lstActs.empty(), "cannot be empty");

    if (pNode)
    {
        // 不是启动执行
        ACTION_NODE rNode = m_lstActs.front();
        switch (rNode.eAct)
        {
        case kAdd:
            break;

        case kDel:
            //m_pSkillMenu->removeChild(m_ppBtnPos[rNode.stDel.iIndex], true);
            delButton(rNode.stDel.pBtn);
            if (rNode.stDel.bClearUp)
            {
                // 队列前面插入clearUp动作节点
                m_lstActs.pop_front();
                m_lstActs.push_front(ACTION_NODE(rNode.stDel.eVer, rNode.stDel.eHor));
                m_lstActs.push_front(rNode);
            }

            break;

        case kMove:
            moveButton(getButtonIndex(rNode.stMove.pBtn), rNode.stMove.iIndexDst);
            break;

        case kAddEx:
            break;

        case kClearUp:
            break;

        default:
            ;
        }

        m_lstActs.pop_front();
    }

    if (m_lstActs.empty())
    {
        return;
    }

    // 动作后半段，上一个动作彻底执行结束，并且已弹出，新的动作开始运行
    ACTION_NODE rNode = m_lstActs.front();
    switch (rNode.eAct)
    {
    case kAdd:
        addButton(rNode.stAdd.pBtn, rNode.stAdd.iIndex);
        CC_SAFE_RELEASE(rNode.stAdd.pBtn);
        rNode.stAdd.pBtn->setOpacity(0);
        rNode.stAdd.pBtn->runAction(CCSequence::create(CCFadeIn::create(CONST_ACTION_DURATION), NULL));
        onPrevActEnd(rNode.stAdd.pBtn);
        break;

    case kDel:
        rNode.stDel.pBtn->stopAllActions();
        rNode.stDel.pBtn->runAction(CCSequence::create(CCFadeOut::create(CONST_ACTION_DURATION), CCCallFuncN::create(this, callfuncN_selector(CCButtonPanel::onPrevActEnd)), NULL));
        break;

    case kMove:
        rNode.stMove.pBtn->runAction(CCSequence::create(CCMoveTo::create(CONST_ACTION_DURATION, index2Point(rNode.stMove.iIndexDst)), CCCallFuncN::create(this, callfuncN_selector(CCButtonPanel::onPrevActEnd)), NULL));
        break;

    case kAddEx:
        addButton(rNode.stAddEx.pBtn, allotSlot(rNode.stAddEx.eVer, rNode.stAddEx.eHor));
        CC_SAFE_RELEASE(rNode.stAddEx.pBtn);
        rNode.stAddEx.pBtn->setOpacity(0);
        rNode.stAddEx.pBtn->runAction(CCSequence::create(CCFadeIn::create(CONST_ACTION_DURATION), NULL));
        onPrevActEnd(rNode.stAddEx.pBtn);
        break;

    case kClearUp:
        {
            bool bY = (rNode.stClearUp.eVer == kBottomToTop);
            bool bX = (rNode.stClearUp.eHor == kLeftToRight);

            int iY = bY ? 1 : -1;
            int iX = bX ? 1 : -1;

            int iStartY = bY ? 0 : (m_iRow - 1);
            int iEndY = bY ? (m_iRow - 1) : 0;

            int iStartX = bX ? 0 : (m_iLine - 1);
            int iEndX = bX ? (m_iLine - 1) : 0;

            int iMove = 0;
            m_lstActs.pop_front();
            CCSkillButtonBase* pBtn;
            int iEmpty = -1;
            int iIndex;
            for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
            {
                int iStartX0 = (y == iStartY ? iStartX : (bX ? 0 : (m_iLine - 1)));
                int iEndX0 = (y == iEndY ? iEndX : (bX ? (m_iLine - 1) : 0));
                for (int x = iStartX0; bX ? (x <= iEndX0) : (x >= iEndX0); x += iX)
                {
                    iIndex = toIndex(x, y);
                    pBtn = getButton(iIndex);
                    if (pBtn)
                    {
                        if (iEmpty >= 0)
                        {
                            // 遇到按钮后且前面有空槽，移动按钮
                            m_ppBtnPos[iEmpty] = pBtn;
                            m_ppBtnPos[iIndex] = NULL;

                            m_lstActs.push_front(ACTION_NODE()); // empty action
                            ++iMove;

                            pBtn->runAction(CCSequence::create(CCMoveTo::create(CONST_ACTION_DURATION, index2Point(iEmpty)), CCCallFuncN::create(this, callfuncN_selector(CCButtonPanel::onPrevActEnd)), NULL));
                            // 在前面找一块新空槽
                            iEmpty = allotSlot(index2X(iEmpty), index2Y(iEmpty), x, y, rNode.stClearUp.eVer, rNode.stClearUp.eHor);
                        }
                    }
                    else
                    {
                        if (iEmpty < 0)
                        {
                            // 首次遇到空槽
                            iEmpty = iIndex;
                        }
                    }
                }
            }

            if (!iMove && !m_lstActs.empty())
            {
                onPrevActEnd((CCNode*)(1));
            }
        }

        break;

    default:
        ;
    }
}

void CCButtonPanel::pushAddButtonAction( CCSkillButtonBase* pButton, int iIndex )
{
    CC_SAFE_RETAIN(pButton);
    pushAction(ACTION_NODE(pButton, iIndex));
}

void CCButtonPanel::pushDelButtonAction( int iIndex, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/, bool bClearUp /*= true*/ )
{
    ACTION_NODE stNode(iIndex, eVer, eHor, bClearUp);
    stNode.stDel.pBtn = getButton(iIndex);
    pushAction(stNode);
}

void CCButtonPanel::pushMoveButtonAction( int iIndexSrc, int iIndexDst )
{
    ACTION_NODE stNode(iIndexSrc, iIndexDst);
    stNode.stMove.pBtn = getButton(iIndexSrc);
    pushAction(stNode);
}

void CCButtonPanel::pushAddButtonExAction( CCSkillButtonBase* pButton, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    CC_SAFE_RETAIN(pButton);
    pushAction(ACTION_NODE(pButton, eVer, eHor));
}

void CCButtonPanel::pushClearUpSlotAction( ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    pushAction(ACTION_NODE(eVer, eHor));
}

int CCButtonPanel::getMaxCount() const
{
    return m_iRow * m_iLine;
}

int CCButtonPanel::getCount() const
{
    return m_iCount;
}
