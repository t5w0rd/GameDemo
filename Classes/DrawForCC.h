#ifndef __DRAWFORCC_H__
#define __DRAWFORCC_H__

#include "Draw.h"
#include "ComponentForCC.h"


class ActionSprite : public Sprite
{
public:
    virtual void onMoveToDone(Node*, const FUNC_VOID& callFunc);
    virtual void onNotifyFrame(Node*, const FUNC_VOID& callFunc);
    virtual void onAnimationDone(Node*, const FUNC_VOID& callFunc);
};

class CSpriteInfo : public CMultiRefObject
{
public:
    CSpriteInfo(const char* pName);

    M_SYNTHESIZE_STR(Name);

    enum ANI_ID
    {
        kAniMove,
        kAniDie,
        kAniAct1,
        kAniAct2,
        kAniAct3,
        kAniAct4,
        kAniAct5,
        kAniAct6,
        kAniAct7,
        kAniAct8
    };

    struct ANI_INFO
    {
        int iNotifyFrameIndex;
        Animation* pAni;
        vector<string> vecSounds;
    };

    typedef map<int, ANI_INFO> MAP_ANI_INFOS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_ANI_INFOS, m_mapAniInfos, AnimationInfos);

    void prepareAnimation(int id, const char* pName, int iNotifyFrameIndex, const char* pSound = nullptr);
    const ANI_INFO* getAnimationInfo(int id) const;
    void addAnimationSound(int id, const char* sound);

    enum FRM_ID
    {
        kFrmDefault,
        kFrmPortraitHero,
        kFrmPortraitSel
    };

    struct FRM_INFO
    {
        SpriteFrame* pSf;
    };

    typedef map<int, FRM_INFO> MAP_FRM_INFOS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(MAP_FRM_INFOS, m_mapFrmInfos, FrameInfos);

    void prepareFrame(int id, const char* pName, const char* pFrameName = nullptr);
    const FRM_INFO* getFrameInfo(int id) const;
};

class CUnitDrawForCC;

class UnitSprite : public ActionSprite
{
public:
    UnitSprite();
    virtual ~UnitSprite();

    virtual bool initWithDraw(CUnitDrawForCC* pDraw);
    M_CREATEWITH_FUNC_PARAM(Draw, UnitSprite, (CUnitDrawForCC* pDraw), pDraw);

    M_SYNTHESIZE(CUnitDrawForCC*, m_pDraw, Draw);

    M_SYNTHESIZE(Node*, m_pShadow, Shadow);
    Node* createShadow();
    virtual void setPosition(const Point& roPos) override;
    virtual void draw(Renderer *renderer, const kmMat4& transform, bool transformUpdated) override;

};

class CUnitDrawForCC : public CUnitDraw2D
{
public:
    CUnitDrawForCC(CSpriteInfo* si);
    virtual ~CUnitDrawForCC();
    virtual CUnitDrawForCC* copy() override;
    virtual void copyData(CUnitDraw* from);

    M_SYNTHESIZE_READONLY(CSpriteInfo*, m_si, SpriteInfo);

    // ·µ»ØactionTag
    virtual int doMoveTo(const CPoint& rPos, float fDuration, const FUNC_VOID& onMoveToDone, float fSpeed = 1.0f);
    virtual void updateMoveTo(const CPoint& rPos);
    virtual int doAnimation(int id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone, float fSpeed = 1.0f);
    virtual void stopAction(int tag);
    virtual void setActionSpeed(int tag, float fSpeed);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();
    virtual Action* getAction(int id);

    virtual void setVisible(bool bVisible = true);

    virtual void setFrame(int id);

    virtual void setFlippedX(bool bFlipX = true);
    virtual bool isFlippedX() const;
    
    M_SYNTHESIZE_PASS_BY_REF(Point, m_oAnchorPoint, AnchorPoint);
    M_SYNTHESIZE_READONLY(UnitSprite*, m_pSprite, Sprite);
    UnitSprite* createSprite();

    virtual void setPosition(const CPoint& p);
    virtual CPoint& getPosition();
    virtual void setHeight(float fHeight);

    void setGeometry(float fHalfOfWidth, float fHalfOfHeight, const Point& anchorPoint, const CPoint& firePoint);

    M_SYNTHESIZE(int, m_iMaxTips, MaxTips);
    M_SYNTHESIZE(int, m_iBaseTipId, BaseTipId);
    M_SYNTHESIZE(int, m_iCurTipId, CurTipId);
    void addBattleTip(const char* pTip, const char* pFont, float fFontSize, Color3B color);
    
    struct CTRL_SOUND
    {
        string sound;
        float duration;
    };
    typedef vector<CTRL_SOUND> VEC_CTRL_SOUNDS;
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_CTRL_SOUNDS, m_vecCtrlSounds, CtrlSounds);
    void addCtrlSound(const char* sound, float duration);
    void playRandomCtrlSound();

protected:
    int m_lastCtrlSoundPlayedId;
    int m_lastCtrlSoundPlayedIndex;

public:
    virtual void say(const char* words);
};

class CWorldForCC;

class UnitLayer : public WinLayer
{
public:
    UnitLayer();
    virtual bool initWithWorld(CWorldForCC* pWorld);
    M_CREATEWITH_FUNC_PARAM(World, UnitLayer, (CWorldForCC* pWorld), pWorld);

    M_SYNTHESIZE(CWorldForCC*, m_pWorld, World);

    virtual void onEnter() override;
    virtual void onExit() override;

    M_SYNTHESIZE_READONLY(float, m_fWorldInterval, WorldInterval);
    virtual void setWorldInterval(float fInterval);

    virtual void onWorldInterval(float dt);
};

class CWorldForCC : public CWorld
{
public:
    CWorldForCC();
    virtual ~CWorldForCC();

    virtual void onAddUnit(CUnit* pUnit) override;
    virtual void onDelUnit(CUnit* pUnit) override;
    virtual void onAddProjectile(CProjectile* pProjectile) override;
    virtual void onDelProjectile(CProjectile* pProjectile) override;
    virtual void onAddNormalAttributes(CUnit* pUnit) override;
    virtual void onDelNormalAttributes(CUnit* pUnit) override;

    M_SYNTHESIZE_READONLY(UnitLayer*, m_pLayer, Layer);
    virtual void setLayer(UnitLayer* pLayer);
    Layer* createLayer();

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

class ProjectileSprite : public ActionSprite
{
public:
    ProjectileSprite();

    virtual bool initWithProjectile(CProjectileForCC* pProjectile);
    M_CREATEWITH_FUNC_PARAM(Projectile, ProjectileSprite, (CProjectileForCC* pProjectile), pProjectile);

    M_SYNTHESIZE(CProjectileForCC*, m_pProjectile, Projectile);

    virtual void setPosition(const Point& roPos) override;
};

class CProjectileForCC : public CProjectile
{
public:
    CProjectileForCC(CSpriteInfo* si);
    virtual ~CProjectileForCC();
    virtual CProjectileForCC* copy();
    virtual void copyData(const CProjectile* from);

    M_SYNTHESIZE(CSpriteInfo*, m_si, SpriteInfo);

    virtual float getRadius() const;

    virtual void setPosition(const CPoint& p);
    virtual CPoint& getPosition();
    virtual void setHeight(float fHeight);

    virtual int doLinkUnitToUnit(CUnit* pFromUnit, CUnit* pToUnit, ANI_ID id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone);
    virtual int doMoveToUnit(CUnit* pToUnit, bool bFixRotation, float fMaxHeightDelta, float fDuration, const FUNC_VOID& onMoveToDone);
    virtual int doMoveTo(const CPoint& rPos, float fDuration, const FUNC_VOID& onMoveToDone);
    virtual int doAnimation(int id, const FUNC_VOID& onNotifyFrame, int iRepeatTimes, const FUNC_VOID& onAnimationDone);
    virtual void stopAction(int tag);
    virtual bool isDoingAction(int id);
    virtual void stopAllActions();

    virtual void setVisible(bool bVisible = true);

    M_SYNTHESIZE_PASS_BY_REF(Point, m_oAnchorPoint, AnchorPoint);
    M_SYNTHESIZE_READONLY(ProjectileSprite*, m_pSprite, Sprite);
    ProjectileSprite* createSprite();

    virtual void playFireSound();
    virtual void playEffectSound();

};


#endif  /* __DRAWFORCC_H__ */
