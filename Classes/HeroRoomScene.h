#ifndef __HEROROOM_SCENE_H__
#define __HEROROOM_SCENE_H__


class CCEffect;
class CCProgressBar;

class CCHeroRoomSceneLayer : public CCLayerColor
{
public:
    CCHeroRoomSceneLayer();
    virtual ~CCHeroRoomSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(CCHeroRoomSceneLayer);

    void onClickHeroPortrait(CCObject* pNode);
    void onClickButtonDone(CCObject* pNode);
    void onHideDone(CCNode* pNode);
    void onScaleDone(CCNode* pNode);
    void onAct5Done(CCNode* pNode);

public:
    CCSprite* m_selSmall;
    CCSprite* m_selBig;
    CCSprite* m_panel;
    CCEffect* m_eff;
    CCSprite* m_attackIcon;
    CCSprite* m_armorIcon;
    CCProgressBar* m_hpBar;
    CCProgressBar* m_attackBar;
    CCProgressBar* m_armorBar;
    CCProgressBar* m_attackSpeedBar;
    CCProgressBar* m_moveSpeedBar;
    CCLabelTTF* m_hpLbl;
    CCLabelTTF* m_attackLbl;
    CCLabelTTF* m_armorLbl;
    CCLabelTTF* m_attackSpeedLbl;
    CCLabelTTF* m_moveSpeedLbl;

    struct HERO_INFO
    {
        int id;
        const char* name;
        const char* desc;
        float hp;
        CAttackValue atkVal;
        CArmorValue armVal;
        float attackSpeed;
        float moveSpeed;
    };

    HERO_INFO m_heroVals[3];
    HERO_INFO m_maxVal;

    CCLabelTTF* m_name;
    CCLabelTTF* m_desc;

    CCSprite* m_blinkSmall;
    CCSprite* m_blinkBig;

    M_SYNTHESIZE(int, m_selIndex, SelectIndex);
};


#endif // __HEROROOM_SCENE_H__
