#pragma once

#include "Logic/Draw.h"


class CUnitDrawForCC;

class CCUnitSprite : public CCSprite
{
public:
    CCUnitSprite();
    virtual ~CCUnitSprite();

    virtual bool initWithDraw(CUnitDrawForCC* pDraw);
    M_CREATEWITH_FUNC_PARAM(Draw, CCUnitSprite, (CUnitDrawForCC* pDraw), pDraw);

    M_SYNTHESIZE(CUnitDrawForCC*, m_pDraw, Draw);

    void onMoveToDone(CCNode*, void* pCallFuncData);
    void onNotifyFrame(CCNode*, void* pCallFuncData);
    void onAnimationDone(CCNode*, void* pCallFuncData);

};

class CUnitDrawForCC : public CUnitDraw2D
{
public:
    CUnitDrawForCC(const char* pName);
    virtual ~CUnitDrawForCC();

    // ·µ»ØactionTag
    virtual int doMoveTo(const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone, float fSpeed = 1.0f);
    virtual int doAnimation(ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone, float fSpeed = 1.0f);
    virtual void stopAction(int tag);
    virtual void setActionSpeed(int tag, float fSpeed);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();
    virtual CCAction* getAction(int id);

    virtual void setFrame(FRM_ID id);

    virtual void setFlipX(bool bFlipX = true);
    virtual bool isFlipX() const;
    
    struct ANI_INFO
    {
        int iNotifyFrameIndex;
        CCAnimation* pAni;
    };

    typedef map<ANI_ID, ANI_INFO> MAP_ANI_INFOS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ANI_INFOS, m_mapAniInfos, AnimationInfos);
    
    void prepareAnimation(ANI_ID id, const char* pName, int iNotifyFrameIndex);

    ANI_INFO* getAnimationInfo(ANI_ID id);

    struct FRM_INFO
    {
        CCSpriteFrame* pSf;
    };
    
    typedef map<FRM_ID, FRM_INFO> MAP_FRM_INFOS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_FRM_INFOS, m_mapFrmInfos, FrameInfos);

    void prepareFrame(FRM_ID id, const char* pName);

    FRM_INFO* getFrameInfo(FRM_ID id);

    M_SYNTHESIZE_READONLY(CCSprite*, m_pSprite, Sprite);
    CCSprite* createSprite();

    M_SYNTHESIZE_PASS_BY_REF(CCPoint, m_oAnchorPoint, AnchorPoint);
    M_SYNTHESIZE_PASS_BY_REF(CCPoint, m_oFirePoint, FirePoint);

    virtual void setPosition(const CPoint& p);
    virtual CPoint& getPosition();

    void setSpriteProperty(float fHalfOfWidth, float fHalfOfHeight, const CCPoint& anchorPoint, const CCPoint& firePoint);

};

class CUnitWorldForCC;

class CCUnitLayer : public CCLayer
{
public:
    CCUnitLayer();
    virtual bool initWithWorld(CUnitWorldForCC* pWorld);
    M_CREATEWITH_FUNC_PARAM(World, CCUnitLayer, (CUnitWorldForCC* pWorld), pWorld);

    M_SYNTHESIZE(CUnitWorldForCC*, m_pWorld, World);

    virtual void onEnter();
    virtual void onExit();

    M_SYNTHESIZE_READONLY(float, m_fWorldInterval, WorldInterval);
    virtual void setWorldInterval(float fInterval);

    virtual void onWorldInterval(float dt);
};

class CUnitWorldForCC : public CWorld
{
public:
    CUnitWorldForCC();
    virtual ~CUnitWorldForCC();

    virtual void onAddUnit(CUnit* pUnit);
    virtual void onDelUnit(CUnit* pUnit);

    M_SYNTHESIZE_READONLY(CCUnitLayer*, m_pLayer, Layer);
    virtual void setLayer(CCUnitLayer* pLayer);
    CCLayer* createLayer();
};


class CCWinUnitLayer : public CCUnitLayer
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
    CCWinUnitLayer();
    virtual bool init();
    CREATE_FUNC(CCWinUnitLayer);

    // default implements are used to call script callback if exist
    virtual void setBackGroundSprite(CCSprite* pSprite);
    virtual void setBackGroundSprite(CCSprite* pSprite, int zOrder, int tag);
    virtual void setBufferEffectParam(float fMoveK, float fBuffRange, float fEdgeK);
    virtual float getTouchMovedDuration() const;
    virtual float getTouchMovedDistance() const;
    virtual float getTouchMovedRadian() const;
    virtual bool isSlideAction() const;
    virtual bool isClickAction() const;
    virtual int touchActionIndex() const;
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    virtual void bufferWindowEffect(float fDt);

    void adjustWinPos(CCPoint& roPos);

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
};
