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
};


#endif  /* __GAMECONTROL_H__ */
