#ifndef __GAMECONTROL_H__
#define __GAMECONTROL_H__

#include <cocos2d.h>


typedef CCScene* (*SCENE_CREATOR)(void);

class CCGameControler : public cocos2d::CCObject
{
public:
    CCGameControler(void);

    virtual bool init();
    CREATE_FUNC(CCGameControler);

    CC_SINGLETON(GameControler);

    void loadFrames(const char* pPath);
    CCAnimation* loadAnimation(const char* pPath, const char* pName, float fDelay);

    CCSpriteFrame* getFrame(const char* pName);
    CCAnimation* getAnimation(const char* pName);

    M_SYNTHESIZE_READONLY(CCSpriteFrameCache*, m_fc, fc);
    M_SYNTHESIZE_READONLY(CCAnimationCache*, m_ac, ac);
    M_SYNTHESIZE_READONLY(SimpleAudioEngine*, m_ae, ae);

    virtual void step(float dt);
    
protected:
    typedef map<int, float> MAP_DURATION;
    MAP_DURATION m_mapSoundDur;

public:
    void preloadSound(const char* sound);
    int playSound(const char* sound, float duration = 0.0f);
    bool isSoundPlaying(int id) const;
    void stopSound(int id);

    void preloadMusic(const char* music);
    void playMusic(const char* music, bool loop = true);

protected:
    SCENE_CREATOR m_sceneCreator;

public:
    static CCLayer* defaultLoadingLayer();
    void replaceSceneWithLoading(SCENE_CREATOR sceneCreator, CCLayer* loading = NULL);
    void onReplaceScene(CCNode* node);

protected:
    int m_loadCount;
    int m_loaded;
    vector<string> m_loadFrames;
    CCObject* m_loadingTarget;
    SEL_CallFuncO m_loadingProgressing;
    SEL_CallFuncO m_loadingDone;

public:
    void loadTexturesAsync(const vector<string>& frames, const vector<string>& otherTextures, CCObject* target, SEL_CallFuncO loadingProgressing, SEL_CallFuncO loadingDone);
    void onLoadingProgressing(CCObject* obj);
};


#endif  /* __GAMECONTROL_H__ */

