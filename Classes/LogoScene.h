#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__


class LogoSceneLayer : public LayerColor
{
public:
    LogoSceneLayer();
    virtual ~LogoSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(LogoSceneLayer);

    void onEffectUpdate(Node* pNode);

protected:
    int m_cur;
};

class LogoScene2Layer : public LayerColor
{
public:
    LogoScene2Layer();
    virtual ~LogoScene2Layer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();

    // implement the "static node()" method manually
    CREATE_FUNC(LogoScene2Layer);

    void onEffectUpdate(Node* pNode);
    void onSoldierTurn(Node* pNode);
    void onClickSkip(Ref* pObject);

public:
    Layer* m_ctrl;

protected:
    int m_cur;
};

#endif // __LOGO_SCENE_H__
