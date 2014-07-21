#ifndef __COMMHEADER_H__
#define __COMMHEADER_H__


//#include <vld.h>

#include <float.h>
#include <math.h>

#include <cocos2d.h>
USING_NS_CC;

#include <extensions/cocos-ext.h>
USING_NS_CC_EXT;

#include <SimpleAudioEngine.h>
using namespace CocosDenshion;


#include "CommInc.h"
#include "Unit.h"


#define CC_SINGLETON(name) \
    inline static name* getInstance() \
    { \
        static name* pInst = nullptr; \
        if (!pInst) \
        { \
            pInst = name::create(); \
            pInst->retain(); \
        } \
        \
        return pInst; \
    }


#define M_ABILITY_BUTTON_CANCEL(node) dynamic_cast<CCAbilityButtonBase*>((node))->setClickRetCode(-1)

#define M_DEF_DR(var) Director* var = Director::getInstance()
#define M_DEF_FU(var) FileUtils* var = FileUtils::getInstance()
#define M_DEF_FC(var) SpriteFrameCache* var = SpriteFrameCache::getInstance()
#define M_DEF_AC(var) AnimationCache* var = AnimationCache::getInstance(
#define M_DEF_GC(var) static GameControler* var = GameControler::getInstance()

#define M_BASE_Z 10000

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define FONT_COMIC_BOOK "Comic Book"
#define FONT_ARIAL FONT_ARIAL
#define FONT_YIKES "Yikes!"
#else
#define FONT_COMIC_BOOK "fonts/Comic Book.ttf"
#define FONT_ARIAL "fonts/Arial.ttf"
#define FONT_YIKES "fonts/Yikes!.ttf"
#endif

typedef std::function<void(cocos2d::Node*)> FUNC_PNODE;

inline const cocos2d::Size& wsz()
{
    static const cocos2d::Size wsz = Director::getInstance()->getVisibleSize();
    return wsz;
}



#endif  /* __COMMHEADER_H__ */

