/* 
 * File:   Network.h
 * Author: thunderliu
 *
 * Created on 2014年7月24日, 上午0:45
 */

#ifndef __NETWORK_H__
#define	__NETWORK_H__

#include "MultiRefObject.h"


class CCommandNode
{
public:
    enum CMD_CODE
    {
        kEmpty
    };

public:
    CCommandNode();

    void clear();

    int cmdCode;
    string cmdData;
};

// 每个Unit以及World都持有一个该对象，该对象缓存接收到的命令，在World命令帧的时候执行
class CCommandCache
{
public:
    // 互斥性命令的命令索引
    enum MTX_CMD_INDEX
    {
        kUnitPosUpdate,  // 单位位置更新
        kUnitOprCmd
    };

    typedef vector<CCommandNode> VEC_MTX_CMDS;
    typedef list<CCommandNode> LIST_SEQ_CMDS;

public:
    CCommandCache();

    CCommandNode* getMutexCommandNodeToWrite(int cmdIndex);
    CCommandNode* getSequenceCommandToWrite();

protected:
    //M_SYNTHESIZE(uint32_t, m_dwMtxCmdFlags, MutexCommandFlags);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_MTX_CMDS, m_mtxCmds, MutexCommands);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(LIST_SEQ_CMDS, m_seqCmds, SequenceCommands);

};

class CNetwork
{
public:
    enum ROLE_TYPE
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

public:
    CNetwork();

    M_SYNTHESIZE(ROLE_TYPE, m_netType, NetType);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CTcpSocket, m_dataSock, DataSock);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(CTcpSocket, m_listenSock, ListenSock);

    void WriteWorldCommand(const CCommandNode& cmd);
    void WriteUnitCommand(int id, const CCommandNode& cmd);

protected:

};


#endif	/* __NETWORK_H__ */

