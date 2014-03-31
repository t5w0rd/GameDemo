#ifndef __HEROROOM_SCENE_H__
#define __HEROROOM_SCENE_H__


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

    void onClickHeroPortrait(CCNode* pNode);
    void onClickButtonDone(CCNode* pNode);
    void onHideDone(CCNode* pNode);

public:
    CCSprite* m_sel;
    CCSprite* m_selBig;
    CCSprite* m_panel;
};


#endif // __HEROROOM_SCENE_H__
