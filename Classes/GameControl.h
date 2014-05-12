#ifndef __GAMECONTROL_H__
#define __GAMECONTROL_H__

#include <cocos2d.h>


typedef Scene* (*SCENE_CREATOR)(void);

class GameControler : public cocos2d::Ref
{
public:
    GameControler(void);
    virtual ~GameControler(void);

    virtual bool init();
    CREATE_FUNC(GameControler);

    CC_SINGLETON(GameControler);

    void loadFrames(const char* pPath);
    Animation* loadAnimation(const char* pPath, const char* pName, float fDelay);

    SpriteFrame* getFrame(const char* pName);
    Animation* getAnimation(const char* pName);

    M_SYNTHESIZE_READONLY(SpriteFrameCache*, m_fc, fc);
    M_SYNTHESIZE_READONLY(AnimationCache*, m_ac, ac);
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
    static Layer* defaultLoadingLayer();
    void replaceSceneWithLoading(SCENE_CREATOR sceneCreator, Layer* loading = nullptr);
    void onReplaceScene(Node* node);

    M_SYNTHESIZE_READONLY(int, m_loadCount, LoadCount);
    M_SYNTHESIZE_READONLY(int, m_loaded, Loaded);

protected:
    vector<string> m_loadFrames;
    FUNC_VOID m_onLoadingProgressing;
    FUNC_VOID m_onLoadingDone;

public:
    void loadTexturesAsync(const vector<string>& frames, const vector<string>& otherTextures, const FUNC_VOID& onLoadingProgressing, const FUNC_VOID& onLoadingDone);
    void addTexturesLoadedToFramesCache();

protected:
    void onLoadingProgressing(Ref* obj);
};



////////////// Inline //////////////////////
inline GameControler& gc()
{
    M_DEF_GC(gc);
    return *gc;
}


#endif  /* __GAMECONTROL_H__ */

