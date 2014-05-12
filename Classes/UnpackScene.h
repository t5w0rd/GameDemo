#ifndef __UNPACK_SCENE_H__
#define __UNPACK_SCENE_H__


class UnpackScene : public Scene
{
public:
    UnpackScene();
    virtual ~UnpackScene();
    
    virtual bool init();
    CREATE_FUNC(UnpackScene);

};

class SpriteFrameCacheEx;
class UnpackSceneLayer : public LayerColor
{
public:
    UnpackSceneLayer();
    virtual ~UnpackSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(UnpackSceneLayer);

    void onLabelPrev(Ref*);
    void onLabelNext(Ref*);
    void onLabelSave(Ref*);

    void updateSprite();

    bool saveToPng(const char* name, const char* path, SpriteFrameCache* fc);
    void scheduleSave(float ft);

    SpriteFrameCacheEx* m_fc;
    vector<string> m_arr;
    Sprite* m_sp;
    unsigned int m_cur;

};

#endif // __UNPACK_SCENE_H__
