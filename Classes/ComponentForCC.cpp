#include "CommHeader.h"

#include "ComponentForCC.h"


// ProgressBar
const float ProgressBar::CONST_MAX_PROCESS_BAR_PERCENT = 99.99999f;

ProgressBar::ProgressBar()
{
}

ProgressBar::~ProgressBar()
{
}

bool ProgressBar::init(const Size& roSize, Sprite* pFill, Sprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop)
{
    setAnchorPoint(Point(0.5f, 0.5f));
    setContentSize(roSize);

    Size oFillSz(roSize.width - fVertBorderWidth * 2, roSize.height - fHorizBorderWidth * 2);

    m_pPt = ProgressTimer::create(pFill);
    if (m_pPt->getParent())
    {
        m_pPt->removeFromParentAndCleanup(true);
    }
    addChild(m_pPt, bFillOnTop);
    Size oSz = m_pPt->getContentSize();
    m_pPt->setPosition(getAnchorPointInPoints());
    m_pPt->setScaleX(oFillSz.width / oSz.width);
    m_pPt->setScaleY(oFillSz.height / oSz.height);
    m_pPt->setType(ProgressTimer::Type::BAR);
    m_pPt->setPercentage(0);
    m_pPt->setBarChangeRate(Point(1, 0));
    m_pPt->setMidpoint(Point(0, 0));

    if (pBorder != nullptr)
    {
        addChild(pBorder, !bFillOnTop);
        oSz = pBorder->getContentSize();
        pBorder->setPosition(getAnchorPointInPoints());
        pBorder->setScaleX(roSize.width / oSz.width);
        pBorder->setScaleY(roSize.height / oSz.height);
    }

    setCascadeOpacityEnabled(true);

    return true;
}

void ProgressBar::setPercentage(float fPercent)
{
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    //m_oHd.setPosition()
    m_pPt->setPercentage(fPercent);
}

void ProgressBar::setPercentage(float fPercent, float fDuration, FiniteTimeAction* pEndAction /*= nullptr*/)
{
    m_pPt->stopAllActions();
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    float fWidth = m_pPt->getScaleX() * m_pPt->getContentSize().width;
    if (pEndAction)
    {
        m_pPt->runAction(Sequence::create(ProgressTo::create(fDuration, fPercent), pEndAction, nullptr));
    }
    else
    {
        m_pPt->runAction(Sequence::create(ProgressTo::create(fDuration, fPercent), nullptr));
    }

}

ActionInterval* ProgressBar::setPercentageAction(float fPercent, float fDuration, FiniteTimeAction* pEndAction /*= nullptr*/)
{
    m_pPt->stopAllActions();
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    float fWidth = m_pPt->getScaleX() * m_pPt->getContentSize().width;

    if (pEndAction)
    {
        return Sequence::create(ProgressTo::create(fDuration, fPercent), pEndAction, nullptr);
    }
    return Sequence::create(ProgressTo::create(fDuration, fPercent), nullptr);
}

void ProgressBar::setFillColor(const Color3B& roColor)
{
    m_pPt->setColor(roColor);
}

void ProgressBar::runActionForTimer(Action* pAction)
{
    m_pPt->runAction(pAction);
}

// SpriteFrameCacheEx
Map<std::string, SpriteFrame*>* SpriteFrameCacheEx::getSpriteFrames()
{
    return &_spriteFrames;
}

ValueMap* SpriteFrameCacheEx::getSpriteFramesAliases()
{
    return &_spriteFramesAliases;
}

std::set<std::string>* SpriteFrameCacheEx::getLoadedFileNames()
{
    return _loadedFileNames;
}

// GameFile
GameFile::GameFile(void)
: m_uPos(0)
{
}


GameFile::~GameFile(void)
{
}

bool GameFile::init(const char* pFileName, const char* pMode)
{
    M_DEF_FU(pFu);
    m_uPos = 0;

    m_data = pFu->getDataFromFile(pFu->fullPathForFilename(pFileName));
    if (m_data.isNull())
    {
        return false;
    }

    return true;
}

ssize_t GameFile::tell() const
{
    return m_uPos;
}

bool GameFile::eof() const
{
    return m_uPos >= m_data.getSize();
}

bool GameFile::seek(long lOffset, FILE_ORIGIN eOrigin)
{
    ssize_t uPos = 0;
    switch (eOrigin)
    {
        case kBegin:
            uPos = lOffset;
            break;

        case kCur:
            uPos = m_uPos + lOffset;
            break;

        case kEnd:
            uPos = m_data.getSize() + lOffset;
            break;

        default:
            return false;
    }

    if (uPos < 0 || uPos > m_data.getSize())
    {
        return false;
    }

    m_uPos = uPos;

    return true;
}

// WinLayer
const float WinLayer::CONST_MIN_MOVE_DELTA = 10.0f;
const float WinLayer::CONST_MAX_CAN_MOVE_DURATION = 0.15f;

WinLayer::WinLayer()
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
{
}

bool WinLayer::init()
{
    m_bIsTouching = false;
    return LayerColor::init();
}

bool WinLayer::initWithColor(const Color4B& color)
{
    m_bIsTouching = false;
    return LayerColor::initWithColor(color);
}

void WinLayer::setBackgroundSprite(Sprite* pSprite)
{
    Size oSz = pSprite->getContentSize();
    oSz.width *= pSprite->getScaleX();
    oSz.height *= pSprite->getScaleY();
    Size oWinSz = Director::getInstance()->getVisibleSize();
    setContentSize(oSz);
    addChild(pSprite, -1);
    pSprite->setPosition(getAnchorPointInPoints());
    setPosition(Point((oWinSz.width - oSz.width) / 2, (oWinSz.height - oSz.height) / 2));
}

void WinLayer::setBackgroundSprite(Sprite* pSprite, int zOrder, int tag)
{
    Size oSz = pSprite->getContentSize();
    oSz.width *= pSprite->getScaleX();
    oSz.height *= pSprite->getScaleY();
    Size oWinSz = Director::getInstance()->getVisibleSize();
    setContentSize(oSz);
    addChild(pSprite, zOrder, tag);
    pSprite->setPosition(getAnchorPointInPoints());
    setPosition(Point((oWinSz.width - oSz.width) / 2, (oWinSz.height - oSz.height) / 2));
}

void WinLayer::setBufferEffectParam(float fScale, float fMoveK, float fBuffRange, float fEdgeK)
{
    setScale(fScale);
    m_fMoveK = min(1.0f, max(0.0f, fMoveK));
    Size oSz = getContentSize() * getScale();
    static Size oWinSz = Director::getInstance()->getVisibleSize();
    oSz.width = max(0.0f, oSz.width - oWinSz.width);
    oSz.height = max(0.0f, oSz.height - oWinSz.height);
    m_fBuffRange = fBuffRange;

    m_fEdgeK = min(1.0f, max(0.0f, fEdgeK));
}

void WinLayer::setScale(float fScale)
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    Size oSz = getContentSize();
    Layer::setScale(min(max(max(wsz.width / oSz.width, wsz.height / oSz.height), fScale), 4.0f));
}

void WinLayer::setPosition(const Point& newPosition)
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    Size oSz = getContentSize() * getScale();
    float fX = (1 - 1 / getScale()) *  0.5 * oSz.width;
    float fY = (1 - 1 / getScale()) *  0.5 * oSz.height;
    Layer::setPosition(Point(min(max(newPosition.x, (wsz.width - oSz.width) + fX), fX), min(max(newPosition.y, (wsz.height - oSz.height) + fY), fY)));
}

float WinLayer::getTouchMovedDuration() const
{
    return m_fTouchMovedDuration;
}

float WinLayer::getTouchMovedDistance() const
{
    return m_fMoveDelta;
}

float WinLayer::getTouchMovedRadian() const
{
    return m_fMoveR;
}

bool WinLayer::isSlideAction() const
{
    return m_bCanMove;
}

void WinLayer::onEnter()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(WinLayer::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(WinLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(WinLayer::onTouchesEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;

    schedule(schedule_selector(WinLayer::bufferWindowEffect), 1.0f / 200.0f);
    LayerColor::onEnter();
}

void WinLayer::onExit()
{
    unschedule(schedule_selector(WinLayer::bufferWindowEffect));
    _eventDispatcher->removeEventListener(_touchListener);
    _touchListener = nullptr;

    LayerColor::onExit();
}

void WinLayer::bufferWindowEffect(float fDt)
{
    if (m_bIsTouching)
    {
        m_fTouchMovedDuration += fDt;
        return;
    }

    static Size oWinSz = Director::getInstance()->getVisibleSize();
    Size oSz = getContentSize() * getScale();
    Point oT = getPosition();

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
        LayerColor::setPosition(oT);
        m_fMoveDelta = 0;
    }
    else if (m_bCanMove && m_bMoveEnabled)
    {
        m_fMoveDelta *= m_fMoveK;
        if (m_fMoveDelta >= CONST_MIN_MOVE_DELTA)
        {
            float fMove = m_fMoveDelta * fDt / m_fTouchMovedDuration;
            Point oP = oT + Point(cos(m_fMoveR) * fMove, sin(-m_fMoveR) * fMove);
            //adjustWinPos(oP);
            setPosition(oP);
        }
        else
        {
            m_bCanMove = false;
        }
    }
}

bool WinLayer::isClickAction() const
{
    return m_fMoveDelta < CONST_MIN_MOVE_DELTA;
}
/*
void WinLayer::adjustWinPos(Point& roPos)
{
    static Size oWinSz = Director::getInstance()->getVisibleSize();
    Size oSz = getContentSize() * getScale();
    float fX = (1 - 1 / getScale()) *  0.5 * oSz.width;
    float fY = (1 - 1 / getScale()) *  0.5 * oSz.height;
    roPos.x = MAX(roPos.x, (oWinSz.width - oSz.width) + fX);
    roPos.y = MAX(roPos.y, (oWinSz.height - oSz.height) + fY);
    roPos.x = MIN(roPos.x, fX);
    roPos.y = MIN(roPos.y, fY);
}
*/
int WinLayer::touchActionIndex() const
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

void WinLayer::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event* event)
{
    auto it = touches.begin();
    Touch* pTouch = ((Touch*)(*it));
    if (touches.size() == 1)
    {
        m_bIsTouching = true;
        m_fTouchMovedDuration = 0;
        m_oMoveStart = pTouch->getLocation();
        m_fMoveDelta = 0;
        m_bCanMove = false;

        m_fStartScale = getScale();
        m_fStartDis = m_oMoveStart.getDistance(m_oLast);
        m_oLast = m_oMoveStart;
    }
}

void WinLayer::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event* event)
{
    if (m_bMoveEnabled == false)
    {
        return;
    }

    auto it = touches.begin();
    Touch* pTouch = ((Touch*)(*it));
    if (touches.size() == 1)
    {
        Point oT = getPosition() + pTouch->getDelta();
        //adjustWinPos(oT);
        setPosition(oT);
    }
    else if (touches.size() == 2)
    {
        Point p1 = pTouch->getLocation();
        ++it;
        Point p2 = ((Touch*)(*it))->getLocation();
        float fDis = p2.getDistance(p1);
        float fScale = fDis / MAX(m_fStartDis, 1.0f) * m_fStartScale;
        Point oT = getPosition();
        setScale(fScale);
        //adjustWinPos(oT);
        setPosition(oT);
    }
}

void WinLayer::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event* event)
{
    auto it = touches.begin();
    Touch* pTouch = ((Touch*)(*it));
    if (touches.size() == 1)
    {
        m_bIsTouching = false;
        m_fMoveDelta = pTouch->getLocation().getDistance(m_oMoveStart);
        m_fMoveR = -(pTouch->getLocation() - m_oMoveStart).getAngle();
        m_bMoveEnabled && (m_fTouchMovedDuration <= CONST_MAX_CAN_MOVE_DURATION && !isClickAction()) && (m_bCanMove = true);
    }
}

void WinLayer::onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event* event)
{
}

// TouchSprite
TouchSprite::TouchSprite()
: m_state(kStateUngrabbed)
{
}

TouchSprite* TouchSprite::createWithSpriteFrameName(const char* pszSpriteFrameName)
{
    SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != nullptr, msg);
#endif
    
    return createWithSpriteFrame(pFrame);
}

TouchSprite* TouchSprite::clone() const
{
    TouchSprite* ret = TouchSprite::createWithTexture(_texture);
    ret->m_state = m_state;
    ret->setPosition(getPosition());
    ret->setAnchorPoint(getAnchorPoint());

    return ret;
}

void TouchSprite::onEnter()
{
    Sprite::onEnter();

    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(TouchSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(TouchSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(TouchSprite::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool TouchSprite::onTouchBegan(Touch* touch, Event* event)
{
    if (m_state != kStateUngrabbed || !containsTouchLocation(touch))
    {
        return false;
    }
    
    //m_state = kStateGrabbed;
    
    return true;
}

void TouchSprite::onTouchMoved(Touch* touch, Event* event)
{
    // If it weren't for the TouchDispatcher, you would need to keep a reference
    // to the touch from touchBegan and check that the current touch is the same
    // as that one.
    // Actually, it would be even more complicated since in the Cocos dispatcher
    // you get CCSets instead of 1 UITouch, so you'd need to loop through the set
    // in each touchXXX method.
    
    CCAssert(m_state == kStateGrabbed, "TouchSprite - Unexpected state!");    
    
    Point touchPoint = touch->getLocation();
    
    setPosition(Point(touchPoint.x, getPosition().y));
}

void TouchSprite::onTouchEnded(Touch* touch, Event* event)
{
    CCAssert(m_state == kStateGrabbed, "TouchSprite - Unexpected state!");    
    
    m_state = kStateUngrabbed;
} 

bool TouchSprite::containsTouchLocation(Touch* touch)
{
    Size s = getTexture()->getContentSize();
    Rect rt(-s.width / 2, -s.height / 2, s.width, s.height);
    return rt.containsPoint(convertTouchToNodeSpaceAR(touch));
}

// TouchMaskLayer
bool TouchMaskLayer::initWithColor(const Color4B& color, GLubyte disOpacity)
{
    if (LayerColor::initWithColor(color) == false)
    {
        return false;
    }

    m_disOpacity = disOpacity;

    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch*, Event*){ return true; };  //CC_CALLBACK_2(TouchMaskLayer::onTouchBegan, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;

    setMaskEnabled(false);

    return true;
}

void TouchMaskLayer::setMaskEnabled(bool enabled)
{
    setOpacity(enabled ? m_disOpacity : 0);
    DCAST(_touchListener, EventListenerTouchOneByOne*)->setSwallowTouches(enabled);
}

// Effect
Effect::Effect()
: CONST_ACT_TAG(CKeyGen::nextKey())
{
}

Effect::~Effect()
{
}

bool Effect::initWithPath(const char* path, float delay)
{
    SpriteFrameCache* fc = SpriteFrameCache::getInstance();
    SpriteFrame* sf = nullptr;
    bool res = false;
    char sz[256];
    Animation* pAni = nullptr;
    for (int i = 0; ; ++i)
    {
        sprintf(sz, "%s/%02d.png", path, i);
        sf = fc->getSpriteFrameByName(sz);
        if (sf == nullptr)
        {
            if (i == 0)
            {
                return false;
            }
            break;
        }
        if (!pAni)
        {
            pAni = Animation::create();
            res = initWithSpriteFrame(sf);
        }
        pAni->addSpriteFrame(sf);
    }

    pAni->setDelayPerUnit(delay);
    AnimationCache::getInstance()->addAnimation(pAni, path);
    m_vecAnis.push_back(path);

    return res;
}

void Effect::play(int index /*= 0*/, float speed /*= 1.0f*/, int times /*= 1.0*/, FiniteTimeAction* done /*= nullptr*/)
{
    if (index >= (int)m_vecAnis.size())
    {
        return;
    }
    Animation* pAni = AnimationCache::getInstance()->getAnimation(m_vecAnis[index]);

    ActionInterval* actInner = Repeat::create(Animate::create(pAni), times);
    if (done != nullptr)
    {
        actInner = Sequence::createWithTwoActions(actInner, done);
    }

    Action* act = Speed::create(actInner, speed);
    act->setTag(CONST_ACT_TAG);
    stop();
    runAction(act);
}

void Effect::playRelease(int index, float speed /*= 1.0f*/, int times /*= 1.0*/)
{
    if (index >= (int)m_vecAnis.size())
    {
        return;
    }
    Animation* pAni = AnimationCache::getInstance()->getAnimation(m_vecAnis[index]);

    Action* act = Speed::create(Sequence::createWithTwoActions(Repeat::create(Animate::create(pAni), times), RemoveSelf::create()), speed);
    act->setTag(CONST_ACT_TAG);
    stop();
    runAction(act);
}

void Effect::playForever(int index, float speed /*= 1.0f*/)
{
    if (index >= (int)m_vecAnis.size())
    {
        return;
    }
    Animation* pAni = AnimationCache::getInstance()->getAnimation(m_vecAnis[index]);

    Action* act = Speed::create(RepeatForever::create(Animate::create(pAni)), speed);
    act->setTag(CONST_ACT_TAG);
    stop();
    runAction(act);
}

Animation* Effect::addAnimation(const char* path, float delay)
{
    SpriteFrameCache* fc = SpriteFrameCache::getInstance();
    SpriteFrame* sf = nullptr;
    bool res = false;
    char sz[256];
    Animation* pAni = nullptr;
    for (int i = 0; ; ++i)
    {
        sprintf(sz, "%s/%02d.png", path, i);
        sf = fc->getSpriteFrameByName(sz);
        if (sf == nullptr)
        {
            if (i == 0)
            {
                return nullptr;
            }
            break;
        }
        if (!pAni)
        {
            pAni = Animation::create();
        }
        pAni->addSpriteFrame(sf);
    }

    pAni->setDelayPerUnit(delay);
    AnimationCache::getInstance()->addAnimation(pAni, path);
    m_vecAnis.push_back(path);

    return pAni;
}

void Effect::stop()
{
    stopActionByTag(CONST_ACT_TAG);
}

// MenuEx
MenuEx* MenuEx::create()
{
    return MenuEx::create(nullptr, nullptr);
}

MenuEx* MenuEx::create(MenuItem* item, ...)
{
    va_list args;
    va_start(args, item);

    auto ret = MenuEx::createWithItems(item, args);

    va_end(args);

    return ret;
}

MenuEx* MenuEx::createWithArray(const Vector<MenuItem*>& arrayOfItems)
{
    auto ret = new MenuEx();
    if (ret && ret->initWithArray(arrayOfItems))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }

    return ret;
}

MenuEx* MenuEx::createWithItems(MenuItem* item, va_list args)
{
    Vector<MenuItem*> items;
    if (item)
    {
        items.pushBack(item);
        MenuItem *i = va_arg(args, MenuItem*);
        while (i)
        {
            items.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }

    return MenuEx::createWithArray(items);
}

MenuEx* MenuEx::createWithItem(MenuItem* item)
{
    return MenuEx::create(item, nullptr);
}

bool MenuEx::onTouchBegan(Touch* touch, Event* event)
{
    if (_state != Menu::State::WAITING || !_visible || !_enabled)
    {
        return false;
    }

    for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }

    _selectedItem = this->getItemForTouchEx(touch);
    if (_selectedItem && _selectedItem->isEnabled())
    {
        _state = Menu::State::TRACKING_TOUCH;
        _selectedItem->selected();

        return true;
    }

    return _selectedItem != nullptr;
}

void MenuEx::onTouchEnded(Touch* touch, Event* event)
{
    if (_state != Menu::State::TRACKING_TOUCH)
    {
        return;
    }

    Menu::onTouchEnded(touch, event);
}

void MenuEx::onTouchCancelled(Touch* touch, Event* event)
{
    if (_state != Menu::State::TRACKING_TOUCH)
    {
        return;
    }

    Menu::onTouchCancelled(touch, event);
}

void MenuEx::onTouchMoved(Touch* touch, Event* event)
{
    if (_state != Menu::State::TRACKING_TOUCH)
    {
        return;
    }

    Menu::onTouchMoved(touch, event);
}

MenuItem* MenuEx::getItemForTouchEx(Touch* touch)
{
    Point touchLocation = touch->getLocation();

    if (!_children.empty())
    {
        for (auto iter = _children.crbegin(); iter != _children.crend(); ++iter)
        {
            MenuItem* child = dynamic_cast<MenuItem*>(*iter);
            if (child && child->isVisible())
            {
                Point local = child->convertToNodeSpace(touchLocation);
                Rect r = child->rect();
                r.origin = Point::ZERO;

                if (r.containsPoint(local))
                {
                    return child;
                }
            }
        }
    }

    return nullptr;
}

// ButtonBase
ButtonBase::ButtonBase()
: m_iClickRetCode(0)
, m_iButtonIndex(0)
{
}

ButtonBase::~ButtonBase()
{
}

void ButtonBase::onCDBlickDone(Node* pNode)
{
    if (m_pBlink)
    {
        m_pBlink->setVisible(false);
    }
}

bool ButtonBase::initWithFile(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished)
{
    Node* pNormalSprite = nullptr;
    Node* pSelectedSprite = nullptr;
    Node* pDisabledSprite = nullptr;
    Node* pBlinkSprite = nullptr;
    Node* pMaskSprite = nullptr;

    pNormalImage && (pNormalSprite = Sprite::create(pNormalImage));
    pSelectedImage && (pSelectedSprite = Sprite::create(pSelectedImage));
    pDisabledImage && (pDisabledSprite = Sprite::create(pDisabledImage));
    pBlinkSprite && (pBlinkSprite = Sprite::create(pBlinkImage));
    pMaskSprite && (pMaskSprite = Sprite::create(pMaskImage));

    return initWithSprite(pNormalSprite, pSelectedSprite, pDisabledSprite, pBlinkSprite, pMaskSprite, fCoolDown, onClick, onFinished);
}

bool ButtonBase::initWithFrameName(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished)
{
    Node* pNormalSprite = nullptr;
    Node* pSelectedSprite = nullptr;
    Node* pDisabledSprite = nullptr;
    Node* pBlinkSprite = nullptr;
    Node* pMaskSprite = nullptr;

    pNormalImage && (pNormalSprite = Sprite::createWithSpriteFrameName(pNormalImage));
    pSelectedImage && (pSelectedSprite = Sprite::createWithSpriteFrameName(pSelectedImage));
    pDisabledImage && (pDisabledSprite = Sprite::createWithSpriteFrameName(pDisabledImage));
    pBlinkSprite && (pBlinkSprite = Sprite::createWithSpriteFrameName(pBlinkImage));
    pMaskSprite && (pMaskSprite = Sprite::createWithSpriteFrameName(pMaskImage));

    return initWithSprite(pNormalSprite, pSelectedSprite, pDisabledSprite, pBlinkSprite, pMaskSprite, fCoolDown, onClick, onFinished);
}

bool ButtonBase::initWithSprite(Node* pNormalSprite, Node* pSelectedSprite, Node* pDisabledSprite, Node* pBlinkSprite, Node* pMaskSprite, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished)
{
    if (!initWithNormalSprite(pNormalSprite, pSelectedSprite, pDisabledSprite, CC_CALLBACK_1(ButtonBase::onClick, this)))
    {
        return false;
    }

    m_onClick = onClick;
    m_onFinished = onFinished;
    setCoolDown(fCoolDown);
    Size sz = getContentSize();
    Point oAp = getAnchorPointInPoints();

    // Create ProgressTimer
    m_pPt = ProgressTimer::create((Sprite*)getNormalImage());
    addChild(m_pPt, 3);
    m_pPt->setPosition(oAp);
    m_pPt->setVisible(false);
    m_pPt->setType(ProgressTimer::Type::RADIAL);

    // Create Blink Image
    if (pBlinkSprite != nullptr)
    {
        m_pBlink = pBlinkSprite;
        addChild(m_pBlink, 2);
        m_pBlink->setPosition(oAp);
        m_pBlink->setVisible(false);
    }
    else
    {
        m_pBlink = nullptr;
    }

    if (pMaskSprite != nullptr)
    {
        m_pMask = pMaskSprite;
        addChild(m_pMask, 1);
        m_pMask->setPosition(oAp);
        m_pMask->setVisible(false);
    }
    else
    {
        m_pMask = nullptr;
    }

    // Create Label
    //m_pLabel = Label::create("5", "", 24, Size(sz.width - 8, 32), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom); //LabelBMFont::create("88", "fonts/futura-48.fnt", sz.width - 6, kCCTextAlignmentRight);//, Size(getContentSize().width - 6, 16.0), kCCTextAlignmentRight, "", 16.0
    //addChild(m_pLabel);
    //Size szLbl = m_pLabel->getContentSize();
    //m_pLabel->setPosition(Point(sz.width - szLbl.width / 2 - 4, szLbl.height / 2));
    //m_pLabel->setColor(ccRED);

    return true;
}

void ButtonBase::activate()
{
    if (!isEnabled())
    {
        return;
    }

    MenuItemImage::activate();
}

void ButtonBase::unselected()
{
    MenuItemImage::unselected();

    if (!isEnabled())
    {
        updateImagesVisibility();
    }
}

void ButtonBase::onClick(Ref* pObject)
{
    setClickRetCode(0);
    if (m_onClick)
    {
        m_onClick(this);
    }

    if (getClickRetCode() < 0)
    {
        return;
    }

    //coolDown();  // for test
}

void ButtonBase::onCoolDownDone(Node* pNode)
{
    if (getCoolDown())
    {
        if (m_pMask != nullptr)
        {
            m_pMask->setVisible(false);
        }
        
        m_pPt->setVisible(false);
        this->setEnabled(true);

        if (m_pBlink != nullptr)
        {
            m_pBlink->setVisible(true);
            m_pBlink->runAction(Sequence::create(FadeIn::create(0.25f), FadeOut::create(0.25f), CallFuncN::create(CC_CALLBACK_1(ButtonBase::onCDBlickDone, this)), nullptr));
        }
    }

    if (m_onFinished)
    {
        m_onFinished(this);
    }
}

void ButtonBase::coolDown(float fFromPercent)
{
    if (getCoolDown())
    {
        float fCoolDownReal = getCoolDown() * (100.0 - fFromPercent) / 100.0;
        if (m_pMask != nullptr)
        {
            m_pMask->setVisible(true);
        }
        
        m_pPt->setVisible(true);
        this->setEnabled(false);

        if (_disabledImage != nullptr)
        {
            _disabledImage->stopAllActions();
            DCAST(_disabledImage, Sprite*)->setOpacity(0x50 + (0xFF - 0x50) * fFromPercent / 100.0);
            _disabledImage->runAction(FadeTo::create(fCoolDownReal, 0xFF));
        }
        

        //m_pPt->setOpacity(0x7F + (0xFF - 0x7F) * fFromPercent / 100.0);
        ProgressFromTo* pPro = ProgressFromTo::create(fCoolDownReal, fFromPercent, 100.0f);

        m_pPt->stopAllActions();
        m_pPt->runAction(
            //Spawn::createWithTwoActions(
            //FadeTo::create(fCoolDownReal, 0xFF),
            Sequence::create(pPro, CallFuncN::create(CC_CALLBACK_1(ButtonBase::onCoolDownDone, this)),
            nullptr));
    }
    else
    {
        onCoolDownDone(this);
    }
}

float ButtonBase::getPercentage() const
{
    return m_pPt->getPercentage();
}

// ButtonNormal
ButtonNormal::ButtonNormal()
: m_fCoolDown(0.0f)
{
}

// ButtonPanel
ButtonPanel::ButtonPanel()
: m_iRow(0)
, m_iColumn(0)
, m_fButtonWidth(0.0f)
, m_fButtonHeight(0.0f)
, m_fHorBorderWidth(0.0f)
, m_fVerBorderWidth(0.0f)
, m_pInnerMenu(nullptr)
, m_pBackground(nullptr)
, m_iOwnerKey(0)
, m_ppBtnPos(nullptr)
, m_pRetain(nullptr)
, m_iCount(0)
{
}

ButtonPanel::~ButtonPanel()
{
    if (m_ppBtnPos)
    {
        delete[] m_ppBtnPos;
    }
    CC_SAFE_RELEASE(m_pRetain);
}

const float ButtonPanel::CONST_ACTION_DURATION = 0.25;

bool ButtonPanel::init(int iRow, int iColumn, float fButtonWidth, float fButtonHeight, float fHorBorderWidth, float fVerBorderWidth, Sprite* pBackground, float fBackgroundOffsetX, float fBackgroundOffsetY)
{
    m_iRow = iRow;
    m_iColumn = iColumn;
    m_fButtonWidth = fButtonWidth;
    m_fButtonHeight = fButtonHeight;
    m_fHorBorderWidth = fHorBorderWidth;
    m_fVerBorderWidth = fVerBorderWidth;

    setAnchorPoint(Point(0.5f, 0.5f));
    Size oSz = Size(m_fHorBorderWidth * (m_iColumn - 1) + m_fButtonWidth * m_iColumn, m_fVerBorderWidth * (m_iRow - 1) + m_fButtonHeight * m_iRow);
    setContentSize(oSz);

    if (pBackground)
    {
        m_pBackground = pBackground;
        addChild(m_pBackground);
        m_pBackground->setPosition(getAnchorPointInPoints() + Point(fBackgroundOffsetX, fBackgroundOffsetY));
    }
    else
    {
        m_pBackground = nullptr;
    }

    m_iOwnerKey = 0;

    m_pInnerMenu = MenuEx::create();
    addChild(m_pInnerMenu);
    m_pInnerMenu->setContentSize(getContentSize());
    m_pInnerMenu->setPosition(Point::ZERO);

    if (m_ppBtnPos)
    {
        delete[] m_ppBtnPos;
    }
    CC_SAFE_RELEASE(m_pRetain);

    size_t uCount = iRow * iColumn;
    m_ppBtnPos = new ButtonBase*[uCount];
    memset(m_ppBtnPos, 0, sizeof(ButtonBase*) * uCount);

    m_iCount = 0;

    setCascadeOpacityEnabled(true);

    return true;
}

void ButtonPanel::addButton(ButtonBase* pButton, int iIndex)
{
    CCAssert(iIndex < m_iRow * m_iColumn, "Break Bounds");
    CCAssert(m_iCount <= m_iRow * m_iColumn, "already full");
    ButtonBase* pBtn = getButton(iIndex);
    if (pBtn)
    {
        delButton(iIndex);
    }

    m_pInnerMenu->addChild(pButton);
    pButton->setPosition(index2Point(iIndex));
    pButton->setButtonIndex(iIndex);

    m_ppBtnPos[iIndex] = pButton;

    ++m_iCount;
}

void ButtonPanel::addButton(ButtonBase* pButton, int iX, int iY)
{
    CCAssert(iY < m_iRow && iX < m_iColumn, "Break Bounds");
    addButton(pButton, toIndex(iX, iY));
}

void ButtonPanel::delButton(int iIndex)
{
    --m_iCount;
    m_pInnerMenu->removeChild(m_ppBtnPos[iIndex], true);
    m_ppBtnPos[iIndex] = nullptr;
}
void ButtonPanel::delButton(ButtonBase* pButton)
{
    int iIndex = getButtonIndex(pButton);
    CCAssert(iIndex >= 0, "button not found");
    delButton(iIndex);
}

void ButtonPanel::delButton(int iX, int iY)
{
    CCAssert(iY < m_iRow && iX < m_iColumn, "Break Bounds");
    delButton(toIndex(iX, iY));
}

int ButtonPanel::allotSlot(ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/)
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iStartY = bY ? 0 : (m_iRow - 1);
    int iStartX = bX ? 0 : (m_iColumn - 1);

    int iEndY = bY ? (m_iRow - 1) : 0;
    int iEndX = bX ? (m_iColumn - 1) : 0;

    return allotSlot(iStartX, iStartY, iEndX, iEndY, eVer, eHor);
}

int ButtonPanel::allotSlot(int iStartX, int iStartY, int iEndX, int iEndY, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor)
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iY = bY ? 1 : -1;
    int iX = bX ? 1 : -1;

    int iIndex;
    for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
    {
        int iStartX0 = (y == iStartY ? iStartX : (bX ? 0 : (m_iColumn - 1)));
        int iEndX0 = (y == iEndY ? iEndX : (bX ? (m_iColumn - 1) : 0));
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

void ButtonPanel::clearUpSlot(ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/)
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iY = bY ? 1 : -1;
    int iX = bX ? 1 : -1;

    int iStartY = bY ? 0 : (m_iRow - 1);
    int iEndY = bY ? (m_iRow - 1) : 0;

    int iStartX = bX ? 0 : (m_iColumn - 1);
    int iEndX = bX ? (m_iColumn - 1) : 0;

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

ButtonBase* ButtonPanel::getButton(int iX, int iY) const
{
    return getButton(toIndex(iX, iY));
}

ButtonBase* ButtonPanel::getButton(int iIndex) const
{
    CCAssert(iIndex < m_iRow * m_iColumn, "Break Bounds");
    return m_ppBtnPos[iIndex];
}

ButtonBase* ButtonPanel::getButton(const function<bool(ButtonBase*)>& match) const
{
    int n = m_iRow * m_iColumn;
    for (int i = 0; i < n; ++i)
    {
        if (m_ppBtnPos[i] && match(m_ppBtnPos[i]))
        {
            return m_ppBtnPos[i];
        }
    }

    return nullptr;
}

int ButtonPanel::getButtonIndex(ButtonBase* pButton) const
{
    int n = m_iRow * m_iColumn;
    for (int i = 0; i < n; ++i)
    {
        if (m_ppBtnPos[i] == pButton)
        {
            return i;
        }
    }

    return -1;
}

bool ButtonPanel::isFull()
{
    return m_iCount == m_iRow * m_iColumn;
}

int ButtonPanel::index2Y(int iIndex) const
{
    return iIndex / m_iColumn;
}

int ButtonPanel::index2X(int iIndex) const
{
    return iIndex % m_iColumn;
}

void ButtonPanel::retainButton(ButtonBase* pButton)
{
    CC_SAFE_RETAIN(pButton);
    CC_SAFE_RELEASE(m_pRetain);
    m_pRetain = pButton;
}

ButtonBase* ButtonPanel::getRetainButton() const
{
    return m_pRetain;
}

int ButtonPanel::toIndex(int iX, int iY) const
{
    return iY * m_iColumn + iX;
}

void ButtonPanel::addButtonEx(ButtonBase* pButton, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/)
{
    addButton(pButton, allotSlot(eVer, eHor));
}

void ButtonPanel::moveButton(int iIndexSrc, int iIndexDst)
{
    ButtonBase* pSrc = getButton(iIndexSrc);
    m_ppBtnPos[iIndexDst] = pSrc;
    m_ppBtnPos[iIndexSrc] = nullptr;
    pSrc->setPosition(index2Point(iIndexDst));
}

Point ButtonPanel::index2Point(int iIndex)
{
    const Size& roSz = getContentSize();
    int iX = index2X(iIndex);
    int iY = index2Y(iIndex);
    return Point(
        m_fButtonWidth * 0.5 + (m_fButtonWidth + m_fHorBorderWidth) * iX,
        m_fButtonHeight * 0.5 + (m_fButtonHeight + m_fVerBorderWidth) * iY);
}

void ButtonPanel::pushAction(const ACTION_NODE& roAct)
{
    m_lstActs.push_back(roAct);
    if (m_lstActs.size() == 1)
    {
        onPrevActEnd(nullptr);
    }
}

void ButtonPanel::onPrevActEnd(Node* pNode)
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
            //m_pInnerMenu->removeChild(m_ppBtnPos[rNode.stDel.iIndex], true);
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
        rNode.stAdd.pBtn->runAction(Sequence::create(FadeIn::create(CONST_ACTION_DURATION), nullptr));
        onPrevActEnd(rNode.stAdd.pBtn);
        break;

    case kDel:
        rNode.stDel.pBtn->stopAllActions();
        rNode.stDel.pBtn->runAction(Sequence::create(FadeOut::create(CONST_ACTION_DURATION), CallFuncN::create(CC_CALLBACK_1(ButtonPanel::onPrevActEnd, this)), nullptr));
        break;

    case kMove:
        rNode.stMove.pBtn->runAction(Sequence::create(MoveTo::create(CONST_ACTION_DURATION, index2Point(rNode.stMove.iIndexDst)), CCCallFuncN::create(CC_CALLBACK_1(ButtonPanel::onPrevActEnd, this)), nullptr));
        break;

    case kAddEx:
        addButton(rNode.stAddEx.pBtn, allotSlot(rNode.stAddEx.eVer, rNode.stAddEx.eHor));
        CC_SAFE_RELEASE(rNode.stAddEx.pBtn);
        rNode.stAddEx.pBtn->setOpacity(0);
        rNode.stAddEx.pBtn->runAction(Sequence::create(FadeIn::create(CONST_ACTION_DURATION), nullptr));
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

            int iStartX = bX ? 0 : (m_iColumn - 1);
            int iEndX = bX ? (m_iColumn - 1) : 0;

            int iMove = 0;
            m_lstActs.pop_front();
            ButtonBase* pBtn;
            int iEmpty = -1;
            int iIndex;
            for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
            {
                int iStartX0 = (y == iStartY ? iStartX : (bX ? 0 : (m_iColumn - 1)));
                int iEndX0 = (y == iEndY ? iEndX : (bX ? (m_iColumn - 1) : 0));
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
                            m_ppBtnPos[iIndex] = nullptr;

                            m_lstActs.push_front(ACTION_NODE()); // empty action
                            ++iMove;

                            pBtn->runAction(Sequence::create(MoveTo::create(CONST_ACTION_DURATION, index2Point(iEmpty)), CCCallFuncN::create(CC_CALLBACK_1(ButtonPanel::onPrevActEnd, this)), nullptr));
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
                onPrevActEnd((Node*)(1));
            }
        }

        break;

    default:
        ;
    }
}

void ButtonPanel::pushAddButtonAction(ButtonBase* pButton, int iIndex)
{
    CC_SAFE_RETAIN(pButton);
    pushAction(ACTION_NODE(pButton, iIndex));
}

void ButtonPanel::pushDelButtonAction(int iIndex, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/, bool bClearUp /*= true*/)
{
    ACTION_NODE stNode(iIndex, eVer, eHor, bClearUp);
    stNode.stDel.pBtn = getButton(iIndex);
    pushAction(stNode);
}

void ButtonPanel::pushMoveButtonAction(int iIndexSrc, int iIndexDst)
{
    ACTION_NODE stNode(iIndexSrc, iIndexDst);
    stNode.stMove.pBtn = getButton(iIndexSrc);
    pushAction(stNode);
}

void ButtonPanel::pushAddButtonExAction(ButtonBase* pButton, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/)
{
    CC_SAFE_RETAIN(pButton);
    pushAction(ACTION_NODE(pButton, eVer, eHor));
}

void ButtonPanel::pushClearUpSlotAction(ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/)
{
    pushAction(ACTION_NODE(eVer, eHor));
}

int ButtonPanel::getMaxCount() const
{
    return m_iRow * m_iColumn;
}

int ButtonPanel::getCount() const
{
    return m_iCount;
}

// PopPanel
PopPanel::PopPanel()
: m_mn(nullptr)
{
}

bool PopPanel::initWithSize(const Size& size)
{
    setAnchorPoint(Point(0.5f, 0.5f));
    setContentSize(size);
    m_mn = MenuEx::create();
    addChild(m_mn, 10);

    setCascadeOpacityEnabled(true);

    return true;
}

bool PopPanel::initWithBackground(Sprite* background)
{
    setAnchorPoint(Point(0.5f, 0.5f));
    setContentSize(background->getContentSize());
    m_mn = MenuEx::create();
    addChild(m_mn, 10);

    addChild(background);
    background->setPosition(getAnchorPointInPoints());

    setCascadeOpacityEnabled(true);

    return true;
}

void PopPanel::setBackground(Sprite* background, bool tile)
{
    addChild(background);
    background->setPosition(getAnchorPointInPoints());

    const Size& bgSz = background->getContentSize();
    const Size& sz = getContentSize();
    if (tile)
    {
        background->setScaleX(sz.width / bgSz.width);
        background->setScaleY(sz.height / bgSz.height);
    }
    else
    {
        background->setScale(min(sz.width / bgSz.width, sz.height / bgSz.height));
    }
}

void PopPanel::addButton(MenuItem* mi)
{
    m_mn->addChild(mi);
}

void PopPanel::onClickClose(Ref* obj)
{
    hide();
}

void PopPanel::show()
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    runAction(EaseExponentialOut::create(Spawn::createWithTwoActions(MoveTo::create(0.5f, Point(wsz.width * 0.5, wsz.height * 0.5)), Sequence::createWithTwoActions(DelayTime::create(0.25f), FadeIn::create(0.25f)))));
}

void PopPanel::hide()
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    stopAllActions();
    runAction(EaseExponentialOut::create(Spawn::createWithTwoActions(MoveTo::create(0.5f, Point(wsz.width * 0.5, wsz.height + getContentSize().height * getScaleY() * 0.5)), Sequence::createWithTwoActions(DelayTime::create(0.25f), FadeOut::create(0.25f)))));
}

// Utils
Image* Utils::nodeToImage(Node* node)
{
    static Size rsz;
    static RenderTexture* rt = nullptr;
    if (rt != nullptr && !node->getContentSize().equals(rsz))
    {
        rt->release();
        rt = nullptr;
    }

    if (rt == nullptr)
    {
        rsz = node->getContentSize();
        rt = RenderTexture::create(rsz.width, rsz.height, Texture2D::PixelFormat::RGBA8888);
        rt->retain();
        rt->setKeepMatrix(true);
    }
    
    Point pos = node->getPosition();
    node->setPosition(node->getAnchorPointInPoints());
    //node->setPosition(Point(rsz.width * 0.5, rsz.height * 0.5));
    rt->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    node->visit();
    rt->end();
    Director::getInstance()->getRenderer()->render();
    node->setPosition(pos);

    return rt->newImage();
}

Image* Utils::transformImage(Image* image, const FUNC_TRAN& funcTransform)
{
    Color4B* data = (Color4B*)image->getData();
    
    GLushort w = image->getWidth();
    GLushort h = image->getHeight();

    for (GLushort y = 0; y < h; ++y)
    {
        for (GLushort x = 0; x < w; ++x)
        {
            funcTransform(data, x, y, w, h);
            ++data;
        }
    }

    return image;
}

Texture2D* Utils::nodeToTexture(Node* node, const FUNC_TRAN& funcTransform)
{
    auto image = nodeToImage(node);

    if (funcTransform != nullptr)
    {
        transformImage(image, funcTransform);
    }

    auto ret = new Texture2D;
    ret->initWithImage(image);
    delete image;

    return ret;
}

bool Utils::nodeToFile(Node* node, const char* file, const FUNC_TRAN& funcTransform /*= nullptr*/)
{
    auto image = nodeToImage(node);

    if (funcTransform != nullptr)
    {
        transformImage(image, funcTransform);
    }

    bool ret = image->saveToFile(FileUtils::getInstance()->fullPathForFilename(file).c_str(), false);
    delete image;

    return ret;
}

void Utils::tranGrayscale(Color4B* c, GLushort x, GLushort y, GLushort w, GLushort h)
{
    GLubyte gray = (c->r + (c->g << 1) + c->b) >> 2;
    gray = max(0, gray - 100);
    c->r = gray;
    c->g = gray;
    c->b = gray;
}

void Utils::tranFillColor(Color4B* c, GLushort x, GLushort y, GLushort w, GLushort h, Color4B* fill)
{
    *c = *fill;
}

void Utils::tranFillAlpha(Color4B* c, GLushort x, GLushort y, GLushort w, GLushort h, GLushort a)
{
    c->a = a;
}
