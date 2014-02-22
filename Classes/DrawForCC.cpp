#include "CommHeader.h"

#include "DrawForCC.h"
#include "GameControl.h"
#include "ActionForCC.h"
#include "Logic/Unit.h"
#include "Logic/MultiRefObject.h"



// CCUnitSprite
CCUnitSprite::CCUnitSprite()
    : m_pDraw(NULL)
{
}

CCUnitSprite::~CCUnitSprite()
{
}

bool CCUnitSprite::initWithDraw( CUnitDrawForCC* pDraw )
{
    assert(pDraw != NULL);

    setDraw(pDraw);

    CCSpriteFrame* sf = pDraw->getFrameInfo(CUnitDrawForCC::kFrmDefault)->pSf;

    return initWithSpriteFrame(sf);
}

void CCUnitSprite::onMoveToDone( CCNode*, void* pCallFuncData )
{
    if (pCallFuncData == NULL)
    {
        return;
    }

    CCallFuncData* pCd = (CCallFuncData*)pCallFuncData;
    (pCd->getSelector()->*pCd->getCallFunc())(pCd->getSelector(), pCd);
}

void CCUnitSprite::onNotifyFrame( CCNode*, void* pCallFuncData )
{
    if (pCallFuncData == NULL)
    {
        return;
    }

    CCallFuncData* pCd = (CCallFuncData*)pCallFuncData;
    (pCd->getSelector()->*pCd->getCallFunc())(pCd->getSelector(), pCd);
}

void CCUnitSprite::onAnimationDone( CCNode*, void* pCallFuncData )
{
    if (pCallFuncData == NULL)
    {
        return;
    }

    CCallFuncData* pCd = (CCallFuncData*)pCallFuncData;
    (pCd->getSelector()->*pCd->getCallFunc())(pCd->getSelector(), pCd);
}

// CUnitDrawForCC
CUnitDrawForCC::CUnitDrawForCC( const char* pName )
    : CUnitDraw2D(pName)
    , m_pSprite(NULL)
    , m_oAnchorPoint(0.5f, 0.5f)
{
    setDbgClassName("CUnitDrawForCC");
}

CUnitDrawForCC::~CUnitDrawForCC()
{
    if (m_pSprite != NULL)
    {
        m_pSprite->release();
    }
}

int CUnitDrawForCC::doMoveTo( const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone, float fSpeed /*= 1.0f*/ )
{
    CCActionInterval* act = CCMoveTo::create(fDuration, ccp(rPos.x, rPos.y));
    act = CCSequence::createWithTwoActions(act,
                                           CCCallFuncNMultiObj::create(getSprite(),
                                                                       callfuncND_selector(CCUnitSprite::onMoveToDone),
                                                                       pOnMoveToDone));

    CCSpeed* spd = CCSpeed::create(act, fSpeed);
    int tag = CKeyGen::nextKey();
    spd->setTag(tag);

    getSprite()->runAction(spd);

    return tag;
}

int CUnitDrawForCC::doAnimation( ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone, float fSpeed /*= 1.0f*/ )
{
    ANI_INFO* pAniInfo = getAnimationInfo(id);
    assert(pAniInfo != NULL);

    CCActionInterval* act = CCNotifyAnimate::create(pAniInfo->pAni,
                                                   pAniInfo->iNotifyFrameIndex,
                                                   getSprite(),
                                                   callfuncND_selector(CCUnitSprite::onNotifyFrame),
                                                   pOnNotifyFrame);

    

    if (iRepeatTimes == INFINITE)
    {
        act = CCRepeatForever::create(act);
    }
    else
    {
        act = CCRepeat::create(act, iRepeatTimes);
        act = CCSequence::createWithTwoActions(act,
                                               CCCallFuncNMultiObj::create(getSprite(),
                                                                           callfuncND_selector(CCUnitSprite::onAnimationDone),
                                                                           pOnAnimationDone));

    }

    CCSpeed* spd = CCSpeed::create(act, fSpeed);
    int tag = CKeyGen::nextKey();
    spd->setTag(tag);

    getSprite()->runAction(spd);

    return tag;
}

void CUnitDrawForCC::stopAction( int tag )
{
    getSprite()->stopActionByTag(tag);
}

void CUnitDrawForCC::setActionSpeed( int tag, float fSpeed )
{
    CCSpeed* spd = DCAST(getSprite()->getActionByTag(tag), CCSpeed*);
    if (spd == NULL)
    {
        return;
    }

    spd->setSpeed(fSpeed);
}

bool CUnitDrawForCC::isDoingAction(int id)
{
    return getSprite()->getActionByTag(id) != NULL;
}

void CUnitDrawForCC::stopAllActions()
{
    getSprite()->stopAllActions();
}

CCAction* CUnitDrawForCC::getAction( int id )
{
    return getSprite()->getActionByTag(id);
}

void CUnitDrawForCC::setFrame( FRM_ID id )
{
    CCSpriteFrame* sf = getFrameInfo(CUnitDrawForCC::kFrmDefault)->pSf;
    if (sf == NULL)
    {
        return;
    }

    getSprite()->setDisplayFrame(sf);
}

void CUnitDrawForCC::setFlipX( bool bFlipX )
{
    getSprite()->setFlipX(bFlipX);
}

bool CUnitDrawForCC::isFlipX() const
{
    return getSprite()->isFlipX();
}

void CUnitDrawForCC::prepareAnimation( ANI_ID id, const char* pName, int iNotifyFrameIndex )
{
    M_DEF_GC(gc);

    char szName[256];
    sprintf(szName, "Units/%s/%s", getName(), pName);
    CCAnimation* pAni = gc->getAnimation(szName);
    if (pAni == NULL)
    {
        return;
    }

    ANI_INFO stAi = {};
    stAi.iNotifyFrameIndex = iNotifyFrameIndex;
    stAi.pAni = pAni;
    m_mapAniInfos.insert(make_pair(id, stAi));
}

CUnitDrawForCC::ANI_INFO* CUnitDrawForCC::getAnimationInfo( ANI_ID id )
{
    auto it = m_mapAniInfos.find(id);
    assert(it != m_mapAniInfos.end());

    return &it->second;
}

void CUnitDrawForCC::prepareFrame( FRM_ID id, const char* pName )
{
    M_DEF_GC(gc);

    char szName[256];
    sprintf(szName, "Units/%s/%s", getName(), pName);
    CCSpriteFrame* pSf = gc->getFrame(szName);
    if (pSf == NULL)
    {
        return;
    }

    FRM_INFO stFi = {};
    stFi.pSf = pSf;
    m_mapFrmInfos.insert(make_pair(id, stFi));
}

CCSprite* CUnitDrawForCC::createSprite()
{
    assert(m_pSprite == NULL);
    m_pSprite = CCUnitSprite::createWithDraw(this);
    assert(m_pSprite != NULL);

    m_pSprite->retain();

    m_pSprite->setAnchorPoint(getAnchorPoint());

    return m_pSprite;
}

void CUnitDrawForCC::setPosition( const CPoint& p )
{
    getSprite()->setPosition(ccp(p.x, p.y));
}

CPoint& CUnitDrawForCC::getPosition()
{
    const CCPoint& p = getSprite()->getPosition();
    m_oPosition.x = p.x;
    m_oPosition.y = p.y;
    return m_oPosition;
}

CUnitDrawForCC::FRM_INFO* CUnitDrawForCC::getFrameInfo( FRM_ID id )
{
    auto it = m_mapFrmInfos.find(id);
    assert(it != m_mapFrmInfos.end());

    return &it->second;
}

void CUnitDrawForCC::setGeometry( float fHalfOfWidth, float fHalfOfHeight, const CCPoint& anchorPoint, const CCPoint& firePoint )
{
    setHalfOfWidth(fHalfOfWidth);
    setHalfOfHeight(fHalfOfHeight);
    setAnchorPoint(anchorPoint);
    setFirePoint(firePoint);
    if (getSprite() != NULL)
    {
        getSprite()->setAnchorPoint(getAnchorPoint());
    }
}

// CCUnitLayer
CCUnitLayer::CCUnitLayer()
    : m_pWorld(NULL)
    , m_fWorldInterval(0.0f)
{
}

bool CCUnitLayer::initWithWorld( CUnitWorldForCC* pWorld )
{
    assert(pWorld != NULL);

    setWorld(pWorld);

    return CCLayer::init();
}

void CCUnitLayer::onEnter()
{
    CCLayer::onEnter();
    schedule(schedule_selector(CCUnitLayer::onWorldInterval), m_fWorldInterval);
}

void CCUnitLayer::onExit()
{
    unschedule(schedule_selector(CCUnitLayer::onWorldInterval));
    CCLayer::onExit();
}

void CCUnitLayer::setWorldInterval( float fInterval )
{
    if (m_fWorldInterval == fInterval)
    {
        return;
    }
    m_fWorldInterval = fInterval;
    if (fInterval != 0)
    {
        schedule(schedule_selector(CCUnitLayer::onWorldInterval), m_fWorldInterval);
    }
    else
    {
        unschedule(schedule_selector(CCUnitLayer::onWorldInterval));
    }
}

void CCUnitLayer::onWorldInterval( float dt )
{
    CWorld* w = getWorld();
    if (w == NULL)
    {
        setWorldInterval(0);
        return;
    }

    w->step(dt);
}

// CUnitWorldForCC
CUnitWorldForCC::CUnitWorldForCC()
    : m_pLayer(NULL)
{
    setDbgClassName("CUnitWorldForCC");;
}

CUnitWorldForCC::~CUnitWorldForCC()
{
    if (m_pLayer != NULL)
    {
        m_pLayer->release();
    }
}

void CUnitWorldForCC::onAddUnit( CUnit* pUnit )
{
    CCLayer* pLayer = getLayer();
    if (pLayer == NULL)
    {
        pLayer = createLayer();
    }
    
    CUnitDrawForCC* pDraw = DCAST(pUnit->getDraw(), CUnitDrawForCC*);
    CCSprite* pSprite = pDraw->getSprite();
    if (pSprite == NULL)
    {
        pSprite = pDraw->createSprite();
    }

    pLayer->addChild(pSprite);
}

void CUnitWorldForCC::onDelUnit( CUnit* pUnit )
{
    CUnitDrawForCC* pDraw = DCAST(pUnit->getDraw(), CUnitDrawForCC*);

    getLayer()->removeChild(pDraw->getSprite());
}

void CUnitWorldForCC::setLayer( CCUnitLayer* pLayer )
{
    if (pLayer != m_pLayer)
    {
        if (m_pLayer != NULL)
        {
            m_pLayer->release();
        }
        else
        {
            pLayer->retain();
            pLayer->setWorld(this);
        }

        m_pLayer = pLayer;
    }
}

CCLayer* CUnitWorldForCC::createLayer()
{
    assert(m_pLayer == NULL);
    m_pLayer = CCUnitLayer::createWithWorld(this);
    assert(m_pLayer != NULL);

    m_pLayer->retain();

    return m_pLayer;
}

// CCWinUnitLayer
const float CCWinUnitLayer::CONST_MIN_MOVE_DELTA = 10.0f;
const float CCWinUnitLayer::CONST_MAX_CAN_MOVE_DURATION = 0.15f;

CCWinUnitLayer::CCWinUnitLayer()
    : m_iPendingAbilityOwner(0)
    , m_bIsTouching(false)
    , m_fMoveK(0.0f)
    , m_fBuffRange(0.0f)
    , m_fEdgeK(0.0f)
    , m_fMoveDelta(0.0f)
    , m_fTouchMovedDuration(0.0f)
    , m_fMoveR(0.0f)
    , m_bCanMove(false)
{
}

bool CCWinUnitLayer::init()
{
    setBufferEffectParam(0, 0, 0);
    m_bIsTouching = false;
    return CCUnitLayer::init();
}

void CCWinUnitLayer::setBackGroundSprite( CCSprite* pSprite )
{
    CCSize oSz = pSprite->getContentSize();
    oSz.width *= pSprite->getScaleX();
    oSz.height *= pSprite->getScaleY();
    CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    setContentSize(oSz);
    addChild(pSprite);
    pSprite->setPosition(getAnchorPointInPoints());
    setPosition(ccp((oWinSz.width - oSz.width) / 2, (oWinSz.height - oSz.height) / 2));

}

void CCWinUnitLayer::setBackGroundSprite( CCSprite* pSprite, int zOrder, int tag )
{
    CCSize oSz = pSprite->getContentSize();
    oSz.width *= pSprite->getScaleX();
    oSz.height *= pSprite->getScaleY();
    CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    setContentSize(oSz);
    addChild(pSprite, zOrder, tag);
    pSprite->setPosition(getAnchorPointInPoints());
    setPosition(ccp((oWinSz.width - oSz.width) / 2, (oWinSz.height - oSz.height) / 2));
}

void CCWinUnitLayer::onEnter()
{
    CCUnitLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    schedule(schedule_selector(CCWinUnitLayer::bufferWindowEffect), 1.0f / 200.0f);
}

void CCWinUnitLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    unschedule(schedule_selector(CCWinUnitLayer::bufferWindowEffect));
    CCUnitLayer::onExit();
}

bool CCWinUnitLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    m_bIsTouching = true;
    m_fTouchMovedDuration = 0;
    m_oMoveStart = pTouch->getLocation();
    m_fMoveDelta = 0;
    m_bCanMove = false;
    return true;
}

void CCWinUnitLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint oT = ccpAdd(getPosition(), pTouch->getDelta());
    adjustWinPos(oT);
    setPosition(oT);
}

void CCWinUnitLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    m_bIsTouching = false;
    m_fMoveDelta = ccpDistance(pTouch->getLocation(), m_oMoveStart);
    m_fMoveR = -ccpToAngle(ccpSub(pTouch->getLocation(), m_oMoveStart));
    (m_fTouchMovedDuration <= CONST_MAX_CAN_MOVE_DURATION && !isClickAction()) && (m_bCanMove = true);
}

void CCWinUnitLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
}

void CCWinUnitLayer::touchDelegateRetain()
{
    this->retain();
}

void CCWinUnitLayer::touchDelegateRelease()
{
    this->release();
}

void CCWinUnitLayer::bufferWindowEffect(float fDt)
{
    if (m_bIsTouching)
    {
        m_fTouchMovedDuration += fDt;
        return;
    }
    static CCSize oSzWin = CCDirector::sharedDirector()->getVisibleSize();
    CCSize oSz = getContentSize();
    CCPoint oT = getPosition();

    bool bOut = false;
    float fMax;
    if (oT.x > -m_fBuffRange * getScaleX())
    {
        oT.x += (-oT.x - m_fBuffRange) * m_fEdgeK;
        !bOut && (bOut = true);
    }
    else if (oT.x < (fMax = (oSzWin.width - oSz.width + m_fBuffRange) * getScaleX()))
    {
        oT.x += (-oT.x + fMax) * m_fEdgeK;
        !bOut && (bOut = true);
    }
    if (oT.y > -m_fBuffRange * getScaleY())
    {
        oT.y += (-oT.y - m_fBuffRange) * m_fEdgeK;
        !bOut && (bOut = true);
    }
    else if (oT.y < (fMax = (oSzWin.height - oSz.height + m_fBuffRange) * getScaleY()))
    {
        oT.y += (-oT.y + fMax) * m_fEdgeK;
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
    }
}

void CCWinUnitLayer::setBufferEffectParam( float fMoveK, float fBuffRange, float fEdgeK )
{
    m_fMoveK = MIN(1, MAX(0, fMoveK));

    CCSize oSz = getContentSize();
    CCSize oWinSz = CCDirector::sharedDirector()->getVisibleSize();
    oSz.width = MAX(0, oSz.width - oWinSz.width);
    oSz.height = MAX(0, oSz.height - oWinSz.height);
    m_fBuffRange = MIN(MIN(oSz.width, oSz.height) / 2, MAX(0, fBuffRange));

    m_fEdgeK = MIN(1, MAX(0, fEdgeK));
}

float CCWinUnitLayer::getTouchMovedDuration() const
{
    return m_fTouchMovedDuration;
}

float CCWinUnitLayer::getTouchMovedDistance() const
{
    return m_fMoveDelta;
}

float CCWinUnitLayer::getTouchMovedRadian() const
{
    return m_fMoveR;
}

bool CCWinUnitLayer::isSlideAction() const
{
    return m_bCanMove;
}

bool CCWinUnitLayer::isClickAction() const
{
    return m_fMoveDelta < CONST_MIN_MOVE_DELTA;
}

void CCWinUnitLayer::adjustWinPos( CCPoint& roPos )
{
    static CCSize oSzWin = CCDirector::sharedDirector()->getVisibleSize();
    CCSize oSz = getContentSize();
    roPos.x = MAX(roPos.x, (oSzWin.width - oSz.width) * getScaleX());
    roPos.y = MAX(roPos.y, (oSzWin.height - oSz.height) * getScaleY());
    roPos.x = MIN(roPos.x, 0);
    roPos.y = MIN(roPos.y, 0);
}

int CCWinUnitLayer::touchActionIndex() const
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
