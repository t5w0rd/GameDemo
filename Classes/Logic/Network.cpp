/*
* File:   Network.cpp
* Author: thunderliu
*
* Created on 2014年7月24日, 上午0:45
*/

#include "CommInc.h"
#include "Network.h"
#include "TSSocket.h"


// CCommandNode
CCommandNode::CCommandNode()
: cmdCode(kEmpty)
{
}

void CCommandNode::clear()
{
    cmdCode = kEmpty;
}

// CCommandCache
CCommandCache::CCommandCache()
//: m_dwMtxCmdFlags(0)
{
    m_mtxCmds.resize(32);
}

CCommandNode* CCommandCache::getMutexCommandNodeToWrite(int cmdIndex)
{
    if (cmdIndex >= (int)m_mtxCmds.size())
    {
        return nullptr;
    }

    //m_dwMtxCmdFlags |= (1 << cmdIndex);
    return &m_mtxCmds[cmdIndex];
}

CCommandNode* CCommandCache::getSequenceCommandToWrite()
{
    m_seqCmds.push(CCommandNode());
    return &m_seqCmds.back();
}
