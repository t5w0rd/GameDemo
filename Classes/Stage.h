#ifndef __STAGE_H__
#define __STAGE_H__


class CStage
{
public:
    CStage();

    M_SYNTHESIZE(int, m_iIndex, Index);

    enum STAGE_STATUS
    {
        kLocked,
        kUnlocked,
        kConquered
    };
    M_SYNTHESIZE_READONLY(STAGE_STATUS, m_eStatus, Status);
    void setStatus(STAGE_STATUS eStatus);

    M_SYNTHESIZE(Node*, m_pNode, Node);
    M_SYNTHESIZE_PASS_BY_REF(Point, m_oPosition, Position);

    M_SYNTHESIZE_STR(Name);
    M_SYNTHESIZE_STR(Describe);
    M_SYNTHESIZE_STR(NormalName);
    M_SYNTHESIZE_STR(SelectedName);
    M_SYNTHESIZE_STR(DisabledName);
    M_SYNTHESIZE_STR(StarName);
    M_SYNTHESIZE_STR(UnstarName);
    M_SYNTHESIZE_READONLY(int, m_iGrade, Grade);
    void setGrade(int iGrade);
    
protected:
    Node* m_apStar[3];

public:
    void setStarNode(int iIndex, Node* pNode);
    Node* getStarNode(int iIndex);
    void setStarNodesVisible(bool bVisible = true);

    virtual void onInit();
    virtual void onChangeStatus(STAGE_STATUS eOldStatus);
};

class CStageMap
{
public:
    CStageMap();
    ~CStageMap();

    typedef vector<int> VEC_INDEXES;

    struct STAGE_NEXT_INFO
    {
        int iIndex;
        Node* pPath;
    };
    typedef vector<STAGE_NEXT_INFO> VEC_STAGE_NEXT_INFOS;

    struct STAGE_INFO
    {
        CStage* pStage;
        VEC_STAGE_NEXT_INFOS vecNextInfos;
    };
    typedef vector<STAGE_INFO> VEC_STAGE_INFOS;

    M_SYNTHESIZE_READONLY_PASS_BY_REF(VEC_STAGE_INFOS, m_stages, Stages);
    CStage* getStage(int iIndex);

    M_SYNTHESIZE_STR(PathName);

protected:
    Menu* m_pPanel;
    ccMenuCallback m_onClickStage;

public:
    void setPanel(Menu* pPanel, const ccMenuCallback& onClickStage);

    void addStage(CStage* pStage, const VEC_INDEXES& vecPrev);
    void addStageNextInfos(int iIndex, int iNextIndex);
    
    void setStageStatus(int iIndex, CStage::STAGE_STATUS eStatus);

};

#endif // __STAGE_H__
