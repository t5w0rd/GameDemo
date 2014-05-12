#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "UnitLibraryForCC.h"
#include "ComponentForCC.h"
#include "HeroRoomScene.h"
#include "DrawForCC.h"


class CStunBuff;
class HeroRoomSceneLayer;

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
    M_SYNTHESIZE_BOOL(LuaWorldTickEnabled);
    
    virtual CProjectile* copyProjectile(int id) const;

    virtual void onUnitDying(CUnit* pUnit) override;
    virtual void onUnitAttackTarget(CUnit* pUnit, CAttackData* pAttack, CUnit* pTarget) override;
    virtual void onUnitProjectileEffect(CUnit* pUnit, CProjectile* pProjectile, CUnit* pTarget) override;
    virtual void onUnitAddActiveAbility(CUnit* pUnit, CActiveAbility* pAbility) override;
    virtual void onUnitDelActiveAbility(CUnit* pUnit, CActiveAbility* pAbility) override;
    virtual void onUnitAbilityCD(CUnit* pUnit, CAbility* pAbility) override;

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
    
};

class BattleScene : public Scene
{
public:
    BattleScene();
    virtual ~BattleScene();
    
    virtual bool init();
    CREATE_FUNC(BattleScene);

    M_SYNTHESIZE(CWorldForCC*, m_pWorld, World);

};

class ProgressBar;
class BattleSceneLayer : public UnitLayer
{
public:
    BattleSceneLayer();
    virtual ~BattleSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool initWithWorld(CWorldForCC* pWorld);
    M_CREATEWITH_FUNC_PARAM(World, BattleSceneLayer, (CWorldForCC* pWorld), pWorld);

    void onLoadingProgress();
    void onLoadingDone();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(BattleSceneLayer);

    virtual void onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event* event) override;
    M_SYNTHESIZE(TouchMaskLayer*, m_ctrlLayer, CtrlLayer);
    M_SYNTHESIZE(TouchMaskLayer*, m_ctrlLayer2, CtrlLayer2);

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
    Sprite* m_pTargetInfoPanel;
    Label* m_pTargetLv;
    Label* m_pTargetHp;
    Label* m_pTargetAtk;
    Label* m_pTargetAtkEx;
    Sprite* m_pTargetAtkIcon;
    Label* m_pTargetDef;
    Sprite* m_pTargetDefIcon;
    TARGET_INFO m_stTargetInfo;
    bool m_bShowTargetInfo;
    Sprite* m_pPortraitSel;
    Label* m_pNameSel;
    int m_iTargetInfoUnitId;
    void initTargetInfo();
    void updateTargetInfo(int id = 0);
    void showTargetInfo(bool bShow = true);

    //Sprite* m_pHeroPortrait;
    ProgressBar* m_pHeroHpBar;
    ProgressBar* m_pHeroExpBar;
    Label* m_pHeroLevel;
    struct HERO_INFO
    {
        float fHpPer;
        float fExpPer;
        int iLevel;
        HERO_INFO() { memset(this, 0, sizeof(HERO_INFO)); }
    };
    HERO_INFO m_stHeroInfo;
    ButtonPanel* m_bp;
    void initHeroPortrait();
    void updateHeroPortrait();

    ButtonBase* createAbilityButton(CAbility* ability);
    void initHeroAbilityPanel();
    void updateHeroAbilityPanel();
    void cancelAbilityWaiting(int abilityId, int cancelTag);

    void onClickFist(Ref* btn);
    void onClickHeroPortrait(Ref* btn);
    void onClickAbilityButton(Ref* btn);

    // Gold
    Label* m_pGold;
    void initResourceInfo();
    void updateResourceInfo(int gold);

    int m_dscur;
    void onDragonStrikeUpdate(Node* pNode);

    void onClickPause(Ref* obj);

    M_SYNTHESIZE(PopPanel*, m_panel, CtrlPanel)
    void initCtrlPanel();
    void showCtrlPanel();
    void onCloseCtrlPanel(Ref* obj);
    void onClickResume(Ref* obj);
    void onClickRestart(Ref* obj);
    void onClickQuit(Ref* obj);
    void onClickSound(Ref* obj);
    void onClickMusic(Ref* obj);

    void endWithVictory(int grade);
    void endWithDefeat();

    void onShowVictoryDone(Node* node);
    
};

#endif // __BATTLE_SCENE_H__
