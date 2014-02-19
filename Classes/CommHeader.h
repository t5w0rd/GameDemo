#pragma once


#include <float.h>
#include <math.h>

#include <cocos2d.h>
USING_NS_CC;


#include "Logic\CommInc.h"
#include "Logic\Unit.h"


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

#define M_DEF_GC(var) CCGameControler* var = CCGameControler::sharedGameControler()
