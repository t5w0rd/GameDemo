#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "DrawForCC.h"
#include "UnitLibraryForCC.h"


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
    void onChangeGold(CMultiRefObject* obj);

    M_SYNTHESIZE(CUnit*, m_pHero, Hero);

    CUnitLibraryForCC m_oULib;

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

class CCBattleSceneLayer : public CCWinUnitLayer
{
public:
    CCBattleSceneLayer();
    virtual ~CCBattleSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(CCBattleSceneLayer);

    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    M_SYNTHESIZE(CCLayer*, m_pCtrlLayer, CtrlLayer);

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
        uint32_t dwDef;

        TARGET_INFO() { memset(this, 0, sizeof(TARGET_INFO)); }
    };
    CCLayerColor m_oTargetInfoLayer;
    CCLabelTTF m_oTargetLv;
    CCLabelTTF m_oTargetHp;
    CCLabelTTF m_oTargetAtk;
    CCLabelTTF m_oTargetAtkEx;
    CCSprite* m_pTargetAtk;
    CCLabelTTF m_oTargetDef;
    CCSprite* m_pTargetDef;
    TARGET_INFO m_stTargetInfo;
    void initTargetInfo();
    void updateTargetInfo(int id = 0);

    // Gold
    CCLabelTTF m_oGold;
    void initResourceInfo();
    void updateResourceInfo(int gold);

};

#endif // __BATTLE_SCENE_H__
