#ifndef __GAMECONTROL_H__
#define __GAMECONTROL_H__

#include <cocos2d.h>


class CCGameControler : public cocos2d::CCObject
{
public:
    CCGameControler(void);

    virtual bool init();
    CREATE_FUNC(CCGameControler);

    CC_SINGLETON(GameControler);

    void loadTexture(const char* pPath);
    CCAnimation* loadAnimation(const char* pPath, const char* pName, float fDelay);

    CCSpriteFrame* getFrame(const char* pName);
    CCAnimation* getAnimation(const char* pName);

    M_SYNTHESIZE_READONLY(CCSpriteFrameCache*, m_fc, fc);
    M_SYNTHESIZE_READONLY(CCAnimationCache*, m_ac, ac);
    M_SYNTHESIZE_READONLY(SimpleAudioEngine*, m_ae, ae);

    virtual void step(float dt);
    
protected:
    typedef map<int, float> MAP_DURATION;
    MAP_DURATION m_mapEffectDur;

public:
    int playEffect(const char* effect, float duration = 0.0f);
    bool isEffectPlaying(int id) const;
    void stopEffect(int id);

    void playMusic(const char* music);
    
};



#endif  /* __GAMECONTROL_H__ */

