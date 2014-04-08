#ifndef __DRAWFORCC_H__
#define __DRAWFORCC_H__

#include "Draw.h"
#include "ComponentForCC.h"


class CUnitDrawForCC;

class CCActionSprite : public CCSprite
{
public:
    virtual void onMoveToDone(CCNode*, void* pCallFuncData);
    virtual void onNotifyFrame(CCNode*, void* pCallFuncData);
    virtual void onAnimationDone(CCNode*, void* pCallFuncData);
};

class CCUnitSprite : public CCActionSprite
{
public:
    CCUnitSprite();
    virtual ~CCUnitSprite();

    virtual bool initWithDraw(CUnitDrawForCC* pDraw);
    M_CREATEWITH_FUNC_PARAM(Draw, CCUnitSprite, (CUnitDrawForCC* pDraw), pDraw);

    M_SYNTHESIZE(CUnitDrawForCC*, m_pDraw, Draw);

    M_SYNTHESIZE(CCNode*, m_pShadow, Shadow);
    CCNode* createShadow();
    virtual void setPosition(const CCPoint& roPos);
    virtual void draw();

};

class CUnitDrawForCC : public CUnitDraw2D
{
public:
    CUnitDrawForCC(const char* pName);
    virtual ~CUnitDrawForCC();
    virtual CMultiRefObject* copy();
    virtual void copyData(CUnitDraw* from);

    // ·µ»ØactionTag
    virtual int doMoveTo(const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone, float fSpeed = 1.0f);
    virtual void updateMoveTo(const CPoint& rPos);
    virtual int doAnimation(ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone, float fSpeed = 1.0f);
    virtual void stopAction(int tag);
    virtual void setActionSpeed(int tag, float fSpeed);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();
    virtual CCAction* getAction(int id);

    virtual void setVisible(bool bVisible = true);

    virtual void setFrame(FRM_ID id);

    virtual void setFlipX(bool bFlipX = true);
    virtual bool isFlipX() const;
    
    struct ANI_INFO
    {
        int iNotifyFrameIndex;
        CCAnimation* pAni;
        vector<string> vecSounds;
    };

    typedef map<ANI_ID, ANI_INFO> MAP_ANI_INFOS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ANI_INFOS, m_mapAniInfos, AnimationInfos);
    
    void prepareAnimation(ANI_ID id, const char* pName, int iNotifyFrameIndex, const char* pSound = NULL);

    ANI_INFO* getAnimationInfo(ANI_ID id);
    void addAnimationSound(ANI_ID id, const char* sound);

    struct FRM_INFO
    {
        CCSpriteFrame* pSf;
    };
    
    typedef map<FRM_ID, FRM_INFO> MAP_FRM_INFOS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_FRM_INFOS, m_mapFrmInfos, FrameInfos);

    void prepareFrame(FRM_ID id, const char* pName);

    FRM_INFO* getFrameInfo(FRM_ID id);

    M_SYNTHESIZE_PASS_BY_REF(CCPoint, m_oAnchorPoint, AnchorPoint);
    M_SYNTHESIZE_READONLY(CCUnitSprite*, m_pSprite, Sprite);
    CCUnitSprite* createSprite();

    virtual void setPosition(const CPoint& p);
    virtual CPoint& getPosition();
    virtual void setHeight(float fHeight);

    void setGeometry(float fHalfOfWidth, float fHalfOfHeight, const CCPoint& anchorPoint, const CPoint& firePoint);

    M_SYNTHESIZE(int, m_iMaxTips, MaxTips);
    M_SYNTHESIZE(int, m_iBaseTipId, BaseTipId);
    M_SYNTHESIZE(int, m_iCurTipId, CurTipId);
    void addBattleTip(const char* pTip, const char* pFont, float fFontSize, ccColor3B color);

};

class CWorldForCC;

class CCUnitLayer : public CCWinLayer
{
public:
    CCUnitLayer();
    virtual bool initWithWorld(CWorldForCC* pWorld);
    M_CREATEWITH_FUNC_PARAM(World, CCUnitLayer, (CWorldForCC* pWorld), pWorld);

    M_SYNTHESIZE(CWorldForCC*, m_pWorld, World);

    virtual void onEnter();
    virtual void onExit();

    M_SYNTHESIZE_READONLY(float, m_fWorldInterval, WorldInterval);
    virtual void setWorldInterval(float fInterval);

    virtual void onWorldInterval(float dt);
};

class CWorldForCC : public CWorld
{
public:
    CWorldForCC();
    virtual ~CWorldForCC();

    virtual void onAddUnit(CUnit* pUnit);
    virtual void onDelUnit(CUnit* pUnit);
    virtual void onAddProjectile(CProjectile* pProjectile);
    virtual void onDelProjectile(CProjectile* pProjectile);

    M_SYNTHESIZE_READONLY(CCUnitLayer*, m_pLayer, Layer);
    virtual void setLayer(CCUnitLayer* pLayer);
    CCLayer* createLayer();

    virtual void shutdown();
};

class CUnitPathForCC : public CUnitPath
{
public:
    CUnitPathForCC(const char* pFileName);

    void addPoints(const char* pFileName);
    void saveAsFile(const char* pFileName);
};

class CProjectileForCC;

class CCProjectileSprite : public CCActionSprite
{
public:
    CCProjectileSprite();

    virtual bool initWithProjectile(CProjectileForCC* pProjectile);
    M_CREATEWITH_FUNC_PARAM(Projectile, CCProjectileSprite, (CProjectileForCC* pProjectile), pProjectile);

    M_SYNTHESIZE(CProjectileForCC*, m_pProjectile, Projectile);

    virtual void setPosition(const CCPoint& roPos);
};

class CProjectileForCC : public CProjectile
{
public:
    CProjectileForCC(const char* pName);
    virtual ~CProjectileForCC();
    virtual CMultiRefObject* copy();
    virtual void copyData(const CProjectile* from);

    virtual void setPosition(const CPoint& p);
    virtual CPoint& getPosition();
    virtual void setHeight(float fHeight);

    virtual int doLinkUnitToUnit(CUnit* pFromUnit, CUnit* pToUnit, ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone);
    virtual int doMoveToUnit(CUnit* pToUnit, bool bFixRotation, float fMaxHeightDelta, float fDuration, CCallFuncData* pOnMoveToDone);
    virtual int doMoveTo(const CPoint& rPos, float fDuration, CCallFuncData* pOnMoveToDone);
    virtual int doAnimation(ANI_ID id, CCallFuncData* pOnNotifyFrame, int iRepeatTimes, CCallFuncData* pOnAnimationDone);
    virtual void stopAction(int tag);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();

    virtual void setVisible(bool bVisible = true);

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

    M_SYNTHESIZE_READONLY_PASS_BY_REF(FRM_INFO, m_stFrmInfo, FrameInfo);

    void prepareFrame(FRM_ID id, const char* pName);

    FRM_INFO* getFrameInfo(FRM_ID id);

    M_SYNTHESIZE_PASS_BY_REF(CCPoint, m_oAnchorPoint, AnchorPoint);
    M_SYNTHESIZE_READONLY(CCProjectileSprite*, m_pSprite, Sprite);
    CCProjectileSprite* createSprite();

    virtual void playFireSound();
    virtual void playEffectSound();
    
};


#endif  /* __DRAWFORCC_H__ */
