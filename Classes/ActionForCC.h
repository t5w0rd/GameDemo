#pragma once


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
    virtual ~CCNotifyAnimate();

    virtual bool initWithAnimation(CCAnimation* pAnimation, int iNotifyFrameIndex, CCObject* pSelector, SEL_CallFuncND pCallback, CCallFuncData* pData);
    M_CREATE_INITWITH_FUNC_PARAM(Animation, CCNotifyAnimate, (CCAnimation *pAnimation, int iNotifyFrameIndex, CCObject* pSelector, SEL_CallFuncND pCallback, CCallFuncData* pData), pAnimation, iNotifyFrameIndex, pSelector, pCallback, pData);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void update(float t);

protected:
    int m_iNotifyFrameIndex;
    CCObject* m_pSelector;
    SEL_CallFuncND m_pCallback;
    CCallFuncData* m_pData;
};

class CCFadeInOutScale4 : public CCSequence
{
public:
    virtual bool init(float fScaleStart, float fScaleMid, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd);
    M_CREATE_FUNC_PARAM(CCFadeInOutScale4, (float fScaleStart, float fScaleMid, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd), fScaleStart, fScaleMid, fScaleEnd, fDurToMid, fDurToNormal, fDurKeep, fDurToEnd);

    virtual void startWithTarget(CCNode *pTarget);

protected:
    float m_fScaleStart;
};

class CUnitDrawForCC;

class CCMoveToNode : public CCActionInterval
{
public:
    enum NODE_TYPE
    {
        kNormal,
        kUnit,
        kProjectile
    };

public:
    CCMoveToNode();
    virtual ~CCMoveToNode();
    virtual bool init(float fDuration, CCNode* pToNode, bool bFixRotation, float fMaxHeightDelta = 0.0f);
    M_CREATE_FUNC_PARAM(CCMoveToNode, (float fDuration, CCNode* pToNode, bool bFixRotation, float fMaxHeightDelta), fDuration, pToNode, bFixRotation, fMaxHeightDelta);
    virtual void startWithTarget(CCNode *pTarget);

    virtual void update(float time);
    virtual CCObject* copyWithZone(CCZone* pZone);

protected:
    CCPoint m_oStartPos;
    CCPoint m_oEndPos;
    CCPoint m_oDeltaPos;
    CCNode* m_pToNode;
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
