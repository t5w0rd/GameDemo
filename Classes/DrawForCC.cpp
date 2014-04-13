#include "CommHeader.h"

#include "DrawForCC.h"
#include "GameControl.h"
#include "ActionForCC.h"
#include "Unit.h"
#include "MultiRefObject.h"
#include "AbilityForCC.h"
#include "ComponentForCC.h"


// CCActionSprite

void CCActionSprite::onMoveToDone(CCNode*, void* pCallFuncData)
{
    if (pCallFuncData == NULL)
    {
        return;
    }

    CCallFuncData* pCd = (CCallFuncData*)pCallFuncData;
    (pCd->getSelector()->*pCd->getCallFunc())(pCd->getSelector(), pCd);
}

void CCActionSprite::onNotifyFrame(CCNode*, void* pCallFuncData)
{
    if (pCallFuncData == NULL)
    {
        return;
    }

    CCallFuncData* pCd = (CCallFuncData*)pCallFuncData;
    (pCd->getSelector()->*pCd->getCallFunc())(pCd->getSelector(), pCd);
}

void CCActionSprite::onAnimationDone(CCNode*, void* pCallFuncData)
{
    if (pCallFuncData == NULL)
    {
        return;
    }

    CCallFuncData* pCd = (CCallFuncData*)pCallFuncData;
    (pCd->getSelector()->*pCd->getCallFunc())(pCd->getSelector(), pCd);
}

// CCUnitSprite
CCUnitSprite::CCUnitSprite()
    : m_pDraw(NULL)
    , m_pShadow(NULL)
{
}

CCUnitSprite::~CCUnitSprite()
{
    CC_SAFE_RELEASE(m_pShadow);
}

bool CCUnitSprite::initWithDraw(CUnitDrawForCC* pDraw)
{
    assert(pDraw != NULL);

    setDraw(pDraw);

    CCSpriteFrame* sf = pDraw->getFrameInfo(CUnitDraw::kFrmDefault)->pSf;

    return initWithSpriteFrame(sf);
}

CCNode* CCUnitSprite::createShadow()
{
    assert(m_pShadow == NULL);
    m_pShadow = CCNode::create();
    m_pShadow->retain();

    m_pShadow->setAnchorPoint(getAnchorPoint());
    m_pShadow->setContentSize(getContentSize());
    m_pShadow->setPosition(getPosition());

    return m_pShadow;
}

void CCUnitSprite::setPosition(const CCPoint& roPos)
{
    CCSprite::setPosition(roPos);
    m_pShadow->setPosition(roPos);
    CCNode* layer = getParent();
    {
        if (layer != NULL)
        {
            CUnitDraw2D* d = DCAST(getDraw(), CUnitDraw2D*);
            int z = M_BASE_Z + d->getHeight() - roPos.y;
            setZOrder(z);
            getShadow()->setZOrder(z);
            //layer->reorderChild(this, z);
            //layer->reorderChild(getShadow(), z);
        }
    }
}

void CCUnitSprite::draw()
{
    CCSprite::draw();
    if (!CCDirector::sharedDirector()->isDisplayStats())
    {
        return;
    }
#if 1
    const CCPoint& p = getAnchorPoint();
    //oPos = pDr->convertToGL(oPos);
    //ccDrawLine(ccp(0, oPos.y), ccp(800, oPos.y));
    CUnitDrawForCC* d = getDraw();
    float w = d->getHalfOfWidth() * 2;
    float h = d->getHalfOfHeight() * 2;
    const CCSize& sz = getContentSize();
    ccDrawColor4B(255, 255, 255, 255);
    ccDrawRect(ccp(0, 0), ccp(sz.width, sz.height));
    ccDrawColor4B(255, 0, 0, 255);
    ccDrawLine(ccp((sz.width - w) * 0.5, p.y * sz.height), ccp((sz.width + w) * 0.5, p.y * sz.height));  // Hor
    ccDrawLine(ccp((isFlipX() ? (1.0f - p.x) : p.x) * sz.width, p.y * sz.height), ccp((isFlipX() ? (1.0f - p.x) : p.x) * sz.width, p.y * sz.height + h));  // Ver
#endif
}

// CUnitDrawForCC
CUnitDrawForCC::CUnitDrawForCC(const char* pName)
    : CUnitDraw2D(pName)
    , m_pSprite(NULL)
    , m_oAnchorPoint(0.5f, 0.5f)
    , m_iMaxTips(6)
    , m_iBaseTipId(CKeyGen::nextKey() * m_iMaxTips)
    , m_iCurTipId(m_iBaseTipId)
{
    setDbgClassName("CUnitDrawForCC");
}

CUnitDrawForCC::~CUnitDrawForCC()
{
    CC_SAFE_RELEASE(m_pSprite);
}

CMultiRefObject* CUnitDrawForCC::copy()
{
    CUnitDrawForCC* ret = new CUnitDrawForCC(getName());
    ret->copyData(this);
    return ret;
}

void CUnitDrawForCC::copyData( CUnitDraw* from )
{
    CUnitDraw2D::copyData(from);
    CUnitDrawForCC* d = DCAST(from, CUnitDrawForCC*);
    m_mapAniInfos = d->m_mapAniInfos;
    m_mapFrmInfos = d->m_mapFrmInfos;
    m_oAnchorPoint = d->m_oAnchorPoint;
}

int CUnitDrawForCC::doMoveTo(const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone, float fSpeed /*= 1.0f*/)
{
    CCPoint ccPos(rPos.x, rPos.y + getHeight());

    CCActionInterval* act = CCMoveTo::create(fDuration, ccPos);
    act = CCSequenceEx::createWithTwoActions(
        act,
        CCCallFuncNMultiObj::create(
            getSprite(),
            callfuncND_selector(CCUnitSprite::onMoveToDone),
            pOnMoveToDone));

    CCSpeed* spd = CCSpeed::create(act, fSpeed);
    int tag = CKeyGen::nextKey();
    spd->setTag(tag);

    getSprite()->runAction(spd);

    return tag;
}

void CUnitDrawForCC::updateMoveTo( const CPoint& rPos )
{
    CCSpeed* spd = DCAST(getAction(getMoveToActionId()), CCSpeed*);
    CCSequenceEx* seq = DCAST(spd->getInnerAction(), CCSequenceEx*);
    CCMoveTo* mt = DCAST(seq->getActionOne(), CCMoveTo*);
    //mt->set
    return;
}

int CUnitDrawForCC::doAnimation(ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone, float fSpeed /*= 1.0f*/)
{
    ANI_INFO* pAniInfo = getAnimationInfo(id);
    assert(pAniInfo != NULL);

    CCActionInterval* act = CCNotifyAnimate::create(
        pAniInfo->pAni,
        pAniInfo->iNotifyFrameIndex,
        getSprite(),
        callfuncND_selector(CCUnitSprite::onNotifyFrame),
        pOnNotifyFrame,
        pAniInfo->vecSounds.empty() ? NULL : pAniInfo->vecSounds[rand() % pAniInfo->vecSounds.size()].c_str());

    if (iRepeatTimes == INFINITE)
    {
        act = CCRepeatForever::create(act);
    }
    else
    {
        act = CCRepeat::create(act, iRepeatTimes);
        act = CCSequence::createWithTwoActions(
            act,
            CCCallFuncNMultiObj::create(
            getSprite(),
            callfuncND_selector(CCUnitSprite::onAnimationDone),
            pOnAnimationDone));
    }

    CCSpeed* spd = CCSpeed::create(act, fSpeed);
    int tag = CKeyGen::nextKey();
    spd->setTag(tag);

    getSprite()->runAction(spd);

    return tag;
}

void CUnitDrawForCC::stopAction(int tag)
{
    getSprite()->stopActionByTag(tag);
}

void CUnitDrawForCC::setActionSpeed(int tag, float fSpeed)
{
    if (tag == 0)
    {
        return;
    }

    CCSpeed* spd = DCAST(getSprite()->getActionByTag(tag), CCSpeed*);
    if (spd == NULL)
    {
        return;
    }

    spd->setSpeed(fSpeed);
}

bool CUnitDrawForCC::isDoingAction(int id)
{
    return id != 0 && getSprite()->getActionByTag(id) != NULL;
}

void CUnitDrawForCC::stopAllActions()
{
    getSprite()->stopAllActions();
}

CCAction* CUnitDrawForCC::getAction(int id)
{
    return getSprite()->getActionByTag(id);
}

void CUnitDrawForCC::setVisible(bool bVisible /*= true*/)
{
    getSprite()->setVisible(bVisible);
}

void CUnitDrawForCC::setFrame(FRM_ID id)
{
    CCSpriteFrame* sf = getFrameInfo(CUnitDraw::kFrmDefault)->pSf;
    if (sf == NULL)
    {
        return;
    }

    getSprite()->setDisplayFrame(sf);
}

void CUnitDrawForCC::setFlipX(bool bFlipX)
{
    getSprite()->setFlipX(bFlipX);
}

bool CUnitDrawForCC::isFlipX() const
{
    return getSprite()->isFlipX();
}

void CUnitDrawForCC::prepareAnimation(ANI_ID id, const char* pName, int iNotifyFrameIndex, const char* pSound)
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
    if (pSound != NULL)
    {
        stAi.vecSounds.push_back(pSound);
        gc->preloadSound(pSound);
    }
    m_mapAniInfos.insert(make_pair(id, stAi));
}

CUnitDrawForCC::ANI_INFO* CUnitDrawForCC::getAnimationInfo(ANI_ID id)
{
    auto it = m_mapAniInfos.find(id);
    assert(it != m_mapAniInfos.end());
    
    return &it->second;
}

void CUnitDrawForCC::addAnimationSound( ANI_ID id, const char* sound )
{
    auto it = m_mapAniInfos.find(id);
    assert(it != m_mapAniInfos.end());
    it->second.vecSounds.push_back(sound);
}

void CUnitDrawForCC::prepareFrame(FRM_ID id, const char* pName)
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

CCUnitSprite* CUnitDrawForCC::createSprite()
{
    assert(m_pSprite == NULL);
    m_pSprite = CCUnitSprite::createWithDraw(this);
    assert(m_pSprite != NULL);

    m_pSprite->retain();

    m_pSprite->setAnchorPoint(getAnchorPoint());

    m_pSprite->createShadow();

    return m_pSprite;
}

void CUnitDrawForCC::setPosition(const CPoint& p)
{
    CCUnitSprite* sp = DCAST(getSprite(), CCUnitSprite*);
    sp->setPosition(ccp(p.x, p.y + getHeight()));
}

CPoint& CUnitDrawForCC::getPosition()
{
    const CCPoint& p = getSprite()->getPosition();
    m_oPosition.x = p.x;
    m_oPosition.y = p.y - getHeight();
    return m_oPosition;
}

void CUnitDrawForCC::setHeight(float fHeight)
{
    const CPoint& p = getPosition();
    CUnitDraw2D::setHeight(fHeight);
    getSprite()->setPosition(ccp(p.x, p.y + getHeight()));
}

CUnitDrawForCC::FRM_INFO* CUnitDrawForCC::getFrameInfo(FRM_ID id)
{
    auto it = m_mapFrmInfos.find(id);
    assert(it != m_mapFrmInfos.end());

    return &it->second;
}

void CUnitDrawForCC::setGeometry(float fHalfOfWidth, float fHalfOfHeight, const CCPoint& anchorPoint, const CPoint& firePoint)
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

void CUnitDrawForCC::addBattleTip(const char* pTip, const char* pFont, float fFontSize, ccColor3B color)
{
    //return;
    CCNode* l = getSprite()->getParent();
    const CPoint& up = getPosition();
    CCPoint p(up.x, up.y + getHalfOfHeight() * 2 + 30.0f);
    int curTipId = getCurTipId();
    cirInc(curTipId, getBaseTipId(), getMaxTips());
    setCurTipId(curTipId);
    
    CCLabelTTF* lbl = DCAST(l->getChildByTag(getCurTipId()), CCLabelTTF*);
    if (lbl != NULL)
    {
        lbl->removeFromParentAndCleanup(true);
    }

    lbl = CCLabelTTF::create(pTip, pFont, fFontSize);
    lbl->setColor(color);
    l->addChild(lbl, 1, getCurTipId());

    lbl->setPosition(p);

#if 0

    

    lbl->runAction(CCSequence::create(

        CCSpawn::create(
        //CCMoveBy::create(0.1f, ccp(0.0f, lbl->getContentSize().height)),
        CCFadeInOutScale4::create(
        0.5f, 1.2f, 0.8f,
        0.1f, 0.1f, 2.0f, 0.2f),
        NULL),
        CCRemoveSelf::create(),
        NULL));

#else

    lbl->runAction(CCSequence::create(
        CCSpawn::create(
        //CCMoveBy::create(0.1f, ccp(0.0f, lbl->getContentSize().height)),
            CCFadeInOutScale4::create(
                0.5f, 2.0f, 0.8f,
                0.1f, 0.1f, 0.3f, 0.2f),
            CCSequence::create(CCDelayTime::create(0.5f), CCMoveBy::create(0.2f, ccp(0.0f, lbl->getContentSize().height * 2)), NULL),
            NULL),
        CCRemoveSelf::create(),
        NULL));
#endif

    for (int i = 0, j = getCurTipId(); i < getMaxTips(); ++i)
    {
        CCNode* pNode = l->getChildByTag(j);
        if (pNode != NULL)
        {
            pNode->runAction(CCMoveBy::create(0.1f, ccp(0.0f, lbl->getContentSize().height)));
        }

        cirDec(j, getBaseTipId(), getMaxTips());
    }
}


// CCUnitLayer
CCUnitLayer::CCUnitLayer()
    : m_pWorld(NULL)
    , m_fWorldInterval(0.0f)
{
}

bool CCUnitLayer::initWithWorld(CWorldForCC* pWorld)
{
    assert(pWorld != NULL);

    setWorld(pWorld);

    return CCWinLayer::init();
}

void CCUnitLayer::onEnter()
{
    CCWinLayer::onEnter();
    schedule(schedule_selector(CCUnitLayer::onWorldInterval), m_fWorldInterval);
}

void CCUnitLayer::onExit()
{
    unschedule(schedule_selector(CCUnitLayer::onWorldInterval));
    CCWinLayer::onExit();
}

void CCUnitLayer::setWorldInterval(float fInterval)
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

void CCUnitLayer::onWorldInterval(float dt)
{
    CWorld* w = getWorld();
    if (w == NULL)
    {
        setWorldInterval(0);
        return;
    }

    w->step(dt);
}

// CWorldForCC
CWorldForCC::CWorldForCC()
    : m_pLayer(NULL)
{
    setDbgClassName("CWorldForCC");;
}

CWorldForCC::~CWorldForCC()
{
    CC_SAFE_RELEASE(m_pLayer);
}

void CWorldForCC::onAddUnit(CUnit* pUnit)
{
    CCLayer* pLayer = getLayer();
    if (pLayer == NULL)
    {
        pLayer = createLayer();
    }

    CUnitDrawForCC* pDraw = DCAST(pUnit->getDraw(), CUnitDrawForCC*);
    CCUnitSprite* pSprite = pDraw->getSprite();
    if (pSprite == NULL)
    {
        pSprite = pDraw->createSprite();
        pUnit->addSystemAbility(new CStatusShowPas);
        pUnit->setAI(CBaseAI::instance());
        //pDraw->updateMoveSpeedDelta();
    }

    pLayer->addChild(pSprite);
    pLayer->addChild(pSprite->getShadow());
}

void CWorldForCC::onDelUnit(CUnit* pUnit)
{
    CUnitDrawForCC* pDraw = DCAST(pUnit->getDraw(), CUnitDrawForCC*);
    CCUnitSprite* pSprite = pDraw->getSprite();
    CCLayer* pLayer = getLayer();

    pLayer->removeChild(pSprite->getShadow(), true);
    pLayer->removeChild(pSprite, true);

}

void CWorldForCC::onAddProjectile(CProjectile* pProjectile)
{
    CCLayer* pLayer = getLayer();
    if (pLayer == NULL)
    {
        pLayer = createLayer();
    }

    CProjectileForCC* pProjectileForCC = DCAST(pProjectile, CProjectileForCC*);
    CCProjectileSprite* pSprite = pProjectileForCC->getSprite();
    if (pSprite == NULL)
    {
        pSprite = pProjectileForCC->createSprite();
    }

    pLayer->addChild(pSprite);
}

void CWorldForCC::onDelProjectile(CProjectile* pProjectile)
{
    CProjectileForCC* pProjectileForCC = DCAST(pProjectile, CProjectileForCC*);
    CCProjectileSprite* pSprite = pProjectileForCC->getSprite();
    CCLayer* pLayer = getLayer();

    pLayer->removeChild(pSprite, true);
}

void CWorldForCC::setLayer(CCUnitLayer* pLayer)
{
    if (pLayer == m_pLayer)
    {
        return;
    }

    if (pLayer != NULL)
    {
        pLayer->retain();
        pLayer->setWorld(this);
    }
    M_SAFE_RELEASE(m_pLayer);
    m_pLayer = pLayer;
}

CCLayer* CWorldForCC::createLayer()
{
    assert(m_pLayer == NULL);
    m_pLayer = CCUnitLayer::createWithWorld(this);
    assert(m_pLayer != NULL);

    m_pLayer->retain();

    return m_pLayer;
}

void CWorldForCC::shutdown()
{
    m_pLayer->removeAllChildrenWithCleanup(true);
    CWorld::shutdown();
}

// CUnitPathForCC
CUnitPathForCC::CUnitPathForCC( const char* pFileName )
{
    addPoints(pFileName);
}

void CUnitPathForCC::addPoints( const char* pFileName )
{
    M_DEF_FU(pFu);
    CCGameFile* pFile = CCGameFile::create(pFileName, "rb");
    if (!pFile)
    {
        return;
    }
    uint32_t dwHdr = 0;
    pFile->read(&dwHdr);
    if (dwHdr != 'HTP')
    {
        return;
    }
    CPoint oP;
    for (;;)
    {
        if (pFile->read(&oP.x) != 1 || pFile->read(&oP.y) != 1)
        {
            break;
        }
        addPoint(oP);
    }
}

void CUnitPathForCC::saveAsFile( const char* pFileName )
{
    string sFullName = CCFileUtils::sharedFileUtils()->fullPathForFilename(pFileName);
    FILE* pFile = fopen(sFullName.c_str(), "wb");
    fwrite("PTH", 1, 4, pFile);
    for (CUnitPath::VEC_POINTS::iterator it = m_vecPoints.begin(); it != m_vecPoints.end(); ++it)
    {
        fwrite(&it->x, sizeof(it->x), 1, pFile);
        fwrite(&it->y, sizeof(it->y), 1, pFile);
    }
    fclose(pFile);
}

// CCProjectileSprite
CCProjectileSprite::CCProjectileSprite()
    : m_pProjectile(NULL)
{
}

bool CCProjectileSprite::initWithProjectile(CProjectileForCC* pProjectile)
{
    assert(pProjectile != NULL);

    setProjectile(pProjectile);

    CCSpriteFrame* sf = pProjectile->getFrameInfo(CProjectileForCC::kFrmDefault)->pSf;

    return initWithSpriteFrame(sf);
}

void CCProjectileSprite::setPosition(const CCPoint& roPos)
{
    CCSprite::setPosition(roPos);
    CProjectile* d = DCAST(getProjectile(), CProjectile*);
    int z = M_BASE_Z + d->getHeight() - roPos.y;
    setZOrder(z);
}

// CProjectileForCC
CProjectileForCC::CProjectileForCC(const char* pName)
    : CProjectile(pName)
    , m_pSprite(NULL)
    , m_oAnchorPoint(0.5f, 0.5f)
{
    memset(&m_stFrmInfo, 0, sizeof(m_stFrmInfo));
}

CProjectileForCC::~CProjectileForCC()
{
    CC_SAFE_RELEASE(m_pSprite);
}

CMultiRefObject* CProjectileForCC::copy()
{
    CProjectileForCC* ret = new CProjectileForCC(getName());
    ret->copyData(this);

    return ret;
}

void CProjectileForCC::copyData( const CProjectile* from )
{
    CProjectile::copyData(from);
    const CProjectileForCC* p = DCAST(from, const CProjectileForCC*);
    m_mapAniInfos = p->m_mapAniInfos;
    m_stFrmInfo = p->m_stFrmInfo;
    // 如果需要复制位置、高度、AttackData等，应在这里添加代码
}

float CProjectileForCC::getRadius() const
{
    return (getSprite()->getContentSize().width + getSprite()->getContentSize().height) / 2;
}

void CProjectileForCC::setPosition(const CPoint& p)
{
    CCProjectileSprite* sp = DCAST(getSprite(), CCProjectileSprite*);
    sp->setPosition(ccp(p.x, p.y + getHeight()));
}

CPoint& CProjectileForCC::getPosition()
{
    const CCPoint& p = getSprite()->getPosition();
    m_oPosition.x = p.x;
    m_oPosition.y = p.y - getHeight();
    return m_oPosition;
}

void CProjectileForCC::setHeight(float fHeight)
{
    const CPoint& p = getPosition();
    CProjectile::setHeight(fHeight);
    getSprite()->setPosition(ccp(p.x, p.y + getHeight()));
}

int CProjectileForCC::doLinkUnitToUnit(CUnit* pFromUnit, CUnit* pToUnit, ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone)
{
    ANI_INFO* pAniInfo = getAnimationInfo(id);
    assert(pAniInfo != NULL);

    CUnitDrawForCC* d = DCAST(pFromUnit->getDraw(), CUnitDrawForCC*);
    assert(d != NULL);

    CUnitDrawForCC* td = DCAST(pToUnit->getDraw(), CUnitDrawForCC*);
    assert(td != NULL);

    CCActionInterval* act = CCLink::createWithDrawToDraw(
        pAniInfo->pAni,
        pAniInfo->iNotifyFrameIndex,
        getSprite(),
        callfuncND_selector(CCProjectileSprite::onNotifyFrame),
        pOnNotifyFrame,
        d,
        td);

    if (iRepeatTimes == INFINITE)
    {
        act = CCRepeatForever::create(act);
    }
    else
    {
        act = CCRepeat::create(act, iRepeatTimes);
        act = CCSequence::createWithTwoActions(
            act,
            CCCallFuncNMultiObj::create(
                getSprite(),
                callfuncND_selector(CCProjectileSprite::onAnimationDone),
                pOnAnimationDone));
    }

    int tag = CKeyGen::nextKey();
    act->setTag(tag);

    getSprite()->runAction(act);

    return tag;
}

int CProjectileForCC::doMoveToUnit(CUnit* pToUnit, bool bFixRotation, float fMaxHeightDelta, float fDuration, CCallFuncData* pOnMoveToDone)
{
    CUnitDrawForCC* d = DCAST(pToUnit->getDraw(), CUnitDrawForCC*);
    assert(d != NULL);

    CCActionInterval* act = CCMoveToNode::createWithDraw(fDuration, d, true, fMaxHeightDelta);
    act = CCSequence::createWithTwoActions(
        act,
        CCCallFuncNMultiObj::create(
            getSprite(),
            callfuncND_selector(CCProjectileSprite::onMoveToDone),
            pOnMoveToDone));

    int tag = CKeyGen::nextKey();
    act->setTag(tag);

    getSprite()->runAction(act);

    return tag;
}

int CProjectileForCC::doMoveTo(const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone)
{
    CCPoint ccPos(rPos.x, rPos.y + getHeight());

    CCActionInterval* act = CCMoveTo::create(fDuration, ccPos);
    act = CCSequence::createWithTwoActions(
        act,
        CCCallFuncNMultiObj::create(
            getSprite(),
            callfuncND_selector(CCProjectileSprite::onMoveToDone),
            pOnMoveToDone));

    int tag = CKeyGen::nextKey();
    act->setTag(tag);

    getSprite()->runAction(act);

    return tag;
}

int CProjectileForCC::doAnimation(ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone)
{
    ANI_INFO* pAniInfo = getAnimationInfo(id);
    assert(pAniInfo != NULL);

    CCActionInterval* act = CCNotifyAnimate::create(
        pAniInfo->pAni,
        pAniInfo->iNotifyFrameIndex,
        getSprite(),
        callfuncND_selector(CCProjectileSprite::onNotifyFrame),
        pOnNotifyFrame);

    if (iRepeatTimes == INFINITE)
    {
        act = CCRepeatForever::create(act);
    }
    else
    {
        act = CCRepeat::create(act, iRepeatTimes);
        act = CCSequence::createWithTwoActions(
            act,
            CCCallFuncNMultiObj::create(
            getSprite(),
            callfuncND_selector(CCProjectileSprite::onAnimationDone),
            pOnAnimationDone));
    }

    int tag = CKeyGen::nextKey();
    act->setTag(tag);

    getSprite()->runAction(act);

    return tag;
}

void CProjectileForCC::stopAction(int tag)
{
    getSprite()->stopActionByTag(tag);
}

bool CProjectileForCC::isDoingAction(int id)
{
    return getSprite()->getActionByTag(id) != NULL;
}

void CProjectileForCC::stopAllActions()
{
    getSprite()->stopAllActions();
}

void CProjectileForCC::setVisible(bool bVisible /*= true*/)
{
    getSprite()->setVisible(bVisible);
}

void CProjectileForCC::prepareAnimation(ANI_ID id, const char* pName, int iNotifyFrameIndex)
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

CProjectileForCC::ANI_INFO* CProjectileForCC::getAnimationInfo(ANI_ID id)
{
    auto it = m_mapAniInfos.find(id);
    assert(it != m_mapAniInfos.end());

    return &it->second;
}

void CProjectileForCC::prepareFrame(FRM_ID id, const char* pName)
{
    M_DEF_GC(gc);

    char szName[256];
    sprintf(szName, "Units/%s/%s", getName(), pName);
    CCSpriteFrame* pSf = gc->getFrame(szName);
    if (pSf == NULL)
    {
        return;
    }

    m_stFrmInfo.pSf = pSf;
}

CProjectileForCC::FRM_INFO* CProjectileForCC::getFrameInfo(FRM_ID id)
{
    return &m_stFrmInfo;
}

CCProjectileSprite* CProjectileForCC::createSprite()
{
    assert(m_pSprite == NULL);
    char sz[256];
    sprintf(sz, "Units/%s/default.png", getName());
    m_pSprite = CCProjectileSprite::createWithProjectile(this);
    assert(m_pSprite != NULL);

    m_pSprite->retain();

    m_pSprite->setAnchorPoint(getAnchorPoint());

    return m_pSprite;
}

void CProjectileForCC::playFireSound()
{
    if (m_vecFireSounds.empty())
    {
        return;
    }
    M_DEF_GC(gc);
    gc->playSound(m_vecFireSounds[rand() % m_vecFireSounds.size()].c_str());
}

void CProjectileForCC::playEffectSound()
{
    if (m_vecEffectSounds.empty())
    {
        return;
    }
    M_DEF_GC(gc);
    gc->playSound(m_vecEffectSounds[rand() % m_vecEffectSounds.size()].c_str());
}
