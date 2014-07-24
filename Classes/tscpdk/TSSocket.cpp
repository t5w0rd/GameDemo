/* 
 * File:   TSSocket.cpp
 * Author: thunderliu
 * 
 * Created on 2011年12月20日, 下午6:21
 */

#define LIBTSCPDK_SRC

#include "TSPlatform.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "TSSocket.h"


NS_TSCPDK_BEGIN

// CSockAddr

CSockAddr::CSockAddr(const struct sockaddr* pstSockAddr, socklen_t dwSockLen)
{
    m_dwSockLen = dwSockLen;
    if (!pstSockAddr)
    {
        m_pstSockAddr = NULL;
        return;
    }
    m_pstSockAddr = (struct sockaddr*)malloc(m_dwSockLen);
    memmove(m_pstSockAddr, pstSockAddr, m_dwSockLen);
}

CSockAddr::CSockAddr(const CSockAddr& roSockAddr)
{
    m_dwSockLen = roSockAddr.m_dwSockLen;
    if (!roSockAddr.m_pstSockAddr)
    {
        m_pstSockAddr = NULL;
        return;
    }
    m_pstSockAddr = (struct sockaddr*)malloc(m_dwSockLen);
    memmove(m_pstSockAddr, roSockAddr.m_pstSockAddr, m_dwSockLen);
}

CSockAddr::operator struct sockaddr*()
{
    if (!m_pstSockAddr)
    {
        m_pstSockAddr = (struct sockaddr*)malloc(m_dwSockLen);
        //memset(m_pstSockAddr, 0, sizeof (struct sockaddr_in));
    }
    return (struct sockaddr*)m_pstSockAddr;
}


// CSockAddrIn
//CSockAddrIn CSockAddrIn::m_oConvert;

void CSockAddrIn::MakeSockAddrInStruct(struct sockaddr* pstSockAddr, const char* pAddr, int iPort)
{
    ((struct sockaddr_in*)pstSockAddr)->sin_family = AF_INET;
    if (pAddr)
    {
        ((struct sockaddr_in*)pstSockAddr)->sin_addr.s_addr = inet_addr(pAddr);
    }
    else
    {
        ((struct sockaddr_in*)pstSockAddr)->sin_addr.s_addr = INADDR_ANY;
    }
    ((struct sockaddr_in*)pstSockAddr)->sin_port = htons(iPort);
    memset(((struct sockaddr_in*)pstSockAddr)->sin_zero, 0, sizeof (((struct sockaddr_in*)pstSockAddr)->sin_zero));
}


// CSocket

bool CSocket::Bind(const CSockAddr* pSockAddr)
{
    if (m_iFd < 0)
    {
        if (!CreateSocket())
        {
            return false;
        }
    }

    if (!m_pSockAddr)
    {
        return false;
    }
    *m_pSockAddr = *pSockAddr;


    return bind(m_iFd, *m_pSockAddr, m_pSockAddr->GetSockLen()) == 0;
}

bool CSocket::SetRecvTimeout(int iTimeout)
{
    struct timeval tv;
    tv.tv_sec = iTimeout / 1000;
    tv.tv_usec = (iTimeout % 1000) * 1000;
    return SetSockOpt(SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof (struct timeval));
}

int CSocket::GetRecvTimeout() const
{
    struct timeval tv;
    socklen_t dwLen = sizeof (tv);
    bool bRet = GetSockOpt(SOL_SOCKET, SO_RCVTIMEO, &tv, &dwLen);
    if (!bRet)
    {
        return -1;
    }
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void CSocket::Close()
{
    unlikely(m_iFd < 0 || !OnClose())
    {
        return;
    }
    
    closesocket(m_iFd);
    m_iFd = -1;
    
    OnClosed();
}




// CIpSocket

bool CIpSocket::Bind(const CSockAddr* pSockAddr, bool bReuseAddr)
{
    if (m_iFd < 0)
    {
        if (!CreateSocket())
        {
            return false;
        }
    }
    
    int iOpt = bReuseAddr;
    if (!SetSockOpt(SOL_SOCKET, SO_REUSEADDR, &iOpt, sizeof (iOpt)))
    {
        return false;
    }
    
    return CSocket::Bind(pSockAddr);
}

bool CIpSocket::Bind(const char* pAddr, int iPort, bool bReuseAddr)
{
    CSockAddrIn oSa(pAddr, iPort);
    return CIpSocket::Bind(&oSa, bReuseAddr);
}

// CUdpSocket

bool CUdpSocket::CreateSocket()
{
    if (m_iFd > 0)
    {
        Close();
    }
    m_iFd = socket(PF_INET, SOCK_DGRAM, 0);
    return m_iFd > 0;
}


// CTcpSocket

bool CTcpSocket::CreateSocket()
{
    if (m_iFd > 0)
    {
        Close();
    }
    m_iFd = socket(PF_INET, SOCK_STREAM, 0);
    return m_iFd > 0;
}

bool CTcpSocket::Connect(const CSockAddr* pSockAddr)
{
    if (m_iFd < 0)
    {
        if (!CreateSocket())
        {
            return false;
        }
    }
    *m_pSockAddr = *pSockAddr;
    return connect(m_iFd, *pSockAddr, pSockAddr->GetSockLen()) == 0;
}

bool CTcpSocket::SendEx(const void* pBuf, int iSize)
{
    int iRet;
    int iPos = 0;
    while (iSize > 0)
    {
        iRet = Send((uint8_t*)pBuf + iPos, iSize);
        if (iRet <= 0)
        {
            return false;
        }

        iPos += iRet;
        iSize -= iRet;
    }

    return true;
}

bool CTcpSocket::RecvEx(void* pBuf, int iSize)
{
    int iRet;
    int iPos = 0;
    while (iSize > 0)
    {
        iRet = Recv((uint8_t*)pBuf + iPos, iSize);
        if (iRet <= 0)
        {
            return false;
        }

        iPos += iRet;
        iSize -= iRet;
    }

    return true;
}

#ifdef TSNETFW_FEATURE_PACKET
// CSocketAddrLl

void CSockAddrLl::MakeSockAddrLlStruct(struct sockaddr* pstSockAddr, const char* pInterface, const char* pHardAddr)
{
    static int iTmpSck = socket(AF_INET, SOCK_DGRAM, 0);
    memset((struct sockaddr_ll*)pstSockAddr, 0, sizeof (struct sockaddr_ll));
    ((struct sockaddr_ll*)pstSockAddr)->sll_family = AF_PACKET;
    ((struct sockaddr_ll*)pstSockAddr)->sll_protocol = htons(ETH_P_ALL);
    ((struct sockaddr_ll*)pstSockAddr)->sll_halen = ETH_ALEN;

    struct ifreq ifr;
    memset(&ifr, 0, sizeof (ifr));
    if (pInterface)
    {
        strcpy(ifr.ifr_name, pInterface);
        if (ioctl(iTmpSck, SIOCGIFINDEX, &ifr) < 0)
        {
            //LOG_POS("ioctl SIOCGIFINDEX err");
            return;
        }
        ((struct sockaddr_ll*)pstSockAddr)->sll_ifindex = ifr.ifr_ifindex;
    }

    /*
    memset(&ifr, 0, sizeof(ifr));
    if (pInterface)
    {
        strcpy(ifr.ifr_name, pInterface);
    }
    
    if (ioctl(iTmpSck, SIOCGIFHWADDR, &ifr) < 0)
    {
        LOG_POS("ioctl SIOCGIFHWADDR err");
        return;
    }
    
    memmove(((struct sockaddr_ll*)pstSockAddr)->sll_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
     */
    if (pHardAddr)
    {
        uint8_t* pAddr = ((struct sockaddr_ll*)pstSockAddr)->sll_addr;
        sscanf(pHardAddr, "%02x:%02x:%02x:%02x:%02x:%02x", (uint32_t*) & pAddr[0], (uint32_t*) & pAddr[1], (uint32_t*) & pAddr[2], (uint32_t*) & pAddr[3], (uint32_t*) & pAddr[4], (uint32_t*) & pAddr[5]);
    }
}

char* CSockAddrLl::GetAddr()
{
    static char szAddr[128];
    const uint8_t* pAddr = ((const struct sockaddr_ll*)m_pstSockAddr)->sll_addr;
    snprintf(szAddr, sizeof (szAddr), "%02x:%02x:%02x:%02x:%02x:%02x", pAddr[0], pAddr[1], pAddr[2], pAddr[3], pAddr[4], pAddr[5]);
    return szAddr;
}

// CEthSocket

bool CEthSocket::CreateSocket()
{
    if (m_iFd > 0)
    {
        Close();
    }
    m_iFd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    return m_iFd > 0;
}

bool CEthSocket::EnablePromisc(const char* pInterface, bool bEnable)
{
    if (m_iFd < 0)
    {
        errno = EBADF;
        return false;
    }
    if (!pInterface)
    {
        return false;
    }
    struct ifreq ifr;
    strncpy(ifr.ifr_name, pInterface, sizeof (ifr.ifr_name));
    if (ioctl(m_iFd, SIOCGIFFLAGS, &ifr) < 0)
    {
        return false;
    }
    if (bEnable)
    {
        ifr.ifr_flags |= IFF_PROMISC;
    }
    else
    {
        ifr.ifr_flags &= ~IFF_PROMISC;
    }
    if (ioctl(m_iFd, SIOCSIFFLAGS, &ifr) < 0)
    {
        return false;
    }
    return true;
}

void CEthSocket::MakeArpHeader(struct etharphdr* pArpHdr, const uint8_t* pSndrMac, in_addr_t dwSndrIp, const uint8_t* pTrgtMac, in_addr_t dwTrgtIp, uint16_t wOpCode)
{
    pArpHdr->ar_hrd = htons(ARPHRD_ETHER);
    pArpHdr->ar_pro = htons(ETH_P_IP);
    pArpHdr->ar_hln = ETH_ALEN;
    pArpHdr->ar_pln = 4;
    pArpHdr->ar_op = htons(wOpCode);
    memmove(pArpHdr->ar_sha, pSndrMac, sizeof (pArpHdr->ar_sha));
    memmove(pArpHdr->ar_sip, &dwSndrIp, sizeof (dwSndrIp));
    memmove(pArpHdr->ar_tha, pTrgtMac, sizeof (pArpHdr->ar_tha));
    memmove(pArpHdr->ar_tip, &dwTrgtIp, sizeof (dwTrgtIp));
}


#endif // TSNETFW_FEATURE_PACKET

NS_TSCPDK_END

