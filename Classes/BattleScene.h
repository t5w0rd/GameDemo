#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "DrawForCC.h"


class CBattleWorld : public CUnitWorldForCC
{
public:
    CBattleWorld();
    virtual ~CBattleWorld();

    virtual void onInit();

    M_SYNTHESIZE(int, m_iHeroUnit, HeroUnit);

    enum UNIT_INDEX
    {
        kBladeMaster,
        kDemonHunter,
        kMountainKing
    };

    CUnit* createUnit(UNIT_INDEX eId);
};

class CCBattleScene : public CCScene
{
public:
    CCBattleScene();
    virtual ~CCBattleScene();
    
    virtual bool init();
    CREATE_FUNC(CCBattleScene);

    M_SYNTHESIZE(CUnitWorldForCC*, m_pWorld, World);

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

    void onMovePreviousLabel(CCNode* pCurLble, void* PreLbl);

};

#endif // __BATTLE_SCENE_H__
