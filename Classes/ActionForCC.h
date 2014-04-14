#ifndef __ACTIONFORCC_H__
#define	__ACTIONFORCC_H__


class CCallFuncData;

class CCCallFuncNMultiObj : public CCCallFuncND
{
public:
    virtual long getClassTypeInfo() {
        static const long id = cocos2d::getHashCodeByString(typeid(cocos2d::CCCallFunc).name());
		return id;
    }

    virtual ~CCCallFuncNMultiObj();

    /** initializes the action with the callback and the data to pass as an argument */
    virtual bool initWithTarget(CCObject* pSelectorTarget, SEL_CallFuncND selector, CCallFuncData* d);
    M_CREATE_INITWITH_FUNC_PARAM(Target, CCCallFuncNMultiObj, (CCObject* pSelectorTarget, SEL_CallFuncND selector, CCallFuncData* d), pSelectorTarget, selector, d);

    // super methods
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CCObject* copyWithZone(CCZone *pZone);
};

class CCNotifyAnimate : public CCAnimate
{
public:
    CCNotifyAnimate();
    virtual ~CCNotifyAnimate();

    virtual bool initWithAnimation(CCAnimation* pAnimation, int iNotifyFrameIndex, CCObject* pSelector, SEL_CallFuncND pCallback, CCallFuncData* pData, const char* pSound = NULL);
    M_CREATE_INITWITH_FUNC_PARAM(Animation, CCNotifyAnimate, (CCAnimation *pAnimation, int iNotifyFrameIndex, CCObject* pSelector, SEL_CallFuncND pCallback, CCallFuncData* pData, const char* pSound = NULL), pAnimation, iNotifyFrameIndex, pSelector, pCallback, pData, pSound);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void update(float t);

protected:
    int m_iNotifyFrameIndex;
    CCObject* m_pSelector;
    SEL_CallFuncND m_pCallback;
    CCallFuncData* m_pData;
    bool m_bNotified;
    string m_sSound;
};

class CCFadeInOutScale4 : public CCSequence
{
public:
    virtual bool init(float fScaleStart, float fScaleMid, float fScaleNormal, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd);
    M_CREATE_FUNC_PARAM(CCFadeInOutScale4, (float fScaleStart, float fScaleMid, float fScaleNormal, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd), fScaleStart, fScaleMid, fScaleNormal, fScaleEnd, fDurToMid, fDurToNormal, fDurKeep, fDurToEnd);

    virtual void startWithTarget(CCNode *pTarget);

protected:
    float m_fScaleStart;
};

class CCSequenceEx : public CCSequence
{
public:
    static CCSequenceEx* createWithTwoActions(CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo);
    virtual CCFiniteTimeAction* getActionOne();
    virtual CCFiniteTimeAction* getActionTwo();
};

class CUnitDrawForCC;
class CProjectileForCC;

class CCMoveToNode : public CCActionInterval
{
public:
    enum NODE_TYPE
    {
        kNormal,
        kDraw,
        kProjectile
    };

public:
    CCMoveToNode();
    virtual ~CCMoveToNode();

    virtual bool initWithNode(float fDuration, CCNode* pToNode, bool bFixRotation = true, float fMaxHeightDelta = 0.0f);
    M_CREATEWITH_FUNC_PARAM(Node, CCMoveToNode, (float fDuration, CCNode* pToNode, bool bFixRotation, float fMaxHeightDelta), fDuration, pToNode, bFixRotation, fMaxHeightDelta);

    virtual bool initWithDraw(float fDuration, CUnitDrawForCC* pToDraw, bool bFixRotation = true, float fMaxHeightDelta = 0.0f);
    M_CREATEWITH_FUNC_PARAM(Draw, CCMoveToNode, (float fDuration, CUnitDrawForCC* pToDraw, bool bFixRotation, float fMaxHeightDelta), fDuration, pToDraw, bFixRotation, fMaxHeightDelta);

    virtual bool initWithProjectile(float fDuration, CProjectileForCC* pToProjectile, bool bFixRotation = true, float fMaxHeightDelta = 0.0f);
    M_CREATEWITH_FUNC_PARAM(Projectile, CCMoveToNode, (float fDuration, CProjectileForCC* pToProjectile, bool bFixRotation, float fMaxHeightDelta), fDuration, pToProjectile, bFixRotation, fMaxHeightDelta);

    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    virtual CCObject* copyWithZone(CCZone* pZone);

protected:
    CCPoint m_oStartPos;
    CCPoint m_oEndPos;
    CCPoint m_oDeltaPos;
    union
    {
        CCNode * m_pToNode;
        CUnitDrawForCC* m_pToDraw;
        CProjectileForCC* m_pToProjectile;
    };
    
    NODE_TYPE m_eFromType;
    NODE_TYPE m_eToType;
    float m_fMinSpeed;
    float m_fFromHeight;
    float m_fToHeight;
    float m_fDeltaHeight;
    float m_fMaxHeightDelta;
    float m_fA;
    bool m_bFixRotation;
};

class CCLink : public CCNotifyAnimate
{
public:
    CCLink();
    virtual ~CCLink();
    virtual bool initWithDrawToDraw(CCAnimation* pAnimation, int iNotifyFrameIndex, CCObject* pSelector, SEL_CallFuncND pCallback, CCallFuncData* pData, CUnitDrawForCC* pFromDraw, CUnitDrawForCC* pToDraw);
    M_CREATEWITH_FUNC_PARAM(DrawToDraw, CCLink, (CCAnimation* pAnimation, int iNotifyFrameIndex, CCObject* pSelector, SEL_CallFuncND pCallback, CCallFuncData* pData, CUnitDrawForCC* pFromDraw, CUnitDrawForCC* pToDraw), pAnimation, iNotifyFrameIndex, pSelector, pCallback, pData, pFromDraw, pToDraw);
    
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float t);

    void fixTargetPosition(CCNode* pTarget);

protected:
    enum NODE_TYPE
    {
        kNormal,
        kDraw,
        kProjectile,
        kPoint
    };

    NODE_TYPE m_eFromType;
    NODE_TYPE m_eToType;
    NODE_TYPE m_eTargetType;

    union
    {
        CCNode* m_pFromNode;
        CUnitDrawForCC* m_pFromDraw;
        CProjectileForCC* m_pFromProjectile;
    };
    CCPoint m_oFromPoint;

    union
    {
        CCNode* m_pToNode;
        CUnitDrawForCC* m_pToDraw;
        CProjectileForCC* m_pToProjectile;
    };
    CCPoint m_oToPoint;

    bool m_bFireFrom;
};

class CCShake : public CCRepeat
{
public:
    CCShake();
    virtual bool init(float fDuration, int iTimes, float fRange);
    M_CREATE_FUNC_PARAM(CCShake, (float fDuration, int iTimes, float fRange), fDuration, iTimes, fRange);

    virtual void startWithTarget(CCNode* pTarget);
    virtual void stop(void);


public:
    CCPoint m_oLoc;
};

#endif  /* __ACTIONFORCC_H__ */
