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
    virtual CCFiniteTimeAction* setPercentageAction(float fPercent, float fDuration, CCFiniteTimeAction* pEndAction = NULL);
    virtual void setFillColor(const ccColor3B& roColor);

public:
    CCProgressTimer* m_pPt;
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
public:
    CCEffect();
    virtual ~CCEffect();

    virtual bool initWithPath(const char* path, float delay);
    M_CREATE_INITWITH_FUNC_PARAM(Path, CCEffect, (const char* path, float delay), path, delay);

    void play(float speed = 1.0f, int times = 1.0);
    void playRelease(float speed = 1.0f, int times = 1.0);
    void playForever(float speed = 1.0f);

    M_SYNTHESIZE(CCAnimation*, m_pAni, Animation);

protected:
    const int CONST_ACT_TAG;
};


#endif  /* __COMPONENTFORCC_H__ */
