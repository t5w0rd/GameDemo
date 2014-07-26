#ifndef __EMPTY_SCENE_H__
#define __EMPTY_SCENE_H__


class EmptySceneLayer : public LayerColor
{
public:
    EmptySceneLayer();
    virtual ~EmptySceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(EmptySceneLayer);

    void lab00();

    unordered_map<string, int> m_mapHost;

};


#endif // __EMPTY_SCENE_H__
