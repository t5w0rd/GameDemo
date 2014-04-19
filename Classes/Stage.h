#ifndef __STAGE_H__
#define __STAGE_H__


class CStage
{
public:
    CStage();

    M_SYNTHESIZE(int, m_iIndex, Index);
    typedef set<int> SET_INDEXES;
    M_SYNTHESIZE_BY_REF_READONLY(SET_INDEXES, m_setNext, Next);
    M_SYNTHESIZE_READONLY(uint32_t, m_dwStatus, Status);
    void setStatus(uint32_t dwStatus);
    M_SYNTHESIZE(CCNode*, m_pNode, Node);

    virtual void onChangeStatus(uint32_t dwOldStatus);
};

class CStageMap
{
public:
    CStageMap();
    ~CStageMap();

    typedef vector<CStage*> VEC_STAGES;
    M_SYTHESIZE_BYREF_READONLY(VEC_STAGES, m_vecStages, Stages);

    typedef CStage::SET_INDEXES SET_INDEXES;
    void addStage(const SET_INDEXES& setPrev);

};

#endif // __STAGE_H__
