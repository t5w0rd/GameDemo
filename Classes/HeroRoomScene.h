#ifndef __HEROROOM_SCENE_H__
#define __HEROROOM_SCENE_H__

#include "UserData.h"


class Effect;
class ProgressBar;

class HeroRoomSceneLayer : public LayerColor
{
public:
    HeroRoomSceneLayer();
    virtual ~HeroRoomSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(HeroRoomSceneLayer);

    Node* createFirstHeroPanel();
    Node* createSecondHeroPanel();
    void updateHeroSprite(int index, Node* panel);

    void onClickHeroPortrait(Ref* pNode);
    void onClickButtonDone(Ref* pNode);
    void onHideDone(Node* pNode);
    void onScaleDone(Node* pNode);
    void onAct5Done(Node* pNode);

public:
    Sprite* m_selSmall;
    Sprite* m_selBig;
    Sprite* m_panel;
    Effect* m_eff;
    Sprite* m_attackIcon;
    Sprite* m_armorIcon;
    ProgressBar* m_hpBar;
    ProgressBar* m_attackBar;
    ProgressBar* m_armorBar;
    ProgressBar* m_attackSpeedBar;
    ProgressBar* m_moveSpeedBar;
    Label* m_hpLbl;
    Label* m_attackLbl;
    Label* m_armorLbl;
    Label* m_attackSpeedLbl;
    Label* m_moveSpeedLbl;

    CUserData::HERO_INFO m_heroInfos[3];
    CUserData::HERO_INFO m_maxVal;

    Label* m_name;
    Label* m_desc;

    Sprite* m_blinkSmall;
    Sprite* m_blinkBig;

    M_SYNTHESIZE(int, m_selIndex, SelectIndex);
};


#endif // __HEROROOM_SCENE_H__
