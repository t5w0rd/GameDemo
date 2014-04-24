#ifndef __USERDATA_H__
#define	__USERDATA_H__

#include "GameData.h"


class CStageMap;

class CUserData : CMultiRefObject
{
public:
    CUserData();

    void reset();
    void load(const char* name);
    void save(const char* name);

    M_SINGLETON(CUserData)

    struct HERO_INFO
    {
        int id;
        string name;
        string desc;
        float hp;
        CAttackValue atkVal;
        CArmorValue armVal;
        float attackSpeed;
        float moveSpeed;
    };

    typedef vector<HERO_INFO> VEC_HEROES;
    VEC_HEROES m_heroes;

    int m_heroSel;
    HERO_INFO* getHeroSelected();

    vector<int> m_stageGrades;
    void newGrades(int stage, int grade);
    void updateGrades(CStageMap* stageMap);

    int m_stageSel;
    CGameData::STAGE_INFO* getStageSelected();
};

#endif
