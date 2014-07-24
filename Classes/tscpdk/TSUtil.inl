/* 
 * File:   Util.inl
 * Author: thunderliu
 *
 * Created on 2013年9月25日, 下午1:09
 */

#ifndef __UTIL_INL__
#define	__UTIL_INL__

#include <stdlib.h>

// CAutoBuffer

inline CAutoBuffer::CAutoBuffer(size_t uSize)
: m_uSize(uSize)
{
    m_pBuf = malloc(uSize);
}

inline CAutoBuffer::~CAutoBuffer()
{
    likely (m_pBuf)
    {
        free(m_pBuf);
    }
}

inline CAutoBuffer::operator void*()
{
    return m_pBuf;
}

inline CAutoBuffer::operator const void*() const
{
    return m_pBuf;
}

inline void* CAutoBuffer::GetBuffer(size_t uOffset)
{
    return (uint8_t*)m_pBuf + uOffset;
}

inline size_t CAutoBuffer::GetSize() const
{
    return m_uSize;
}


#endif	/* __UTIL_INL__ */

