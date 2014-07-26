#ifndef __NETWORK_FOR_CC_H__
#define __NETWORK_FOR_CC_H__

#include "Network.h"
#include "Protocol.hpp"
using namespace Protocol;


static const int CONST_MAGIC_NUMBER = 0xFFCC2014;

enum CMD_CODE
{
    kCmdRoomBroadCast
};


template <typename TYPE>
size_t packCmd(uint16_t wCmd, const TYPE& data, void* buf, size_t size);

template <typename TYPE>
inline size_t packCmd(uint16_t wCmd, const TYPE& data, void* buf, size_t size)
{
    uint8_t ac[4096];
    auto ret = data.SerializeToArray(ac, sizeof(ac));

    // stPack = dwMagic + wVersion + wCommand + wDataSize + cData[wDataSize]
    CType_stPack pkt;
    pkt.dwMagic() = CONST_MAGIC_NUMBER;
    pkt.wVersion() = MAKEWORD(0, 1);
    pkt.wCommand() = wCmd;
    pkt.Assign_acData(ac, ret);
    ret = pkt.SerializeToArray(buf, size);

    return ret;
}

class CNetwork : public CMultiRefObject
{
public:
    enum PROTO_TYPE
    {
        kClient,
        kServer
    };

    enum NET_STATUS
    {
        kDisconnected,
        kConnected,  // to recv
        kListening  // to accept
    };

    typedef function<void(CNetwork*)> FUNC_NET;

public:
    CNetwork();
    M_SINGLETON(CNetwork);

    M_SYNTHESIZE(PROTO_TYPE, m_protoType, ProtoType);
    M_SYNTHESIZE(NET_STATUS, m_netStatus, NetStatus);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CTcpSocket, m_dataSock, DataSock);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CTcpSocket, m_listenSock, ListenSock);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CUdpSocket, m_udpSock, UdpSock);
    M_SYNTHESIZE_STR(BroadCastAddress);

    void startThread();

    void doNetwork(const FUNC_NET& netFunc, const function<void()>& onReturn = nullptr);
    void connectToRemote(const string& host, int port, const function<void(bool)>& onReturn = nullptr);
    void waitForConnection(const string& host, int port, const function<void(bool, const string&, int)>& onReturn = nullptr);
    void discoverRemote(const string& host, int port, const function<void(bool, const string&, int)>& onReturn = nullptr);

protected:
    void _threadLoop();

    bool _connectToRemote(const string& host, int port);
    bool _waitForConnection(const string& host, int port);
    bool _bindUdpOnAddress(const string& host, int port);

protected:
    CSyncQueue<FUNC_NET> m_cmdQueue;
    const function<void(const function<void()>&)> m_cocos2dxFunc;
};




#endif  /* __NETWORK_FOR_CC_H__ */

