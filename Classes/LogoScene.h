#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__


class CCLogoSceneLayer : public CCLayerColor
{
public:
    CCLogoSceneLayer();
    virtual ~CCLogoSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(CCLogoSceneLayer);

    void onEffectUpdate(CCNode* pNode);

protected:
    int m_cur;
};

class CCLogoScene2Layer : public CCLayerColor
{
public:
    CCLogoScene2Layer();
    virtual ~CCLogoScene2Layer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(CCLogoScene2Layer);

    void onEffectUpdate(CCNode* pNode);
    void onSoldierTurn(CCNode* pNode);

protected:
    int m_cur;
};

#endif // __LOGO_SCENE_H__
