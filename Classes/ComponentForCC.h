#ifndef __COMPONENTFORCC_H__
#define __COMPONENTFORCC_H__


class ProgressBar : public Node
{
public:
    static const float CONST_MAX_PROCESS_BAR_PERCENT;

public:
    ProgressBar();
    virtual ~ProgressBar();

    virtual bool init(const Size& roSize, Sprite* pFill, Sprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop);
    M_CREATE_FUNC_PARAM(ProgressBar, (const Size& roSize, Sprite* pFill, Sprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop), roSize, pFill, pBorder, fHorizBorderWidth, fVertBorderWidth, bFillOnTop);

    virtual void setPercentage(float fPercent);
    virtual void setPercentage(float fPercent, float fDuration, FiniteTimeAction* pEndAction = nullptr);
    virtual ActionInterval* setPercentageAction(float fPercent, float fDuration, FiniteTimeAction* pEndAction = nullptr);
    virtual void setFillColor(const Color3B& roColor);
    void runActionForTimer(Action* pAction);

public:
    ProgressTimer* m_pPt;
};

class SpriteFrameCacheEx : public SpriteFrameCache
{
public:
    Map<std::string, SpriteFrame*>* getSpriteFrames();
    ValueMap* getSpriteFramesAliases();
    std::set<std::string>* getLoadedFileNames();
};

class GameFile : public Ref
{
public:
    enum FILE_ORIGIN
    {
        kBegin = SEEK_SET,
        kCur = SEEK_CUR,
        kEnd = SEEK_END
    };

public:
    GameFile(void);
    virtual ~GameFile(void);
    virtual bool init(const char* pFileName, const char* pMode);
    M_CREATE_FUNC_PARAM(GameFile, (const char* pFileName, const char* pMode), pFileName, pMode);

    template <typename TYPE>
    ssize_t read(TYPE* pData, ssize_t uCount = 1);
    ssize_t tell() const;
    bool eof() const;
    bool seek(long lOffset, FILE_ORIGIN eOrigin);

protected:
    Data m_data;
    ssize_t m_uPos;
};

class WinLayer : public LayerColor
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
    WinLayer();
    virtual bool init();
    M_CREATE_FUNC_PARAM(WinLayer, ());
    virtual bool initWithColor(const Color4B& color);
    M_CREATE_INITWITH_FUNC_PARAM(Color, WinLayer, (const Color4B& color), color);

    M_SYNTHESIZE_READONLY_PASS_BY_REF(Size, m_oWinSize, WinSize);
    virtual void setWinSize(const Size& rWinSize);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(Point, m_oWinPosition, WinPosition);
    virtual void setWinPosition(const Point& rOffsetPosition);
    void setOffsetPosition(const Point& rWinPosition);
    Point getOffsetPosition();
    // default implements are used to call script callback if exist
    void setBackgroundSprite(Sprite* pSprite);
    void setBackgroundSprite(Sprite* pSprite, int zOrder, int tag);
    void setBufferEffectParam(float fScale, float fMoveK, float fBuffRange, float fEdgeK);
    virtual void setScale(float fScale) override;
    virtual void setPosition(const Point& newPosition) override;
    virtual void setContentSize(const Size& rSize) override;
    
    float getTouchMovedDuration() const;
    float getTouchMovedDistance() const;
    float getTouchMovedRadian() const;
    virtual bool isSlideAction() const;
    virtual bool isClickAction() const;
    
    M_SYNTHESIZE_BOOL(MoveEnabled);
    M_SYNTHESIZE_BOOL(InWin);

    void setActionCallback(const function<void(int actionIndex)>& callback);

protected:
    virtual int touchActionIndex() const;
    virtual void onEnter();
    virtual void onExit();
    void bufferWindowEffect(float fDt);
    //void adjustWinPos(Point& roPos);

    virtual void onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event *event);
    virtual void onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event *event);

protected:
    int m_iPendingAbilityOwner;
    bool m_bIsTouching;
    float m_fMoveK;
    float m_fHorBuffRange;
    float m_fVerBuffRange;
    float m_fEdgeK;
    float m_fMoveDelta;
    Point m_oMoveStart;
    float m_fTouchMovedDuration;
    float m_fMoveR;
    bool m_bCanMove;

    Point m_oLast;
    float m_fStartScale;
    float m_fStartDis;
    function<void(int actionIndex)> m_actionCallback;
};

class TouchSprite : public Sprite
{
public:
    TouchSprite();
    M_CREATE_INITWITH_FUNC_PARAM(File, TouchSprite, (const char *pszFilename), pszFilename);
    M_CREATEWITH_FUNC_PARAM(SpriteFrame, TouchSprite, (SpriteFrame *pSpriteFrame), pSpriteFrame);
    M_CREATEWITH_FUNC_PARAM(Texture, TouchSprite, (Texture2D *pTexture), pTexture);
    M_CREATEWITH_FUNC_PARAM(Texture, TouchSprite, (Texture2D *pTexture, const Rect& rect), pTexture, rect);
    static TouchSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    
    virtual TouchSprite* clone() const;
    
    virtual void onEnter() override;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    bool containsTouchLocation(Touch* touch);
    
protected:
    enum TOUCH_SPRITE_STATE 
    {
        kStateUngrabbed,
        kStateGrabbed
    }; 
    TOUCH_SPRITE_STATE m_state;
};

class TouchMaskLayer : public LayerColor
{
public:
    M_CREATE_FUNC_PARAM(TouchMaskLayer, ());
    
    M_CREATE_INITWITH_FUNC_PARAM(Color, TouchMaskLayer, (const Color4B& color, GLubyte disOpacity), color, disOpacity);
    virtual bool initWithColor(const Color4B& color, GLubyte disOpacity);
    
    void setMaskEnabled(bool enabled);

protected:
    GLubyte m_disOpacity;
};

class Effect : public Sprite
{
protected:
    const int CONST_ACT_TAG;

public:
    Effect();
    virtual ~Effect();

    virtual bool initWithPath(const char* path, float delay);
    M_CREATE_INITWITH_FUNC_PARAM(Path, Effect, (const char* path, float delay), path, delay);

    void play(int index = 0, float speed = 1.0f, int times = 1.0, FiniteTimeAction* done = NULL);
    void playRelease(int index = 0, float speed = 1.0f, int times = 1.0);
    void playForever(int index = 0, float speed = 1.0f);

    Animation* addAnimation(const char* path, float delay);
    void stop();

    typedef vector<string> VEC_ANIS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_ANIS, m_vecAnis, AnimationNames);
};

class MenuEx : public Menu
{
public:
    static MenuEx* create();
    static MenuEx* create(MenuItem* item, ...) CC_REQUIRES_NULL_TERMINATION;
    static MenuEx* createWithArray(const Vector<MenuItem*>& arrayOfItems);
    static MenuEx* createWithItem(MenuItem* item);
    static MenuEx* createWithItems(MenuItem *firstItem, va_list args);

    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    virtual void onTouchCancelled(Touch *touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);

protected:
    MenuItem* getItemForTouchEx(Touch* touch);
};

class ButtonBase : public MenuItemImage
{
protected:
    ButtonBase();
    virtual ~ButtonBase();

public:
    virtual bool initWithFile(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished);
    virtual bool initWithFrameName(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished);
    virtual bool initWithSprite(Node* pNormalSprite, Node* pSelectedSprite, Node* pDisabledSprite, Node* pBlinkSprite, Node* pMaskSprite, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished);
    //CREATE_FUNC_PARAM(ButtonBase, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, onClick, onFinished);

    virtual void activate() override;
    virtual void unselected() override;

    virtual void setCoolDown(float fDuration) = 0;
    virtual float getCoolDown() const = 0;
    void setLabel(const char* pLabel);
    M_SYNTHESIZE(int, m_iClickRetCode, ClickRetCode);

    M_SYNTHESIZE(int, m_iButtonIndex, ButtonIndex);

    virtual void coolDown(float fFromPercent = 0);
    float getPercentage() const;

private:
    void onClick(Ref* pObject);
    void onCoolDownDone(Node* pNode);
    void onCDBlickDone(Node* pNode);

protected:
    ccMenuCallback m_onClick;
    ccMenuCallback m_onFinished;
    ProgressTimer* m_pPt;
    FiniteTimeAction* m_pAct;
    Node* m_pBlink;
    Node* m_pMask;

};

class ButtonNormal : public ButtonBase
{
public:
    ButtonNormal();

    M_CREATEWITH_FUNC_PARAM(File, ButtonNormal, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, onClick, onFinished);
    M_CREATEWITH_FUNC_PARAM(FrameName, ButtonNormal, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, onClick, onFinished);
    M_CREATE_INITWITH_FUNC_PARAM(Sprite, ButtonNormal, (Node* pNormalSprite, Node* pSelectedSprite, Node* pDisabledSprite, Node* pBlinkSprite, Node* pMaskSprite, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished), pNormalSprite, pSelectedSprite, pDisabledSprite, pBlinkSprite, pMaskSprite, fCoolDown, onClick, onFinished);
    
    CC_SYNTHESIZE(float, m_fCoolDown, CoolDown);
};

class CommmButton : public ButtonNormal
{
public:
    virtual bool init (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished, int iKey);
    M_CREATEWITH_FUNC_PARAM(File, ButtonNormal, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, onClick, onFinished);
    M_CREATEWITH_FUNC_PARAM(FrameName, ButtonNormal, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, onClick, onFinished);
    M_CREATE_INITWITH_FUNC_PARAM(Sprite, ButtonNormal, (Node* pNormalSprite, Node* pSelectedSprite, Node* pDisabledSprite, Node* pBlinkSprite, Node* pMaskSprite, float fCoolDown, const ccMenuCallback& onClick, const ccMenuCallback& onFinished), pNormalSprite, pSelectedSprite, pDisabledSprite, pBlinkSprite, pMaskSprite, fCoolDown, onClick, onFinished);

public:
    int m_iKey;

};

class ButtonPanel : public Node
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
        ACTION_NODE(ButtonBase* pBtn, int iIndex): eAct(kAdd) { stAdd.pBtn = pBtn; stAdd.iIndex = iIndex; }
        ACTION_NODE(int iIndex, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor, bool bClearUp): eAct(kDel) { stDel.pBtn = nullptr; stDel.iIndex = iIndex; stDel.eVer = eVer; stDel.eHor = eHor; stDel.bClearUp = bClearUp; }
        ACTION_NODE(int iIndexSrc, int iIndexDst): eAct(kMove) { stMove.pBtn = nullptr; stMove.iIndexSrc = iIndexSrc; stMove.iIndexDst = iIndexDst; }
        ACTION_NODE(ButtonBase* pBtn, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor): eAct(kAddEx) { stAddEx.pBtn = pBtn; stAddEx.eVer = eVer; stAddEx.eHor = eHor; }
        ACTION_NODE(ADD_VERTICAL eVer, ADD_HORIZONTAL eHor): eAct(kClearUp) { stClearUp.eVer = eVer; stClearUp.eHor = eHor; }
        //~ACTION_NODE() { if (eAct == kAdd) {CC_SAFE_RELEASE(stAdd.pBtn); } else if (eAct == kAddEx) {CCLOG("release");CC_SAFE_RELEASE(stAddEx.pBtn);} }

        ACTION_CMD eAct;
        union
        {
            struct // ADD
            {
                ButtonBase* pBtn;
                int iIndex;
            } stAdd;

            struct // DEL
            {
                ButtonBase* pBtn;
                int iIndex;
                ADD_VERTICAL eVer;
                ADD_HORIZONTAL eHor;
                bool bClearUp;
            } stDel;

            struct // MOVE
            {
                ButtonBase* pBtn;
                int iIndexSrc;
                int iIndexDst;
            } stMove;

            struct // ADDEX
            {
                ButtonBase* pBtn;
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
    ButtonPanel();
    virtual ~ButtonPanel();

    virtual bool init(int iRow, int iColumn, const Size& rButtonSize, float fHorBorderWidth, float fVerBorderWidth, Node* pBackground, float fBackgroundOffsetX = 0.0f, float fBackgroundOffsetY = 0.0f);
    M_CREATE_FUNC_PARAM(ButtonPanel, (int iRow, int iColumn, const Size& rButtonSize, float fHorBorderWidth, float fVerBorderWidth, Node* pBackground, float fBackgroundOffsetX = 0.0f, float fBackgroundOffsetY = 0.0f), iRow, iColumn, rButtonSize, fHorBorderWidth, fVerBorderWidth, pBackground, fBackgroundOffsetX, fBackgroundOffsetY);

    void addButton(ButtonBase* pButton, int iIndex); // org
    void addButton(ButtonBase* pButton, int iX, int iY);

    void delButton(int iIndex); // org
    void delButton(int iX, int iY);
    void delButton(ButtonBase* pButton);

    void moveButton(int iIndexSrc, int iIndexDst); // org

    void addButtonEx(ButtonBase* pButton, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    int allotSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);
    int allotSlot(int iStartX, int iStartY, int iEndX, int iEndY, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor);
    void clearUpSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    void retainButton(ButtonBase* pButton);
    ButtonBase* getRetainButton() const;

    ButtonBase* getButton(int iX, int iY) const;
    ButtonBase* getButton(int iIndex) const;
    ButtonBase* getButton(const function<bool(ButtonBase*)>& match) const;

    int getButtonIndex(ButtonBase* pButton) const;

    int index2Y(int iIndex) const;
    int index2X(int iIndex) const;
    int toIndex(int iX, int iY) const;
    Point index2Point(int iIndex);

    bool isFull();

    void pushAction(const ACTION_NODE& roAct);
    void onPrevActEnd(Node* pNode);

    void pushAddButtonAction(ButtonBase* pButton, int iIndex);
    void pushDelButtonAction(int iIndex, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight, bool bClearUp = true);
    void pushMoveButtonAction(int iIndexSrc, int iIndexDst);
    void pushAddButtonExAction(ButtonBase* pButton, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);
    void pushClearUpSlotAction(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    int getMaxCount() const;
    int getCount() const;

public:
    CC_SYNTHESIZE(int, m_iRow, Row);
    CC_SYNTHESIZE(int, m_iColumn, Column);
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(Size, m_oButtonSize, ButtonSize);
    CC_SYNTHESIZE(float, m_fHorBorderWidth, HorBorderWidth);
    CC_SYNTHESIZE(float, m_fVerBorderWidth, VerBorderWidth);
    CC_SYNTHESIZE(MenuEx*, m_pInnerMenu, InnerMenu);
    CC_SYNTHESIZE_READONLY(Node*, m_pBackground, Background);
    void setBackground(Node* pBackground, float fBackgroundOffsetX, float fBackgroundOffsetY);

public:
    int m_iOwnerKey;
    ButtonBase** m_ppBtnPos;
    ButtonBase* m_pRetain;
    LIST_ACTION m_lstActs;
    int m_iCount;
};

class PopPanel : public Node
{
public:
    PopPanel();

    virtual bool initWithSize(const Size& size);
    M_CREATE_INITWITH_FUNC_PARAM(Size, PopPanel, (const Size& size), size);
    virtual bool initWithBackground(Sprite* background);
    M_CREATE_INITWITH_FUNC_PARAM(Background, PopPanel, (Sprite* background), background);

    void setBackground(Sprite* background, bool tile);
    M_SYNTHESIZE(MenuEx*, m_mn, Menu);
    void addButton(MenuItem* mi);

    void onClickClose(Ref* obj);

    void show();
    void hide();
};

class Utils
{
public:
    typedef function<void(Color4B* c, GLushort x, GLushort y, GLushort w, GLushort h)> FUNC_TRAN;
    static Image* nodeToImage(Node* node);
    static RenderTexture* nodeToRenderTexture(Node* node);
    static Image* transformImage(Image* image, const FUNC_TRAN& funcTransform);
    static Texture2D* nodeToTexture(Node* node, const FUNC_TRAN& funcTransform = nullptr);
    static bool nodeToFile(Node* node, const char* file, const FUNC_TRAN& funcTransform = nullptr);
    static void render();

#define M_TRAN_COLOR_BIND(func, ...) std::bind(func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, ##__VA_ARGS__)
    static void tranGrayscale(Color4B* c, GLushort x, GLushort y, GLushort w, GLushort h);
    static void tranFillColor(Color4B* c, GLushort x, GLushort y, GLushort w, GLushort h, Color4B* fill);
    static void tranFillAlpha(Color4B* c, GLushort x, GLushort y, GLushort w, GLushort h, GLushort a);
};

#include "Ability.h"
class AbilityItem : public Node
{
CC_CONSTRUCTOR_ACCESS:
    AbilityItem();

public:
    bool initWithAbility(CAbility* ability);
    M_CREATE_INITWITH_FUNC_PARAM(Ability, AbilityItem, (CAbility* ability), ability);

    static Color3B abilityGradeColor3B(CAbility::GRADE grade);
    //Sprite
};

class WinFormPanel : public WinLayer
{
public:
    WinFormPanel();
    virtual ~WinFormPanel();
    bool init(int iRow, int iColumn, int iWinRow, int iWinColumn, const Size& rNodeSize, float fHorInnerBorderWidth, float fVerInnerBorderWidth, float fHorBorderWidth = 0.0f, float fVerBorderWidth = 0.0f, Node* pBackground = nullptr, float fBackgroundOffsetX = 0.0f, float fBackgroundOffsetY = 0.0f);
    M_CREATE_FUNC_PARAM(WinFormPanel, (int iRow, int iColumn, int iWinRow, int iWinColumn, const Size& rNodeSize, float fHorInnerBorderWidth, float fVerInnerBorderWidth, float fHorBorderWidth = 0.0f, float fVerBorderWidth = 0.0f, Node* pBackground = nullptr, float fBackgroundOffsetX = 0.0f, float fBackgroundOffsetY = 0.0f), iRow, iColumn, iWinRow, iWinColumn, rNodeSize, fHorInnerBorderWidth, fVerInnerBorderWidth, fHorBorderWidth, fVerBorderWidth, pBackground, fBackgroundOffsetX, fBackgroundOffsetY);

    void addNode(Node* pNode, int iIndex); // org
    void addNode(Node* pNode, int iX, int iY);

    void delNode(int iIndex); // org
    void delNode(int iX, int iY);
    void delNode(Node* pNode);

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

    void moveNode(int iIndexSrc, int iIndexDst); // org

    void addNodeEx(Node* pNode, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    int allotSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);
    int allotSlot(int iStartX, int iStartY, int iEndX, int iEndY, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor);
    void clearUpSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    Node* getNode(int iX, int iY) const;
    Node* getNode(int iIndex) const;
    Node* getNode(const function<bool(Node*)>& match) const;

    int getNodeIndex(Node* pNode) const;

    int index2Y(int iIndex) const;
    int index2X(int iIndex) const;
    int toIndex(int iX, int iY) const;
    Point index2Point(int iIndex);

    M_SYNTHESIZE(int, m_iRow, Row);
    M_SYNTHESIZE(int, m_iColumn, Column);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(Size, m_oNodeSize, NodeSize);
    M_SYNTHESIZE(float, m_fHorInnerBorderWidth, HorInnerBorderWidth);
    M_SYNTHESIZE(float, m_fVerInnerBorderWidth, VerInnerBorderWidth);
    M_SYNTHESIZE_READONLY(Node*, m_pBackground, Background);
    void setBackground(Node* pBackground, float fBackgroundOffsetX, float fBackgroundOffsetY);

    M_SYNTHESIZE_READONLY(int, m_iCount, Count);

    M_SYNTHESIZE(float, m_fHorBorderWidth, HorBorderWidth);
    M_SYNTHESIZE(float, m_fVerBorderWidth, VerBorderWidth);

protected:
    Node** m_ppNodes;
};






////////////////////////////////// Inline ////////////////////////////////////////
// GameFile
template <typename TYPE>
ssize_t GameFile::read(TYPE* pData, ssize_t uCount /*= 1*/)
{
    ssize_t uRead;
    for (uRead = 0; uRead < uCount && m_uPos + (uRead + 1) * (ssize_t)sizeof(TYPE) <= m_data.getSize(); ++uRead);
    ssize_t uReadSize = uRead * sizeof(TYPE);
    m_data.copy((uint8_t*)pData, uReadSize);
    m_uPos += uReadSize;

    return uRead;
}



#endif  /* __COMPONENTFORCC_H__ */
