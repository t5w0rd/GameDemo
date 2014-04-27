#ifndef __UNPACK_SCENE_H__
#define __UNPACK_SCENE_H__


class CCUnpackScene : public CCScene
{
public:
    CCUnpackScene();
    virtual ~CCUnpackScene();
    
    virtual bool init();
    CREATE_FUNC(CCUnpackScene);

};

class CCSpriteFrameCacheEx;
class CCUnpackSceneLayer : public CCLayerColor
{
public:
    CCUnpackSceneLayer();
    virtual ~CCUnpackSceneLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(CCUnpackSceneLayer);

    void onLabelPrev(CCObject*);
    void onLabelNext(CCObject*);
    void onLabelSave(CCObject*);

    void updateSprite();

    bool saveToPng(const char* name, const char* path, CCSpriteFrameCache* fc);
    void scheduleSave(float ft);

    CCSpriteFrameCacheEx* m_fc;
    CCArray* m_arr;
    CCSprite* m_sp;
    unsigned int m_cur;

};

#endif // __UNPACK_SCENE_H__
