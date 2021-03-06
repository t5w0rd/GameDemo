/* 
 * File:   CommInc.h
 * Author: thunderliu
 *
 * Created on 2013��12��8��, ����10:44
 */

#ifndef __COMINC_H__
#define	__COMINC_H__


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <math.h>
#include <errno.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
using namespace std;

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
#include "utils/strnormalize.h"
}

#define LIBTSCPDK_SRC
#include "tscpdk/TSPlatform.h"
#include "tscpdk/TSUtil.h"
#include "tscpdk/TSSocket.h"
USING_NS_TSCPDK;

#include "CommDef.h"


#ifdef DEBUG_FOR_CC
// for cocos2d
#include "../CommHeader.h"
#include "../DrawForCC.h"
#endif


#endif	/* __COMINC_H__ */

