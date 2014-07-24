/* 
 * File:   Util.h
 * Author: thunderliu
 *
 * Created on 2013年9月25日, 下午1:08
 */

#ifndef __UTIL_H__
#define	__UTIL_H__


#include <stdint.h>


class TSCPDK_API CUtil
{
public:
    static uint32_t GetTodayFileName(const char* pPrefix, const char* pSuffix, int iDayOffset, char* pFileName, int iFileNameLen);
};


class TSCPDK_API CAutoBuffer
{
public:
    CAutoBuffer(size_t uSize);
    ~CAutoBuffer();
    operator void*();
    operator const void*() const;
    void* GetBuffer(size_t uOffset = 0);
    size_t GetSize() const;
    
protected:
    void* m_pBuf;
    size_t m_uSize;
};


#include "TSUtil.inl"

#endif	/* __UTIL_H__ */

