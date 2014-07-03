#include "CommHeader.h"

#include "DrawForCC.h"
#include "GameControl.h"
#include "ActionForCC.h"
#include "Unit.h"
#include "MultiRefObject.h"
#include "AbilityForCC.h"
#include "ComponentForCC.h"


// ActionSprite
void ActionSprite::onMoveToDone(Node*, const FUNC_VOID& callFunc)
{
    if (!callFunc)
    {
        return;
    }

    callFunc();
}

void ActionSprite::onNotifyFrame(Node*, const FUNC_VOID& callFunc)
{
    if (!callFunc)
    {
        return;
    }

    callFunc();
}

void ActionSprite::onAnimationDone(Node*, const FUNC_VOID& callFunc)
{
    if (!callFunc)
    {
        return;
    }

    callFunc();
}

// CSpriteInfo
CSpriteInfo::CSpriteInfo(const char* pName)
: m_sName(pName)
{
    setDbgClassName("CSpriteInfo");
}

void CSpriteInfo::prepareAnimation(int id, const char* pName, int iNotifyFrameIndex, const char* pSound)
{
    M_DEF_GC(gc);

    char szName[256];
    sprintf(szName, "Sprites/%s/%s", getName(), pName);
    Animation* pAni = gc->getAnimation(szName);
    if (pAni == nullptr)
    {
        return;
    }

    ANI_INFO stAi = {};
    stAi.iNotifyFrameIndex = iNotifyFrameIndex;
    stAi.pAni = pAni;
    if (pSound != nullptr)
    {
        stAi.vecSounds.push_back(pSound);
        gc->preloadSound(pSound);
    }
    m_mapAniInfos.insert(make_pair(id, stAi));
}

const CSpriteInfo::ANI_INFO* CSpriteInfo::getAnimationInfo(int id) const
{
    auto it = m_mapAniInfos.find(id);
    assert(it != m_mapAniInfos.end());

    return &it->second;
}

void CSpriteInfo::addAnimationSound(int id, const char* sound)
{
    auto it = m_mapAniInfos.find(id);
    assert(it != m_mapAniInfos.end());
    it->second.vecSounds.push_back(sound);
}

void CSpriteInfo::prepareFrame(int id, const char* pName, const char* pFrameName)
{
    M_DEF_GC(gc);

    char szName[256];
    sprintf(szName, "Sprites/%s/%s", pFrameName != nullptr ? pFrameName : getName(), pName);
    SpriteFrame* pSf = gc->getFrame(szName);
    if (pSf == nullptr)
    {
        return;
    }

    FRM_INFO stFi = {};
    stFi.pSf = pSf;
    m_mapFrmInfos.insert(make_pair(id, stFi));
}

const CSpriteInfo::FRM_INFO* CSpriteInfo::getFrameInfo(int id) const
{
    auto it = m_mapFrmInfos.find(id);
    assert(it != m_mapFrmInfos.end());

    return &it->second;
}

// UnitSprite
UnitSprite::UnitSprite()
: m_pDraw(nullptr)
, m_pShadow(nullptr)
{
}

UnitSprite::~UnitSprite()
{
    CC_SAFE_RELEASE(m_pShadow);
}

bool UnitSprite::initWithDraw(CUnitDrawForCC* pDraw)
{
    assert(pDraw != nullptr);

    setDraw(pDraw);

    SpriteFrame* sf = pDraw->getSpriteInfo()->getFrameInfo(kFrmDefault)->pSf;

    return initWithSpriteFrame(sf);
}

Node* UnitSprite::createShadow()
{
    assert(m_pShadow == nullptr);
    m_pShadow = Node::create();
    m_pShadow->retain();

    m_pShadow->setAnchorPoint(getAnchorPoint());
    m_pShadow->setContentSize(getContentSize());
    m_pShadow->setPosition(getPosition());

    return m_pShadow;
}

void UnitSprite::setPosition(const Point& roPos)
{
    Sprite::setPosition(roPos);
    m_pShadow->setPosition(roPos);
    Node* layer = getParent();
    {
        if (layer != nullptr)
        {
            CUnitDraw2D* d = DCAST(getDraw(), CUnitDraw2D*);
            int z = M_BASE_Z + d->getHeight() - roPos.y;
            setLocalZOrder(z);
            getShadow()->setLocalZOrder(z + 1);
            //layer->reorderChild(this, z);
            //layer->reorderChild(getShadow(), z);
        }
    }
}

void UnitSprite::draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated)
{
    Sprite::draw(renderer, transform, transformUpdated);
    if (!Director::getInstance()->isDisplayStats())
    {
        return;
    }
#if 1
    const Point& p = getAnchorPoint();
    //oPos = pDr->convertToGL(oPos);
    //ccDrawLine(Point(0, oPos.y), Point(800, oPos.y));
    CUnitDrawForCC* d = getDraw();
    float w = d->getHalfOfWidth() * 2;
    float h = d->getHalfOfHeight() * 2;
    const Size& sz = getContentSize();
    DrawPrimitives::setDrawColor4B(255, 255, 255, 255);
    DrawPrimitives::drawRect(Point(0, 0), Point(sz.width, sz.height));
    DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
    DrawPrimitives::drawLine(Point((sz.width - w) * 0.5, p.y * sz.height), Point((sz.width + w) * 0.5, p.y * sz.height));  // Hor
    DrawPrimitives::drawLine(Point((isFlippedX() ? (1.0f - p.x) : p.x) * sz.width, p.y * sz.height), Point((isFlippedX() ? (1.0f - p.x) : p.x) * sz.width, p.y * sz.height + h));  // Ver
#endif
}

// CUnitDrawForCC
CUnitDrawForCC::CUnitDrawForCC(CSpriteInfo* si)
: m_si(si)
, m_pSprite(nullptr)
, m_oAnchorPoint(0.5f, 0.5f)
, m_iMaxTips(6)
, m_iBaseTipId(CIdGen::nextId() * m_iMaxTips)
, m_iCurTipId(m_iBaseTipId)
, m_lastCtrlSoundPlayedId(-1)
, m_lastCtrlSoundPlayedIndex(-1)
{
    M_SAFE_RETAIN(m_si);
    setDbgClassName("CUnitDrawForCC");
}

CUnitDrawForCC::~CUnitDrawForCC()
{
    M_SAFE_RELEASE(m_si);
    CC_SAFE_RELEASE(m_pSprite);
}

CUnitDrawForCC* CUnitDrawForCC::copy()
{
    CUnitDrawForCC* ret = new CUnitDrawForCC(m_si);
    ret->copyData(this);
    return ret;
}

void CUnitDrawForCC::copyData(CUnitDraw* from)
{
    CUnitDraw2D::copyData(from);
    CUnitDrawForCC* d = DCAST(from, CUnitDrawForCC*);
    m_oAnchorPoint = d->m_oAnchorPoint;
    m_vecCtrlSounds = d->m_vecCtrlSounds;
}

int CUnitDrawForCC::doMoveTo(const CPoint& rPos, float fDuration, const FUNC_VOID& onMoveToDone, float fSpeed /*= 1.0f*/)
{
    Point ccPos(rPos.x, rPos.y + getHeight());

    ActionInterval* act = MoveTo::create(fDuration, ccPos);
    act = SequenceEx::createWithTwoActions(
        act,
        CallFuncN::create(CC_CALLBACK_1(UnitSprite::onMoveToDone, getSprite(), onMoveToDone)));

    Speed* spd = Speed::create(act, fSpeed);
    int tag = CIdGen::nextId();
    spd->setTag(tag);

    getSprite()->runAction(spd);

    return tag;
}

void CUnitDrawForCC::updateMoveTo(const CPoint& rPos)
{
    Speed* spd = DCAST(getAction(getMoveToActionId()), Speed*);
    SequenceEx* seq = DCAST(spd->getInnerAction(), SequenceEx*);
    MoveTo* mt = DCAST(seq->getActionOne(), MoveTo*);
    
    return;
}

int CUnitDrawForCC::doAnimation(int id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone, float fSpeed /*= 1.0f*/)
{
    const CSpriteInfo::ANI_INFO* pAniInfo = m_si->getAnimationInfo(id);
    assert(pAniInfo != nullptr);

    ActionInterval* act = NotifyAnimate::create(
        pAniInfo->pAni,
        pAniInfo->iNotifyFrameIndex,
        CC_CALLBACK_1(UnitSprite::onNotifyFrame, getSprite(), onNotifyFrame),
        pAniInfo->vecSounds.empty() ? nullptr : pAniInfo->vecSounds[rand() % pAniInfo->vecSounds.size()].c_str());

    if (iRepeatTimes == INFINITE)
    {
        act = RepeatForever::create(act);
    }
    else
    {
        act = Repeat::create(act, iRepeatTimes);
        act = Sequence::createWithTwoActions(
            act,
            CallFuncN::create(CC_CALLBACK_1(UnitSprite::onAnimationDone, getSprite(), onAnimationDone)));
    }

    Speed* spd = Speed::create(act, fSpeed);
    int tag = CIdGen::nextId();
    spd->setTag(tag);

    getSprite()->runAction(spd);

    return tag;
}

void CUnitDrawForCC::stopAction(int tag)
{
    if (tag == 0)
    {
        return;
    }

    getSprite()->stopActionByTag(tag);
}

void CUnitDrawForCC::setActionSpeed(int tag, float fSpeed)
{
    if (tag == 0)
    {
        return;
    }

    Speed* spd = DCAST(getSprite()->getActionByTag(tag), Speed*);
    if (spd == nullptr)
    {
        return;
    }

    spd->setSpeed(fSpeed);
}

bool CUnitDrawForCC::isDoingAction(int id)
{
    return id != 0 && getSprite()->getActionByTag(id) != nullptr;
}

void CUnitDrawForCC::stopAllActions()
{
    getSprite()->stopAllActions();
}

Action* CUnitDrawForCC::getAction(int id)
{
    return getSprite()->getActionByTag(id);
}

void CUnitDrawForCC::setVisible(bool bVisible /*= true*/)
{
    getSprite()->setVisible(bVisible);
}

void CUnitDrawForCC::setFrame(int id)
{
    SpriteFrame* sf = m_si->getFrameInfo(kFrmDefault)->pSf;
    if (sf == nullptr)
    {
        return;
    }

    getSprite()->setSpriteFrame(sf);
}

SpriteFrame* CUnitDrawForCC::getFrameOfAnimation(int id, int index)
{
    const CSpriteInfo::ANI_INFO* pAniInfo = m_si->getAnimationInfo(id);
    assert(pAniInfo != nullptr);

    auto& frames = pAniInfo->pAni->getFrames();
    if (index >= frames.size())
    {
        return nullptr;
    }

    return frames.at(index)->getSpriteFrame();
}

void CUnitDrawForCC::setFrameByAnimation(int id, int index)
{
    auto sf = getFrameOfAnimation(id, index);
    if (sf == nullptr)
    {
        return;
    }

    getSprite()->setSpriteFrame(sf);
}

SpriteFrame* CUnitDrawForCC::getCurrentFrame() const
{
    return getSprite()->getSpriteFrame();
}

void CUnitDrawForCC::setFrame(SpriteFrame* frame)
{
    getSprite()->setSpriteFrame(frame);
}

void CUnitDrawForCC::setFlippedX(bool bFlipX)
{
    getSprite()->setFlippedX(bFlipX);
}

bool CUnitDrawForCC::isFlippedX() const
{
    return getSprite()->isFlippedX();
}

UnitSprite* CUnitDrawForCC::createSprite()
{
    assert(m_pSprite == nullptr);
    m_pSprite = UnitSprite::createWithDraw(this);
    assert(m_pSprite != nullptr);

    m_pSprite->retain();

    m_pSprite->setAnchorPoint(getAnchorPoint());

    m_pSprite->createShadow();

    return m_pSprite;
}

void CUnitDrawForCC::setPosition(const CPoint& p)
{
    UnitSprite* sp = DCAST(getSprite(), UnitSprite*);
    sp->setPosition(Point(p.x, p.y + getHeight()));
}

CPoint& CUnitDrawForCC::getPosition()
{
    const Point& p = getSprite()->getPosition();
    m_oPosition.x = p.x;
    m_oPosition.y = p.y - getHeight();
    return m_oPosition;
}

void CUnitDrawForCC::setHeight(float fHeight)
{
    const CPoint& p = getPosition();
    CUnitDraw2D::setHeight(fHeight);
    getSprite()->setPosition(Point(p.x, p.y + getHeight()));
}

void CUnitDrawForCC::setGeometry(float fHalfOfWidth, float fHalfOfHeight, const Point& anchorPoint, const CPoint& firePoint)
{
    setHalfOfWidth(fHalfOfWidth);
    setHalfOfHeight(fHalfOfHeight);
    setAnchorPoint(anchorPoint);
    setFirePoint(firePoint);
    if (getSprite() != nullptr)
    {
        getSprite()->setAnchorPoint(getAnchorPoint());
    }
}

void CUnitDrawForCC::addBattleTip(const char* pTip, const char* pFont, float fFontSize, Color3B color)
{
    //return;
    if (pFont == nullptr || pFont[0] == 0)
    {
        pFont = "fonts/Comic Book.fnt";
    }

    Node* l = getSprite()->getParent();
    const CPoint& up = getPosition();
    Point p(up.x, up.y + getHalfOfHeight() * 2 + 30.0f);
    int curTipId = getCurTipId();
    cirInc(curTipId, getBaseTipId(), getMaxTips());
    setCurTipId(curTipId);
    
    //Label* lbl = DCAST(l->getChildByTag(getCurTipId()), Label*);
    Label* lbl = DCAST(l->getChildByTag(getCurTipId()), Label*);
    if (lbl != nullptr)
    {
        lbl->removeFromParentAndCleanup(true);
    }

    //lbl = Label::create(pTip, pFont, fFontSize);
    char sz[1024];
    lbl = Label::createWithBMFont(pFont, gbk_to_utf8(pTip, sz));
    float scale = fFontSize / 32;
    lbl->setScale(scale);
    lbl->setColor(color);
    l->addChild(lbl, 1, getCurTipId());

    lbl->setPosition(p);

#if 0

    lbl->runAction(Sequence::create(

        Spawn::create(
        //MoveBy::create(0.1f, Point(0.0f, lbl->getContentSize().height)),
        FadeInOutScale4::create(
        0.5f, 1.2f, 0.8f,
        0.1f, 0.1f, 2.0f, 0.2f),
        nullptr),
        RemoveSelf::create(),
        nullptr));

#else

    lbl->runAction(Sequence::create(
        Spawn::create(
        //MoveBy::create(0.1f, Point(0.0f, lbl->getContentSize().height)),
            FadeInOutScale4::create(
                0.5f * scale, 2.0f * scale, scale, 0.8f * scale,
                0.1f, 0.1f, 0.3f, 0.2f),
            Sequence::create(DelayTime::create(0.5f), MoveBy::create(0.2f, Point(0.0f, lbl->getContentSize().height * 2)), nullptr),
            nullptr),
        RemoveSelf::create(),
        nullptr));
#endif

    for (int i = 0, j = getCurTipId(); i < getMaxTips(); ++i)
    {
        Node* pNode = l->getChildByTag(j);
        if (pNode != nullptr)
        {
            pNode->runAction(MoveBy::create(0.1f, Point(0.0f, lbl->getContentSize().height)));
        }

        cirDec(j, getBaseTipId(), getMaxTips());
    }
}

void CUnitDrawForCC::addCtrlSound(const char* sound, float duration)
{
    CTRL_SOUND cs;
    cs.sound = sound;
    cs.duration = duration;
    m_vecCtrlSounds.push_back(cs);
}

void CUnitDrawForCC::playRandomCtrlSound()
{
    if (m_vecCtrlSounds.empty())
    {
        return;
    }

    M_DEF_GC(gc);
    if (m_lastCtrlSoundPlayedId != -1 && gc->isSoundPlaying(m_lastCtrlSoundPlayedId))
    {
        return;
    }

    auto n = m_vecCtrlSounds.size();
    m_lastCtrlSoundPlayedIndex =
        (n == 1 || m_lastCtrlSoundPlayedIndex == -1) ?
        (rand() % n) :
        ((rand() % (n - 1) + m_lastCtrlSoundPlayedIndex + 1) % n);

    auto sd = m_vecCtrlSounds[m_lastCtrlSoundPlayedIndex];
    m_lastCtrlSoundPlayedId = gc->playSound(sd.sound.c_str(), sd.duration);
}

void CUnitDrawForCC::say(const char* words)
{
    M_DEF_GC(gc);
    gc->playSound("sounds/Effects/Sound_NotificationSecondLevel.mp3");

    auto nd = getSprite()->getShadow();

    nd->removeChildByTag(getId());
    auto sp = Scale9Sprite::create("UI/TalkBubble.png");
    sp->setTag(getId());
    nd->addChild(sp);
    
    sp->setInsetLeft(30);
    sp->setInsetRight(30);
    sp->setInsetTop(30);
    sp->setInsetBottom(30);

    char sz[1024];
    auto lbl = Label::createWithBMFont(
        "fonts/Comic Book.fnt",
        gbk_to_utf8(words, sz));
    sp->addChild(lbl, 2);
    lbl->setMaxLineWidth(400);
    lbl->setColor(Color3B::BLACK);
    
    sp->setContentSize(lbl->getContentSize() + Size(40, 20));
    Point lblp = Point(sp->getContentSize().width * 0.5 + 1, sp->getContentSize().height - lbl->getContentSize().height * 0.5 - 10);
    sp->setAnchorPoint(Point(0.5f, 0.0f));
    lbl->setPosition(lblp);

    auto sp2 = Sprite::create("UI/TalkBubble2.png");
    sp2->setAnchorPoint(Point(0.5f, 1.0f));
    sp->addChild(sp2, 1);
    sp2->setPosition(sp->getAnchorPointInPoints() + Point(-20.0f, 5.0f));
    
    sp->setPosition(nd->getAnchorPointInPoints() + Point(0, getHalfOfHeight() * 2.5 + 38));
    float sc = 0.8f;
    sp->setScale(sc);
    sp->runAction(Sequence::create(FadeInOutScale4::create(0, sc * 1.2, sc, sc * 0.5, 0.06f, 0.06f, strlen(words) / 10 + 2, 0.06f), RemoveSelf::create(), nullptr));
}

// UnitLayer
UnitLayer::UnitLayer()
: m_pWorld(nullptr)
, m_fWorldInterval(0.0f)
{
}

bool UnitLayer::initWithWorld(CWorldForCC* pWorld)
{
    assert(pWorld != nullptr);

    pWorld->setLayer(this);

    return WinLayer::init();
}

void UnitLayer::onEnter()
{
    WinLayer::onEnter();
    if (m_fWorldInterval != 0.0)
    {
        schedule(schedule_selector(UnitLayer::onWorldInterval), m_fWorldInterval);
    }
}

void UnitLayer::onExit()
{
    unschedule(schedule_selector(UnitLayer::onWorldInterval));
    WinLayer::onExit();
}

void UnitLayer::setWorldInterval(float fInterval)
{
    if (m_fWorldInterval == fInterval)
    {
        return;
    }
    m_fWorldInterval = fInterval;
    if (fInterval != 0)
    {
        schedule(schedule_selector(UnitLayer::onWorldInterval), m_fWorldInterval);
    }
    else
    {
        unschedule(schedule_selector(UnitLayer::onWorldInterval));
    }
}

void UnitLayer::onWorldInterval(float dt)
{
    CWorld* w = getWorld();
    if (w == nullptr)
    {
        setWorldInterval(0);
        return;
    }

    w->step(dt);
}

// CWorldForCC
CWorldForCC::CWorldForCC()
: m_pLayer(nullptr)
{
    setDbgClassName("CWorldForCC");;
}

CWorldForCC::~CWorldForCC()
{
    CC_SAFE_RELEASE(m_pLayer);
}

void CWorldForCC::onAddUnit(CUnit* pUnit)
{
    Layer* pLayer = getLayer();
    if (pLayer == nullptr)
    {
        pLayer = createLayer();
    }

    CUnitDrawForCC* pDraw = DCAST(pUnit->getDraw(), CUnitDrawForCC*);
    UnitSprite* pSprite = pDraw->getSprite();
    if (pSprite == nullptr)
    {
        pSprite = pDraw->createSprite();
        pUnit->addSystemAbility(new CShowStatusPas());
        if (!pUnit->isGhost())
        {
            onAddNormalAttributes(pUnit);
        }
        else
        {
            onDelNormalAttributes(pUnit);
        }
    }

    pLayer->addChild(pSprite);
    pLayer->addChild(pSprite->getShadow());
}

void CWorldForCC::onDelUnit(CUnit* pUnit)
{
    CUnitDrawForCC* pDraw = DCAST(pUnit->getDraw(), CUnitDrawForCC*);
    UnitSprite* pSprite = pDraw->getSprite();
    Layer* pLayer = getLayer();

    pLayer->removeChild(pSprite->getShadow(), true);
    pLayer->removeChild(pSprite, true);

}

void CWorldForCC::onAddProjectile(CProjectile* pProjectile)
{
    Layer* pLayer = getLayer();
    if (pLayer == nullptr)
    {
        pLayer = createLayer();
    }

    CProjectileForCC* pProjectileForCC = DCAST(pProjectile, CProjectileForCC*);
    ProjectileSprite* pSprite = pProjectileForCC->getSprite();
    if (pSprite == nullptr)
    {
        pSprite = pProjectileForCC->createSprite();
    }

    pLayer->addChild(pSprite);
}

void CWorldForCC::onDelProjectile(CProjectile* pProjectile)
{
    CProjectileForCC* pProjectileForCC = DCAST(pProjectile, CProjectileForCC*);
    ProjectileSprite* pSprite = pProjectileForCC->getSprite();
    Layer* pLayer = getLayer();

    pLayer->removeChild(pSprite, true);
}

void CWorldForCC::onAddNormalAttributes(CUnit* pUnit)
{
    //pUnit->addSystemAbility(new CShowStatusPas());
    pUnit->setAI(CBaseAI::instance());
    DCAST(pUnit->getDraw(), CUnitDrawForCC*)->getSprite()->getShadow()->setVisible(true);
}

void CWorldForCC::onDelNormalAttributes(CUnit *pUnit)
{
    //pUnit->delSystemAbility("ShowStatus");
    pUnit->setAI(nullptr);
    DCAST(pUnit->getDraw(), CUnitDrawForCC*)->getSprite()->getShadow()->setVisible(false);
}

void CWorldForCC::setLayer(UnitLayer* pLayer)
{
    if (pLayer == m_pLayer)
    {
        return;
    }

    if (pLayer != nullptr)
    {
        pLayer->retain();
        pLayer->setWorld(this);
    }
    M_SAFE_RELEASE(m_pLayer);
    m_pLayer = pLayer;
}

Layer* CWorldForCC::createLayer()
{
    assert(m_pLayer == nullptr);
    m_pLayer = UnitLayer::createWithWorld(this);
    assert(m_pLayer != nullptr);

    m_pLayer->retain();

    return m_pLayer;
}

void CWorldForCC::shutdown()
{
    m_pLayer->removeAllChildrenWithCleanup(true);
    CWorld::shutdown();
}

// CUnitPathForCC
CUnitPathForCC::CUnitPathForCC(const char* pFileName)
{
    addPoints(pFileName);
}

void CUnitPathForCC::addPoints(const char* pFileName)
{
    M_DEF_FU(pFu);
    GameFile* pFile = GameFile::create(pFileName, "rb");
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

void CUnitPathForCC::saveAsFile(const char* pFileName)
{
    string sFullName = FileUtils::getInstance()->fullPathForFilename(pFileName);
    FILE* pFile = fopen(sFullName.c_str(), "wb");
    fwrite("PTH", 1, 4, pFile);
    for (CUnitPath::VEC_POINTS::iterator it = m_vecPoints.begin(); it != m_vecPoints.end(); ++it)
    {
        fwrite(&it->x, sizeof(it->x), 1, pFile);
        fwrite(&it->y, sizeof(it->y), 1, pFile);
    }
    fclose(pFile);
}

// ProjectileSprite
ProjectileSprite::ProjectileSprite()
: m_pProjectile(nullptr)
{
}

bool ProjectileSprite::initWithProjectile(CProjectileForCC* pProjectile)
{
    assert(pProjectile != nullptr);

    setProjectile(pProjectile);

    SpriteFrame* sf = pProjectile->getSpriteInfo()->getFrameInfo(kFrmDefault)->pSf;

    return initWithSpriteFrame(sf);
}

void ProjectileSprite::setPosition(const Point& roPos)
{
    Sprite::setPosition(roPos);
    CProjectile* d = DCAST(getProjectile(), CProjectile*);
    int z = M_BASE_Z + d->getHeight() - roPos.y;
    setLocalZOrder(z);
}

// CProjectileForCC
CProjectileForCC::CProjectileForCC(CSpriteInfo* si)
: m_si(si)
, m_pSprite(nullptr)
, m_oAnchorPoint(0.5f, 0.5f)
{
    M_SAFE_RETAIN(m_si);
    setDbgClassName("CProjectileForCC");
}

CProjectileForCC::~CProjectileForCC()
{
    M_SAFE_RELEASE(m_si);
    CC_SAFE_RELEASE(m_pSprite);
}

CProjectileForCC* CProjectileForCC::copy()
{
    CProjectileForCC* ret = new CProjectileForCC(m_si);
    ret->copyData(this);

    return ret;
}

void CProjectileForCC::copyData(const CProjectile* from)
{
    CProjectile::copyData(from);
    const CProjectileForCC* p = DCAST(from, const CProjectileForCC*);
    // 如果需要复制位置、高度、AttackData等，应在这里添加代码
}

float CProjectileForCC::getRadius() const
{
    return (getSprite()->getContentSize().width + getSprite()->getContentSize().height) / 2;
}

void CProjectileForCC::setPosition(const CPoint& p)
{
    ProjectileSprite* sp = DCAST(getSprite(), ProjectileSprite*);
    sp->setPosition(Point(p.x, p.y + getHeight()));
}

CPoint& CProjectileForCC::getPosition()
{
    const Point& p = getSprite()->getPosition();
    m_oPosition.x = p.x;
    m_oPosition.y = p.y - getHeight();
    return m_oPosition;
}

void CProjectileForCC::setHeight(float fHeight)
{
    const CPoint& p = getPosition();
    CProjectile::setHeight(fHeight);
    getSprite()->setPosition(Point(p.x, p.y + getHeight()));
}

int CProjectileForCC::doLinkUnitToUnit(CUnit* pFromUnit, CUnit* pToUnit, ANI_ID id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone)
{
    const CSpriteInfo::ANI_INFO* pAniInfo = m_si->getAnimationInfo(id);
    assert(pAniInfo != nullptr);

    CUnitDrawForCC* d = DCAST(pFromUnit->getDraw(), CUnitDrawForCC*);
    assert(d != nullptr);

    CUnitDrawForCC* td = DCAST(pToUnit->getDraw(), CUnitDrawForCC*);
    assert(td != nullptr);

    ActionInterval* act = LinkAnimate::createWithDrawToDraw(
        pAniInfo->pAni,
        pAniInfo->iNotifyFrameIndex,
        CC_CALLBACK_1(ProjectileSprite::onNotifyFrame, getSprite(), onNotifyFrame),
        d,
        td);

    if (iRepeatTimes == INFINITE)
    {
        act = RepeatForever::create(act);
    }
    else
    {
        act = Repeat::create(act, iRepeatTimes);
        act = Sequence::createWithTwoActions(
            act,
            CallFuncN::create(CC_CALLBACK_1(ProjectileSprite::onAnimationDone, getSprite(), onAnimationDone)));
    }

    int tag = CIdGen::nextId();
    act->setTag(tag);

    getSprite()->runAction(act);

    return tag;
}

int CProjectileForCC::doMoveToUnit(CUnit* pToUnit, bool bFixRotation, float fMaxHeightDelta, float fDuration, const FUNC_VOID& onMoveToDone)
{
    CUnitDrawForCC* d = DCAST(pToUnit->getDraw(), CUnitDrawForCC*);
    assert(d != nullptr);

    ActionInterval* act = MoveToNode::createWithDraw(fDuration, d, true, fMaxHeightDelta);
    act = Sequence::createWithTwoActions(
        act,
        CallFuncN::create(CC_CALLBACK_1(ProjectileSprite::onMoveToDone, getSprite(), onMoveToDone)));

    int tag = CIdGen::nextId();
    act->setTag(tag);

    getSprite()->runAction(act);

    return tag;
}

int CProjectileForCC::doMoveTo(const CPoint& rPos, float fDuration, const FUNC_VOID& onMoveToDone)
{
    Point ccPos(rPos.x, rPos.y + getHeight());

    ActionInterval* act = MoveTo::create(fDuration, ccPos);
    act = Sequence::createWithTwoActions(
        act,
        CallFuncN::create(CC_CALLBACK_1(ProjectileSprite::onMoveToDone, getSprite(), onMoveToDone)));

    int tag = CIdGen::nextId();
    act->setTag(tag);

    getSprite()->setRotation(-CC_RADIANS_TO_DEGREES((rPos - getPosition()).getAngle()));
    getSprite()->runAction(act);

    return tag;
}

int CProjectileForCC::doAnimation(int id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone)
{
    const CSpriteInfo::ANI_INFO* pAniInfo = m_si->getAnimationInfo(id);
    assert(pAniInfo != nullptr);

    ActionInterval* act = NotifyAnimate::create(
        pAniInfo->pAni,
        pAniInfo->iNotifyFrameIndex,
        CC_CALLBACK_1(ProjectileSprite::onNotifyFrame, getSprite(), onNotifyFrame),
        pAniInfo->vecSounds.empty() ? nullptr : pAniInfo->vecSounds[rand() % pAniInfo->vecSounds.size()].c_str());

    if (iRepeatTimes == INFINITE)
    {
        act = RepeatForever::create(act);
    }
    else
    {
        act = Repeat::create(act, iRepeatTimes);
        act = Sequence::createWithTwoActions(
            act,
            CallFuncN::create(CC_CALLBACK_1(ProjectileSprite::onAnimationDone, getSprite(), onAnimationDone)));
    }

    int tag = CIdGen::nextId();
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
    return getSprite()->getActionByTag(id) != nullptr;
}

void CProjectileForCC::stopAllActions()
{
    getSprite()->stopAllActions();
}

void CProjectileForCC::setVisible(bool bVisible /*= true*/)
{
    getSprite()->setVisible(bVisible);
}

ProjectileSprite* CProjectileForCC::createSprite()
{
    assert(m_pSprite == nullptr);
    m_pSprite = ProjectileSprite::createWithProjectile(this);
    assert(m_pSprite != nullptr);

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
