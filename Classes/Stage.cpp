#include "CommHeader.h"

#include "Stage.h"


// CStage
CStage::CStage()
    : m_iIndex(0)
    , m_dwStatus(0)
    , m_pNode(NULL)
{
}

void CStage::setStatus(uint32_t dwStatus)
{
    if (m_dwStatus != dwStatus)
    {
        m_dwStatus = dwStatus;
    }

    uint32_t dwOldStatus = m_dwStatus;
    onChangeStatus(dwOldStatus);
}

// CStageMap
CStageMap::CStageMap()
{

}

CStageMap::~CStageMap()
{

}

void CStageMap::addStage(const SET_INDEXES& setPrev)
{

}
