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

// CIdGen
int CIdGen::m_id = 0;
int CIdGen::nextId()
{
    if (m_id == 0)
    {
        srand(time(nullptr));
        m_id = 1000000 + rand() % 9000;
    }
    
    return ++m_id;
}

void CIdGen::resetId(int id)
{
    m_id = id;
}

