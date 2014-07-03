/* 
 * File:   CommInc.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午10:44
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

#include <string>
#include <vector>
#include <set>
#include <map>
#include <functional>
using namespace std;

extern "C"
{
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
#include "utils/strnormalize.h"
}


#include "CommDef.h"


#ifdef DEBUG_FOR_CC
// for cocos2d
#include "../CommHeader.h"
#include "../DrawForCC.h"
#endif


#endif	/* __COMINC_H__ */

