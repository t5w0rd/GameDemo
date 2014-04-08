#ifndef __EMPTY_SCENE_H__
#define __EMPTY_SCENE_H__


class CCEmptySceneLayer : public CCLayerColor
{
public:
    CCEmptySceneLayer();
    virtual ~CCEmptySceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(CCEmptySceneLayer);

};


#endif // __EMPTY_SCENE_H__
