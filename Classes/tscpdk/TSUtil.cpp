/* 
 * File:   Util.cpp
 * Author: thunderliu
 * 
 * Created on 2013年9月25日, 下午1:08
 */

#define LIBTSCPDK_SRC

#include <time.h>
#include <stdio.h>
#include "TSPlatform.h"
#include "TSUtil.h"


// CUtil

uint32_t CUtil::GetTodayFileName(const char* pPrefix, const char* pSuffix, int iDayOffset, char* pFileName, int iFileNameLen)
{
    time_t iNewTime = time(NULL);
    time_t iOffsetTime = iNewTime + iDayOffset * 86400;
    struct tm* pstTime = localtime(&iOffsetTime);
    uint32_t dwFileName = (pstTime->tm_year + 1900) * 10000 + (pstTime->tm_mon + 1) * 100 + pstTime->tm_mday;
    sprintf(pFileName, "%s%08d%s", pPrefix, dwFileName, pSuffix);
    return dwFileName;
}

