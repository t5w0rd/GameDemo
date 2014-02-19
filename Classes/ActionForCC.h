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

