#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "UnitLibraryForCC.h"
#include "ComponentForCC.h"
#include "HeroRoomScene.h"
#include "DrawForCC.h"


class CStunBuff;
class CCHeroRoomSceneLayer;

class CBattleWorld : public CWorldForCC
{
public:
    static const float CONST_MAX_REWARD_RANGE;

public:
    CBattleWorld();
    virtual ~CBattleWorld();

    virtual bool onInit();
    virtual void onTick(float dt);
    bool onLuaWorldInit();
    void onLuaWorldTick(float dt);
    
    virtual CProjectile* copyProjectile(int id) const;

    virtual void onUnitDying(CUnit* pUnit);
    virtual void onUnitAttackTarget(CUnit* pUnit, CAttackData* pAttack, CUnit* pTarget);
    virtual void onUnitProjectileEffect(CUnit* pUnit, CProjectile* pProjectile, CUnit* pTarget);

    void onChangeGold(CMultiRefObject* obj);

    void onAniDone(CMultiRefObject* obj, void* data);

    M_SYNTHESIZE(CUnit*, m_pHero, Hero);
    M_SYNTHESIZE(CActiveAbility*, m_pThunderCapAct, ThunderCapAct);
    M_SYNTHESIZE(CActiveAbility*, m_pHammerThrowAct, HammerThrowAct);
    M_SYNTHESIZE(CActiveAbility*, m_pWarCryAct, WarCryAct);
    M_SYNTHESIZE(CPassiveAbility*, m_pRebirthPas, RebirthPas);
    M_SYNTHESIZE(CStunBuff*, m_pDragonStrikeBuff, DragonStrikeBuff);

    CUnitLibraryForCC m_oULib;

    M_SYNTHESIZE_STR(ScriptName);

    void endWithVictory(int grade);
    void endWithDefeat();
};

class CCBattleScene : public CCScene
{
public:
    CCBattleScene();
    virtual ~CCBattleScene();
    
    virtual bool init();
    CREATE_FUNC(CCBattleScene);

    M_SYNTHESIZE(CWorldForCC*, m_pWorld, World);

};

class CCProgressBar;
class CCBattleSceneLayer : public CCUnitLayer
{
public:
    CCBattleSceneLayer();
    virtual ~CCBattleSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool initWithWorld(CWorldForCC* pWorld);
    M_CREATEWITH_FUNC_PARAM(World, CCBattleSceneLayer, (CWorldForCC* pWorld), pWorld);

    void onLoadingProgress(CCObject* pObj);
    void onLoadingDone(CCObject* pObj);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(CCBattleSceneLayer);

    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    M_SYNTHESIZE(CCTouchMaskLayer*, m_ctrlLayer, CtrlLayer);
    M_SYNTHESIZE(CCTouchMaskLayer*, m_ctrlLayer2, CtrlLayer2);

    M_SYNTHESIZE(int, m_iMaxLogs, MaxLogs);
    M_SYNTHESIZE(int, m_iBaseLogId, BaseLogId);
    M_SYNTHESIZE(int, m_iCurLogId, CurLogId);
    void log(const char* fmt, ...);

    // TargetInfoLayer
    struct TARGET_INFO
    {
        uint32_t dwLevel;
        uint32_t dwHp;
        uint32_t dwMaxHp;
        uint32_t dwAtk0;
        uint32_t dwAtk1;
        uint32_t dwAtkEx;
        int iDef;

        TARGET_INFO() { memset(this, 0, sizeof(TARGET_INFO)); }
    };
    CCSprite* m_pTargetInfoPanel;
    CCLabelTTF* m_pTargetLv;
    CCLabelTTF* m_pTargetHp;
    CCLabelTTF* m_pTargetAtk;
    CCLabelTTF* m_pTargetAtkEx;
    CCSprite* m_pTargetAtkIcon;
    CCLabelTTF* m_pTargetDef;
    CCSprite* m_pTargetDefIcon;
    TARGET_INFO m_stTargetInfo;
    bool m_bShowTargetInfo;
    CCSprite* m_pPortraitSel;
    CCLabelTTF* m_pNameSel;
    void initTargetInfo();
    void updateTargetInfo(int id = 0);
    void showTargetInfo(bool bShow = true);

    //CCSprite* m_pHeroPortrait;
    CCProgressBar* m_pHeroHpBar;
    CCProgressBar* m_pHeroExpBar;
    CCLabelTTF* m_pHeroLevel;
    struct HERO_INFO
    {
        float fHpPer;
        float fExpPer;
        int iLevel;
        HERO_INFO() { memset(this, 0, sizeof(HERO_INFO)); }
    };
    HERO_INFO m_stHeroInfo;
    void initHeroPortrait();
    void updateHeroPortrait();

    // Gold
    CCLabelTTF* m_pGold;
    void initResourceInfo();
    void updateResourceInfo(int gold);

    int m_dscur;
    void onDragonStrikeUpdate(CCNode* pNode);

    void onClickFist(CCObject* pNode);
    void onClickHeroPortrait(CCObject* pNode);

    void onClickPause(CCObject* obj);

    M_SYNTHESIZE(CCSprite*, m_panel, CtrlPanel)
    void initCtrlPanel();
    void showCtrlPanel();
    void onCloseCtrlPanel(CCObject* obj);
    void onClickResume(CCObject* obj);
    void onClickRestart(CCObject* obj);
    void onClickQuit(CCObject* obj);
    void onClickSound(CCObject* obj);
    void onClickMusic(CCObject* obj);
    
};

#endif // __BATTLE_SCENE_H__
