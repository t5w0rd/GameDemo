#ifndef __COMPONENTFORCC_H__
#define __COMPONENTFORCC_H__


class CCProgressBar : public CCNode
{
public:
    static const float CONST_MAX_PROCESS_BAR_PERCENT;

public:
    CCProgressBar();
    virtual ~CCProgressBar();

    virtual bool init(const CCSize& roSize, CCSprite* pFill, CCSprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop);
    M_CREATE_FUNC_PARAM(CCProgressBar, (const CCSize& roSize, CCSprite* pFill, CCSprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop), roSize, pFill, pBorder, fHorizBorderWidth, fVertBorderWidth, bFillOnTop);

    virtual void setPercentage(float fPercent);
    virtual void setPercentage(float fPercent, float fDuration, CCFiniteTimeAction* pEndAction = NULL);
    virtual CCActionInterval* setPercentageAction(float fPercent, float fDuration, CCFiniteTimeAction* pEndAction = NULL);
    virtual void setFillColor(const ccColor3B& roColor);
    void runActionForTimer(CCAction* pAction);

public:
    CCProgressTimer* m_pPt;
};

typedef std::set<std::string> SET_STR;
class CCSpriteFrameCacheEx : public CCSpriteFrameCache
{
public:
    CCDictionary* getSpriteFrames();
    CCDictionary* getSpriteFramesAliases();
    SET_STR*  getLoadedFileNames();
};

class CCGameFile : public CCObject
{
public:
    enum FILE_ORIGIN
    {
        kBegin = SEEK_SET,
        kCur = SEEK_CUR,
        kEnd = SEEK_END
    };

public:
    CCGameFile(void);
    virtual ~CCGameFile(void);
    virtual bool init(const char* pFileName, const char* pMode);
    M_CREATE_FUNC_PARAM(CCGameFile, (const char* pFileName, const char* pMode), pFileName, pMode);

    template <typename TYPE>
    size_t read(TYPE* pData, size_t uCount = 1);
    size_t tell() const;
    bool eof() const;
    bool seek(long lOffset, FILE_ORIGIN eOrigin);

protected:
    uint8_t* m_pData;
    unsigned long m_uSize;
    unsigned long m_uPos;
};

class CCWinLayer : public CCLayer
{
public:
    enum TOUCH_ACTION_INDEX
    {
        kNormalTouch = 0, // move, attack, select
        kUnitCastTarget = 1,
        kClickPoint = 2,
        kSlideWindow = 3
    };

public:
    static const float CONST_MIN_MOVE_DELTA;
    static const float CONST_MAX_CAN_MOVE_DURATION;

public:
    CCWinLayer();
    virtual bool init();
    CREATE_FUNC(CCWinLayer);

    // default implements are used to call script callback if exist
    void setBackgroundSprite(CCSprite* pSprite);
    void setBackgroundSprite(CCSprite* pSprite, int zOrder, int tag);
    void setBufferEffectParam(float fScale, float fMoveK, float fBuffRange, float fEdgeK);
    virtual void setScale(float fScale);
    virtual void setPosition(const CCPoint& newPosition);
    
    float getTouchMovedDuration() const;
    float getTouchMovedDistance() const;
    float getTouchMovedRadian() const;
    virtual bool isSlideAction() const;
    virtual bool isClickAction() const;
    
    M_SYNTHESIZE_BOOL(MoveEnabled);

protected:
    virtual int touchActionIndex() const;
    virtual void onEnter();
    virtual void onExit();
    void bufferWindowEffect(float fDt);
    //void adjustWinPos(CCPoint& roPos);

    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);

protected:
    int m_iPendingAbilityOwner;
    bool m_bIsTouching;
    float m_fMoveK;
    float m_fBuffRange;
    float m_fEdgeK;
    float m_fMoveDelta;
    CCPoint m_oMoveStart;
    float m_fTouchMovedDuration;
    float m_fMoveR;
    bool m_bCanMove;
    CCSprite* m_pBackground;

    CCPoint m_oLast;
    float m_fStartScale;
    float m_fStartDis;
};

class CCTouchSprite : public CCSprite, public CCTargetedTouchDelegate
{
public:
    CCTouchSprite();
    M_CREATE_INITWITH_FUNC_PARAM(File, CCTouchSprite, (const char *pszFilename), pszFilename);
    M_CREATEWITH_FUNC_PARAM(SpriteFrame, CCTouchSprite, (CCSpriteFrame *pSpriteFrame), pSpriteFrame);
    M_CREATEWITH_FUNC_PARAM(Texture, CCTouchSprite, (CCTexture2D *pTexture), pTexture);
    M_CREATEWITH_FUNC_PARAM(Texture, CCTouchSprite, (CCTexture2D *pTexture, const CCRect& rect), pTexture, rect);
    static CCTouchSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    
    virtual CCObject* copyWithZone(CCZone *pZone);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    bool containsTouchLocation(CCTouch* touch);
    
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    
protected:
    enum TOUCH_SPRITE_STATE 
    {
        kStateUngrabbed,
        kStateGrabbed
    }; 
    TOUCH_SPRITE_STATE m_state;
};

class CCEffect : public CCSprite
{
protected:
    const int CONST_ACT_TAG;

public:
    CCEffect();
    virtual ~CCEffect();

    virtual bool initWithPath(const char* path, float delay);
    M_CREATE_INITWITH_FUNC_PARAM(Path, CCEffect, (const char* path, float delay), path, delay);

    void play(int index = 0, float speed = 1.0f, int times = 1.0, CCObject* target = NULL, SEL_CallFuncN done = NULL);
    void playRelease(int index = 0, float speed = 1.0f, int times = 1.0);
    void playForever(int index = 0, float speed = 1.0f);

    CCAnimation* addAnimation(const char* path, float delay);
    void stop();

    typedef vector<string> VEC_ANIS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_ANIS, m_vecAnis, AnimationNames);
};

class CCButtonBase : public CCMenuItemImage
{
protected:
    CCButtonBase();
    virtual ~CCButtonBase();

public:
    virtual bool initWithFile(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished);
    virtual bool initWithFrameName(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished);
    virtual bool initWithSprite(CCSprite* pNormalSprite, CCSprite* pSelectedSprite, CCSprite* pDisabledSprite, CCSprite* pBlinkSprite, CCSprite* pMaskSprite, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished);
    //CREATE_FUNC_PARAM(CCButtonBase, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
    virtual void setCoolDown(float fDuration) = 0;
    virtual float getCoolDown() const = 0;
    void setLabel(const char* pLabel);
    CC_SYNTHESIZE(int, m_iClickRetCode, ClickRetCode);

    CC_SYNTHESIZE(int, m_iButtonIndex, ButtonIndex);

    virtual void coolDown(float fFromPercent = 0);
    float getPercentage() const;

private:
    void onClick(CCObject* pObject);
    void onCoolDownDone(CCNode* pNode);
    void onCDBlickDone(CCNode* pNode);

protected:
    CCObject* m_pTarget;
    SEL_CallFuncN m_pOnClick;
    SEL_CallFuncN m_pOnFinished;
    CCProgressTimer* m_pPt;
    CCFiniteTimeAction* m_pAct;
    CCSprite* m_pBlink;
    CCSprite* m_pMask;

};

class CCButtonNormal : public CCButtonBase
{
public:
    M_CREATEWITH_FUNC_PARAM(File, CCButtonNormal, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
    M_CREATEWITH_FUNC_PARAM(FrameName, CCButtonNormal, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
    M_CREATE_INITWITH_FUNC_PARAM(Sprite, CCButtonNormal, (CCSprite* pNormalSprite, CCSprite* pSelectedSprite, CCSprite* pDisabledSprite, CCSprite* pBlinkSprite, CCSprite* pMaskSprite, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished), pNormalSprite, pSelectedSprite, pDisabledSprite, pBlinkSprite, pMaskSprite, fCoolDown, pTarget, pOnClick, pOnFinished);
    
    CC_SYNTHESIZE(float, m_fCoolDown, CoolDown);
};

class CCCommmButton : public CCButtonNormal
{
public:
    virtual bool init (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iKey);
    M_CREATEWITH_FUNC_PARAM(File, CCButtonNormal, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
    M_CREATEWITH_FUNC_PARAM(FrameName, CCButtonNormal, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
    M_CREATE_INITWITH_FUNC_PARAM(Sprite, CCButtonNormal, (CCSprite* pNormalSprite, CCSprite* pSelectedSprite, CCSprite* pDisabledSprite, CCSprite* pBlinkSprite, CCSprite* pMaskSprite, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished), pNormalSprite, pSelectedSprite, pDisabledSprite, pBlinkSprite, pMaskSprite, fCoolDown, pTarget, pOnClick, pOnFinished);

public:
    int m_iKey;

};

class CCButtonPanel : public CCNode
{
public:
    enum ADD_HORIZONTAL
    {
        kLeftToRight,
        kRightToLeft
    };

    enum ADD_VERTICAL
    {
        kBottomToTop,
        kTopToBottom
    };

    enum ACTION_CMD
    {
        kUnknown,
        kAdd,
        kDel,
        kMove,
        kAddEx,
        kClearUp
    };

    static const float CONST_ACTION_DURATION;

    struct ACTION_NODE
    {
        ACTION_NODE() { memset(this, 0, sizeof(ACTION_NODE)); }
        ACTION_NODE(CCButtonBase* pBtn, int iIndex): eAct(kAdd) { stAdd.pBtn = pBtn; stAdd.iIndex = iIndex; }
        ACTION_NODE(int iIndex, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor, bool bClearUp): eAct(kDel) { stDel.pBtn = NULL; stDel.iIndex = iIndex; stDel.eVer = eVer; stDel.eHor = eHor; stDel.bClearUp = bClearUp; }
        ACTION_NODE(int iIndexSrc, int iIndexDst): eAct(kMove) { stMove.pBtn = NULL; stMove.iIndexSrc = iIndexSrc; stMove.iIndexDst = iIndexDst; }
        ACTION_NODE(CCButtonBase* pBtn, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor): eAct(kAddEx) { stAddEx.pBtn = pBtn; stAddEx.eVer = eVer; stAddEx.eHor = eHor; }
        ACTION_NODE(ADD_VERTICAL eVer, ADD_HORIZONTAL eHor): eAct(kClearUp) { stClearUp.eVer = eVer; stClearUp.eHor = eHor; }
        //~ACTION_NODE() { if (eAct == kAdd) {CC_SAFE_RELEASE(stAdd.pBtn); } else if (eAct == kAddEx) {CCLOG("release");CC_SAFE_RELEASE(stAddEx.pBtn);} }

        ACTION_CMD eAct;
        union
        {
            struct // ADD
            {
                CCButtonBase* pBtn;
                int iIndex;
            } stAdd;

            struct // DEL
            {
                CCButtonBase* pBtn;
                int iIndex;
                ADD_VERTICAL eVer;
                ADD_HORIZONTAL eHor;
                bool bClearUp;
            } stDel;

            struct // MOVE
            {
                CCButtonBase* pBtn;
                int iIndexSrc;
                int iIndexDst;
            } stMove;

            struct // ADDEX
            {
                CCButtonBase* pBtn;
                ADD_VERTICAL eVer;
                ADD_HORIZONTAL eHor;
            } stAddEx;

            struct // CLEARUP
            {
                ADD_VERTICAL eVer;
                ADD_HORIZONTAL eHor;
            } stClearUp;
        };
    };

    typedef list<ACTION_NODE> LIST_ACTION;

public:
    CCButtonPanel();
    virtual ~CCButtonPanel();

    virtual bool init(int iRow, int iColumn, float fButtonWidth, float fButtonHeight, float fBorderWidth, float fInnerBorderWidth, CCSprite* pBackground, float fOffsetX = 0.0f, float fOffsetY = 0.0f);
    M_CREATE_FUNC_PARAM(CCButtonPanel, (int iRow, int iColumn, float fButtonWidth, float fButtonHeight, float fBorderWidth, float fInnerBorderWidth, CCSprite* pBackground, float fOffsetX = 0.0f, float fOffsetY = 0.0f), iRow, iColumn, fButtonWidth, fButtonHeight, fBorderWidth, fInnerBorderWidth, pBackground, fOffsetX, fOffsetY);

    void addButton(CCButtonBase* pButton, int iIndex); // org
    void addButton(CCButtonBase* pButton, int iX, int iY);

    void delButton(int iIndex); // org
    void delButton(int iX, int iY);
    void delButton(CCButtonBase* pButton);

    void moveButton(int iIndexSrc, int iIndexDst); // org

    void addButtonEx(CCButtonBase* pButton, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    int allotSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);
    int allotSlot(int iStartX, int iStartY, int iEndX, int iEndY, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor);
    void clearUpSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    void retainButton(CCButtonBase* pButton);
    CCButtonBase* getRetainButton() const;

    CCButtonBase* getButton(int iX, int iY) const;
    CCButtonBase* getButton(int iIndex) const;

    int getButtonIndex(CCButtonBase* pButton) const;

    int index2Y(int iIndex) const;
    int index2X(int iIndex) const;
    int toIndex(int iX, int iY) const;
    CCPoint index2Point(int iIndex);

    bool isFull();

    void pushAction(const ACTION_NODE& roAct);
    void onPrevActEnd(CCNode* pNode);

    void pushAddButtonAction(CCButtonBase* pButton, int iIndex);
    void pushDelButtonAction(int iIndex, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight, bool bClearUp = true);
    void pushMoveButtonAction(int iIndexSrc, int iIndexDst);
    void pushAddButtonExAction(CCButtonBase* pButton, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);
    void pushClearUpSlotAction(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    int getMaxCount() const;
    int getCount() const;

public:
    CC_SYNTHESIZE(int, m_iRow, Row);
    CC_SYNTHESIZE(int, m_iColumn, Column);
    CC_SYNTHESIZE(float, m_fButtonWidth, ButtonWidth);
    CC_SYNTHESIZE(float, m_fButtonHeight, ButtonHeight);
    CC_SYNTHESIZE(float, m_fOffsetX, OffsetX);
    CC_SYNTHESIZE(float, m_fOffsetY, OffsetY);
    CC_SYNTHESIZE(float, m_fInnerBorderWidth, InnerBorderWidth);
    CC_SYNTHESIZE(float, m_fBorderWidth, BorderWidth);
    CC_SYNTHESIZE(CCMenu*, m_pInnerMenu, InnerMenu);
    CC_SYNTHESIZE(CCSprite*, m_pBackground, Background);

public:
    int m_iOwnerKey;
    CCButtonBase** m_ppBtnPos;
    CCButtonBase* m_pRetain;
    LIST_ACTION m_lstActs;
    int m_iCount;
};

////////////////////////////////// Inline ////////////////////////////////////////
// CCGameFile
template <typename TYPE>
size_t CCGameFile::read( TYPE* pData, size_t uCount /*= 1*/ )
{
    size_t uRead;
    TYPE* pCur = (TYPE*)((size_t)m_pData + m_uPos);
    for (uRead = 0; uRead < uCount && m_uPos + (uRead + 1) * sizeof(TYPE) <= m_uSize; ++uRead);
    size_t uReadSize = uRead * sizeof(TYPE);
    memmove(pData, pCur, uReadSize);
    m_uPos += uReadSize;
    return uRead;
}



#endif  /* __COMPONENTFORCC_H__ */
