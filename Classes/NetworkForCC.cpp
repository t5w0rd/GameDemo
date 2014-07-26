#include "CommHeader.h"

#include "NetworkForCC.h"


// CNetwork
CNetwork::CNetwork()
: m_protoType(kClient)
, m_netStatus(kDisconnected)
, m_cocos2dxFunc(bind(&Scheduler::performFunctionInCocosThread, Director::getInstance()->getScheduler(), placeholders::_1))
{
    setDbgClassName("CNetwork");
}

void CNetwork::startThread()
{
    thread(&CNetwork::_threadLoop, this).detach();
}

void CNetwork::doNetwork(const FUNC_NET& netFunc, const function<void()>& onReturn)
{
    m_cmdQueue.syncPush([this, netFunc, onReturn](CNetwork* net)
    {
        if (netFunc)
        {
            netFunc(net);
        }

        if (onReturn)
        {
            m_cocos2dxFunc(onReturn);
        }
    });
}

void CNetwork::connectToRemote(const string& host, int port, const function<void(bool)>& onReturn)
{
    m_cmdQueue.syncPush([this, host, port, onReturn](CNetwork* net)
    {
        auto res = net->_connectToRemote(host.c_str(), port);
        if (onReturn)
        {
            m_cocos2dxFunc(bind(onReturn, res));
        }
    });
}

void CNetwork::waitForConnection(const string& host, int port, const function<void(bool, const string&, int)>& onReturn)
{
    m_cmdQueue.syncPush([this, host, port, onReturn](CNetwork* net)
    {
        // 创建广播线程，有连接接入后终止掉
        auto res = _bindUdpOnAddress(host, 0);
        if (res == false && onReturn)
        {
            m_cocos2dxFunc(bind(onReturn, res, string(), 0));
            return;
        }

        atomic_bool stop;
        stop = false;
        CSockAddrIn to(m_sBroadCastAddress.c_str(), port);  // INADDR_BROADCAST

        // 线程函数
        thread bc([this, &stop, &to]()
        {
            char buf[4096];
            auto size = packCmd(kCmdRoomBroadCast, CType_stCmdRoomBroadCast(), buf, sizeof(buf));

            while (!stop.load())
            {
                // 循环发送广播包，每秒1次
                auto res = m_udpSock.SendTo(buf, size, &to);
                if (res < 0)
                {
                    CCLOG("MSG | sendto(%s:%d) err(%d)", to.GetAddr(), to.GetPort(), ::GetSocketError());
                }
                this_thread::sleep_for(chrono::seconds(1));
            }
        });

        // 等待tcp接入
        res = net->_waitForConnection(host.c_str(), port);
        stop = true;
        bc.join();
        if (onReturn)
        {
            auto sa = m_dataSock.GetSockAddrIn();
            m_cocos2dxFunc(bind(onReturn, res, res && sa ? sa->GetAddr() : string(), res && sa ? sa->GetPort() : 0));
        }
    });
}

void CNetwork::discoverRemote(const string& host, int port, const function<void(bool, const string&, int)>& onReturn)
{
    m_cmdQueue.syncPush([this, host, port, onReturn](CNetwork* net)
    {
        auto res = _bindUdpOnAddress(host, port);
        if (res == false && onReturn)
        {
            m_cocos2dxFunc(bind(onReturn, res, string(), 0));
            return;
        }

        thread([this, host, port, onReturn]()
        {
            char buf[4096];
            CSockAddrIn from;
            for (;;)
            {
                auto res = m_udpSock.RecvFrom(buf, sizeof(buf), &from);
                if (res < 0)
                {
                    CCLOG("MSG | recvfrom err(%d)", ::GetSocketError());
                    return;
                }

                // unpack
                if (onReturn)
                {
                    m_cocos2dxFunc(bind(onReturn, true, from.GetAddr(), from.GetPort()));
                }

            }
        }).detach();
    });
}

void CNetwork::_threadLoop()
{
    FUNC_NET func;
    for (;;)
    {
        while (m_cmdQueue.syncPop(func))
        {
            func(this);
        }
        printf("threadLoop()\n");
    }
}

bool CNetwork::_connectToRemote(const string& host, int port)
{
    if (m_netStatus != kDisconnected)
    {
        CCLOG("ERR | status(%d) err", m_netStatus);
        return false;
    }

    setProtoType(kClient);

    CCLOG("MSG | connecting to remote(%s:%d) ..", host.c_str(), port);
    auto res = m_dataSock.Connect(host.c_str(), port);
    if (res == false)
    {
        CCLOG("ERR | connect(%s:%d) err(%d)", host.c_str(), port, ::GetSocketError());
        return false;
    }
    setNetStatus(kConnected);
    CCLOG("MSG | connect to remote(%s:%d) succ", host.c_str(), port);

    return true;
}

bool CNetwork::_waitForConnection(const string& host, int port)
{
    if (m_netStatus != kDisconnected)
    {
        CCLOG("ERR | status(%d) err", m_netStatus);
        return false;
    }

    setProtoType(kServer);

    CCLOG("MSG | listening on local(%s:%d) ..", host.c_str(), port);
    auto res = m_listenSock.Bind(host.c_str(), port);
    if (res == false)
    {
        CCLOG("ERR | bind(%s:%d) err(%d)", host.c_str(), port, ::GetSocketError());
        return false;
    }

    res = m_listenSock.Listen();
    if (res == false)
    {
        CCLOG("ERR | listen() err(%d)", ::GetSocketError());
        return false;
    }
    setNetStatus(kListening);

    res = m_listenSock.Accept(&m_dataSock);
    if (res == false)
    {
        CCLOG("ERR | accept() err(%d)", ::GetSocketError());
        return false;
    }
    setNetStatus(kConnected);
    auto sa = m_dataSock.GetSockAddrIn();
    CCLOG("MSG | accept connection(%s:%d) succ", sa->GetAddr(), sa->GetPort());

    return true;
}

bool CNetwork::_bindUdpOnAddress(const string& host, int port)
{
    auto res = m_udpSock.Bind(host.c_str(), port);
    if (res == false)
    {
        CCLOG("ERR | bindudp(%s:%d) err(%d)", host.c_str(), port, ::GetSocketError());
        return false;
    }
    CCLOG("MSG | bindudp on local(%s:%d) succ", host.c_str(), port);

    const int opt = 1;
    m_udpSock.SetSockOpt(SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

    return true;
}
