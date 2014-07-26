/* 
* File:   TSPlatform.h
* Author: thunderliu
*
* Created on 2012年7月31日, 下午4:22
*/

#ifndef __TSPLATFORM_H__
#define	__TSPLATFORM_H__


// common macros
#define NS_TSCPDK_BEGIN     namespace tscpdk {
#define NS_TSCPDK_END       }
#define USING_NS_TSCPDK     using namespace tscpdk

#define EXTERN_C_BEGIN      extern "C" {
#define EXTERN_C_END        }


////////////////////////////////////////////////////////////////////////////

#if !defined(__GNUC__) || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
#define likely(x)       if((x))
#define unlikely(x)     if((x))
#else  
#define likely(x)       if(__builtin_expect((x) != 0, 1))
#define unlikely(x)     if(__builtin_expect((x) != 0, 0))
#endif

#include <stdio.h>
extern FILE* TS_STDIN;
extern FILE* TS_STDOUT;
extern FILE* TS_STDERR;

#if defined(TSCPDK_MSVC)

#if defined(LIBTSCPDK_SO_EXPORTS)
#define TSCPDK_API __declspec(dllexport)
#elif !defined(LIBTSCPDK_SRC)
#define TSCPDK_API __declspec(dllimport)
#else
#define TSCPDK_API
#endif

#define IS_PTH_VALID(hdl) ((hdl).p)

#include <WinSock2.h>
//#include <Windows.h>
inline void SleepForSec(unsigned int dwSec) { ::Sleep(dwSec * 1000); }
inline void SleepForMSec(unsigned int dwMSec) { ::Sleep(dwMSec); }

typedef int socklen_t;
inline void SetNonBlockForSocket(int iFd) { u_long uFlag = 0; ioctlsocket(iFd, FIONBIO, &uFlag); }
inline int GetSocketError() { return WSAGetLastError(); }
#include <time.h>
inline int gettimeofday(struct timeval *tp, void *tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon     = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min     = wtm.wMinute;
    tm.tm_sec     = wtm.wSecond;
    tm. tm_isdst    = -1;
    clock = mktime(&tm);
    tp->tv_sec = (long)clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;
    return 0;
}

#define localtime_r(t, tm) localtime_s((tm), (t))
#define asctime_r(tm, s) asctime_s((s), sizeof(s), tm)

inline unsigned long long int strtoull(const char *ptr, char **end, int base)
{
    unsigned long long ret = 0;

    if (base > 36)
        goto out;

    while (*ptr) {
        int digit;

        if (*ptr >= '0' && *ptr <= '9' && *ptr < '0' + base)
            digit = *ptr - '0';
        else if (*ptr >= 'A' && *ptr < 'A' + base - 10)
            digit = *ptr - 'A' + 10;
        else if (*ptr >= 'a' && *ptr < 'a' + base - 10)
            digit = *ptr - 'a' + 10;
        else
            break;

        ret *= base;
        ret += digit;
        ptr++;
    }

out:
    if (end)
        *end = (char *)ptr;

    return ret;
}


#else // TSCPDK_MSVC

#define TSCPDK_API

#define IS_PTH_VALID(hdl) (hdl)

#include <unistd.h>
inline void SleepForSec(unsigned int dwSec) { ::sleep(dwSec); }
inline void SleepForMSec(unsigned int dwMSec) { ::usleep(dwMSec * 1000); }

inline void closesocket(int iFd) { close(iFd); }
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
inline void SetNonBlockForSocket(int iFd) { int iFlag; (iFlag = fcntl(iFd, F_GETFL, 0)) >= 0 && fcntl(iFd, F_SETFL, iFlag | O_NONBLOCK); }
inline int GetSocketError() { return errno; }
#include <sys/epoll.h>

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>


#endif // TSCPDK_MSVC



#endif	/* __TSPLATFORM_H__ */

