#ifndef __ABILITY_SCENE_H__
#define __ABILITY_SCENE_H__


class AbilitySceneLayer : public LayerColor
{
public:
    AbilitySceneLayer();
    virtual ~AbilitySceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(AbilitySceneLayer);

    void onClickAbilityItems(Node* abilityItem);
    void onClickAbilityItemsEquipped(Ref* ref);
    void onChangeAbilityItemEquipped(AbilityItem* ai, bool equipped);

protected:
    WinFormPanel* m_abilityItemsPanel;
    Node* m_winFormBorder;
    ButtonPanel* m_abilityEquippedPanel;
    Label* m_energy;
    int m_energyCost;
};


#endif // __ABILITY_SCENE_H__
