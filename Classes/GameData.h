#ifndef __GAMEDATA_H__
#define	__GAMEDATA_H__

class CGameData : CMultiRefObject
{
public:
    CGameData();

    M_SINGLETON(CGameData)

public:
    struct STAGE_INFO
    {
        string name;
        string desc;
        string btnNorName;
        string btnDisName;
        CCPoint pos;
        vector<int> prevIndex;
        string script;
        string background;
    };

    typedef vector<STAGE_INFO> VEC_STAGES;
    VEC_STAGES m_stages;
};

#endif
