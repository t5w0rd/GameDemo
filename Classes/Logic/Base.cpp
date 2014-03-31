/* 
 * File:   Base.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月9日, 上午12:42
 */

#include "CommInc.h"
#include "Base.h"
#include "Application.h"


// CApplicationLog
void CDebugLog::Log(const char* pFormat, ...)
{
    return;
    //fprintf(stdout, "[%03.2f] ", CLogicApplication::sharedApplication()->getElapsedTime());
    va_list argv;
    va_start(argv, pFormat);
    vfprintf(stdout, pFormat, argv);
    va_end(argv);
    fprintf(stdout, "\n");
}

// CKeyGen 
int CKeyGen::nextKey()
{
    static int iKey = 0;
    if (!iKey)
    {
        srand(time(NULL));
        iKey = 1000 + rand() % 9000;
    }
    
    return ++iKey;
}
