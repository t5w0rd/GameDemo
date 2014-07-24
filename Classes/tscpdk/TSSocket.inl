/* 
 * File:   TSSocket.inl
 * Author: thunderliu
 *
 * Created on 2011年12月20日, 下午6:22
 */

#ifndef __TSSOCKET_INL__
#define	__TSSOCKET_INL__

//#include "TSSocket.h"


NS_TSCPDK_BEGIN

// CSockAddr

inline CSockAddr::~CSockAddr()
{
    if (m_pstSockAddr)
    {
        free(m_pstSockAddr);
    }
}

inline CSockAddr::operator const struct sockaddr*() const
{
    return (const struct sockaddr*)m_pstSockAddr;
}

inline CSockAddr& CSockAddr::operator =(const CSockAddr& roSockAddr)
{
    memmove((struct sockaddr*)(*this), (const struct sockaddr*)roSockAddr, m_dwSockLen);
    return *this;
}

inline void CSockAddr::SetSockAddr(const struct sockaddr* pstSockAddr)
{
    memmove((struct sockaddr*)(*this), pstSockAddr, m_dwSockLen);
}

inline socklen_t CSockAddr::GetSockLen() const
{
    return m_dwSockLen;
}

// CSockAddrIn

inline CSockAddrIn::CSockAddrIn(const struct sockaddr* pstSockAddr)
: CSockAddr(pstSockAddr, sizeof (struct sockaddr_in))
{
}

inline CSockAddrIn::CSockAddrIn(const char* pAddr, int iPort)
: CSockAddr(NULL, sizeof (struct sockaddr_in))
{
    m_pstSockAddr = (struct sockaddr*)malloc(sizeof(struct sockaddr_in));
    CSockAddrIn::MakeSockAddrInStruct(m_pstSockAddr, pAddr, iPort);
}

inline void CSockAddrIn::SetSockAddrIn(const char* pAddr, int iPort)
{
    CSockAddrIn::MakeSockAddrInStruct((struct sockaddr*)(*this), pAddr, iPort);
}

inline CSockAddrIn::operator struct sockaddr_in*()
{
    if (!m_pstSockAddr)
    {
        m_pstSockAddr = (struct sockaddr*)malloc(sizeof(struct sockaddr_in));
        //memset(m_pstSockAddr, 0, sizeof (struct sockaddr_in));
    }
    return (struct sockaddr_in*)m_pstSockAddr;
}

inline CSockAddrIn::operator const struct sockaddr_in*() const
{
    return (const struct sockaddr_in*)m_pstSockAddr;
}

inline char* CSockAddrIn::GetAddr()
{
    return m_pstSockAddr ? inet_ntoa(((struct sockaddr_in*)m_pstSockAddr)->sin_addr) : NULL;
}

inline int CSockAddrIn::GetPort()
{
    return m_pstSockAddr ? ntohs(((struct sockaddr_in*)m_pstSockAddr)->sin_port) : -1;
}


// CSocket

inline CSocket::CSocket(int iSock)
: m_iFd(iSock)
, m_pSockAddr(NULL)
{
}

inline CSocket::~CSocket()
{
    Close();
}

inline CSockAddr* CSocket::GetSockAddr()
{
    return m_pSockAddr;
}

inline bool CSocket::SetSockOpt(int iLevel, int iOptName, const void* pOptVal, socklen_t uOptlen)
{
    return setsockopt(m_iFd, iLevel, iOptName, (const char*)pOptVal, uOptlen) == 0;
}

inline bool CSocket::GetSockOpt(int iLevel, int iOptName, void* pOptVal, socklen_t* pOptLen) const
{
    return getsockopt(m_iFd, iLevel, iOptName, (char*)pOptVal, pOptLen) == 0;
}

inline bool CSocket::SetSendBuffLen(int iSendBuffLen)
{
    socklen_t uOptLen = sizeof(iSendBuffLen);
    return setsockopt(m_iFd, SOL_SOCKET, SO_SNDBUF, (const char*)&iSendBuffLen, uOptLen) == 0;
}

inline int CSocket::GetSendBuffLen()
{
    int iSendBuffLen = 0;
    socklen_t uOptLen = sizeof(iSendBuffLen);
    return getsockopt(m_iFd, SOL_SOCKET, SO_SNDBUF, (char*)&iSendBuffLen, &uOptLen) == 0 ? iSendBuffLen : -1;
}

inline bool CSocket::SetRecvBuffLen(int iRecvBuffLen)
{
    socklen_t uOptLen = sizeof(iRecvBuffLen);
    return setsockopt(m_iFd, SOL_SOCKET, SO_RCVBUF, (char*)&iRecvBuffLen, uOptLen) == 0;
}

inline int CSocket::GetRecvBuffLen()
{
    int iRecvBuffLen = 0;
    socklen_t uOptLen = sizeof(iRecvBuffLen);
    return getsockopt(m_iFd, SOL_SOCKET, SO_RCVBUF, (char*)&iRecvBuffLen, &uOptLen) == 0 ? iRecvBuffLen : -1;
}

inline int CSocket::Send(const void* pBuf, int iSize)
{
    return send(m_iFd, (const char*)pBuf, iSize, 0);
}

inline int CSocket::Recv(void* pBuf, int iSize)
{
    return recv(m_iFd, (char*)pBuf, iSize, 0);
}

inline int CSocket::SendTo(const void* pBuf, int iSize, const CSockAddr* pSockAddr)
{
    unlikely (m_iFd < 0)
    {
        if (!CreateSocket())
        {
            return -1;
        }
    }
    
    return sendto(m_iFd, (const char*)pBuf, iSize, 0, *pSockAddr, pSockAddr->GetSockLen());
}

inline int CSocket::RecvFrom(void* pBuf, int iSize, CSockAddr* pSockAddr)
{
    socklen_t dwAddrSize = sizeof (struct sockaddr);
    return recvfrom(m_iFd, (char*)pBuf, iSize, 0, *pSockAddr, &dwAddrSize);
}


inline void CSocket::SetNonBlock()
{
    SetNonBlockForSocket(m_iFd);
}

inline bool CSocket::IsInUse() const
{
    return m_iFd >= 0;
}

inline int CSocket::GetHandle() const
{
    return m_iFd;
}

inline void CSocket::SetHandle(int iFd)
{
    m_iFd = iFd;
}

inline CSocket::operator int() const
{
    return m_iFd;
}

inline bool CSocket::OnRead()
{
    return true;
}

inline bool CSocket::OnWrite()
{
    return true;
}

inline bool CSocket::OnError()
{
    return true;
}

inline bool CSocket::OnClose()
{
    return true;
}

inline bool CSocket::OnClosed()
{
    return true;
}

// CIpSocket

inline CIpSocket::CIpSocket(int iSock)
: CSocket(iSock)
{
    m_pSockAddr = new CSockAddrIn;
}

inline CIpSocket::~CIpSocket()
{
    if (m_pSockAddr)
    {
        delete m_pSockAddr;
        m_pSockAddr = NULL;
    }
    Close();
}

inline CSockAddrIn* CIpSocket::GetSockAddrIn()
{
    return (CSockAddrIn*)GetSockAddr();
}


// CUdpSocket

inline CUdpSocket::CUdpSocket(int iSock)
: CIpSocket(iSock)
//, m_iEpoll(-1)
{
}

inline bool CUdpSocket::EnableBroadcast(bool bEnable)
{
    int iOpt = bEnable;
    return SetSockOpt(SOL_SOCKET, SO_BROADCAST, &iOpt, sizeof (iOpt));
}


// CTcpSocket

inline CTcpSocket::CTcpSocket(int iSock)
: CIpSocket(iSock)
{
}

inline bool CTcpSocket::Connect(const char* pAddr, int iPort)
{
    CSockAddrIn oSa(pAddr, iPort);
    return Connect(&oSa);
}

inline bool CTcpSocket::Listen(int iBacklog)
{
    return listen(m_iFd, iBacklog) == 0;
}

inline bool CTcpSocket::Accept(CTcpSocket* pStreamSock)
{
    socklen_t uAddrSize = sizeof (struct sockaddr_in);
    pStreamSock->m_iFd = accept(m_iFd, *(pStreamSock->GetSockAddr()), &uAddrSize);
    return pStreamSock->m_iFd != -1;
}


#ifdef TSNETFW_FEATURE_PACKET
// CSockAddrLl

inline CSockAddrLl::CSockAddrLl(const struct sockaddr* pstSockAddr)
: CSockAddr(pstSockAddr, sizeof (struct sockaddr_ll))
{
}

inline CSockAddrLl::CSockAddrLl(const char* pInterface, const char* pHardAddr)
: CSockAddr(NULL, sizeof (struct sockaddr_ll))
{
    m_pstSockAddr = (struct sockaddr*)malloc(sizeof(struct sockaddr_ll));
    CSockAddrLl::MakeSockAddrLlStruct(m_pstSockAddr, pInterface, pHardAddr);
}

inline void CSockAddrLl::SetSockAddrLl(const char* pInterface, const char* pHardAddr)
{
    CSockAddrLl::MakeSockAddrLlStruct((struct sockaddr*)(*this), pInterface, pHardAddr);
}

inline CSockAddrLl::operator struct sockaddr_ll*()
{
    if (NULL == m_pstSockAddr)
    {
        m_pstSockAddr = (struct sockaddr*)malloc(sizeof(struct sockaddr_ll));
        //memset(m_pstSockAddr, 0, sizeof (struct sockaddr_in));
    }
    return (struct sockaddr_ll*)m_pstSockAddr;
}

inline CSockAddrLl::operator const struct sockaddr_ll*() const
{
    return (const struct sockaddr_ll*)m_pstSockAddr;
}


// CEthSocket

inline CEthSocket::CEthSocket(int iSock)
: CSocket(iSock)
{
    m_pSockAddr = new CSockAddrLl;
}

inline CEthSocket::~CEthSocket()
{
    if (m_pSockAddr)
    {
        delete m_pSockAddr;
        m_pSockAddr = NULL;
    }
}

inline bool CEthSocket::Bind(const char* pInterface)
{
    return CSocket::Bind(CSockAddrLl::ConvertToSockAddrLl(pInterface, NULL));
}

inline void CEthSocket::MakeEthernetHeader(struct ethhdr* pEthHdr, const uint8_t* pDstMac, const uint8_t* pSrcMac, uint16_t wType)
{
    memmove(pEthHdr->h_dest, pDstMac, sizeof (pEthHdr->h_dest));
    memmove(pEthHdr->h_source, pSrcMac, sizeof (pEthHdr->h_source));
    pEthHdr->h_proto = htons(wType);
}
#endif // TSNETFW_FEATURE_PACKET

NS_TSCPDK_END

#endif	/* __TSSOCKET_INL__ */

