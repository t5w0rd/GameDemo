#ifndef __COMMHEADER_H__
#define __COMMHEADER_H__


#include <float.h>
#include <math.h>

#include <cocos2d.h>
USING_NS_CC;

#include <SimpleAudioEngine.h>
using namespace CocosDenshion;


#include "CommInc.h"
#include "Unit.h"


#define CC_SINGLETON(name) \
    inline static CC##name* shared##name() \
    { \
        static CC##name* pInst = NULL; \
        if (!pInst) \
        { \
            pInst = CC##name::create(); \
            pInst->retain(); \
        } \
        \
        return pInst; \
    }

#define M_DEF_GC(var) static CCGameControler* var = CCGameControler::sharedGameControler()
#define M_BASE_Z 10000

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define FONT_COMIC_BOOK "Comic Book"
#define FONT_ARIAL "Arial"
#else
#define FONT_COMIC_BOOK "fonts/Comic Book.ttf"
#define FONT_ARIAL "fonts/Arial.ttf"
#endif

#endif  /* __COMMHEADER_H__ */

