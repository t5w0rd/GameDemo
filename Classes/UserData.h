#ifndef __USERDATA_H__
#define	__USERDATA_H__

#include "GameData.h"


class CStageMap;
class CValue;

class CUserData : CMultiRefObject
{
public:
    CUserData();

    void load();
    void reset();

    void loadUserData();
    void saveUserData();

    M_SINGLETON(CUserData);

    CValue* m_ud;
    
    struct HERO_INFO
    {
        HERO_INFO() : id(-1), attackSpeed(0.0f), moveSpeed(0.0f), exp(0.0f), energy(0.0f) {}
        int id;
        string name;
        string desc;
        float hp;
        CAttackValue atkVal;
        CArmorValue armVal;
        float attackSpeed;
        float moveSpeed;
        int exp;
        int energy;

        map<int, int> m_mapAbilitiesEquipped;
    };

    typedef vector<HERO_INFO> VEC_HEROES;
    VEC_HEROES m_heroes;

    int m_heroSel;
    HERO_INFO* getHeroSelected();

    vector<int> m_stageGrades;
    bool onLuaLoadingUserData();
    void newGrades(int stage, int grade);
    void updateGrades(CStageMap* stageMap);

    int m_stageSel;
    CGameData::STAGE_INFO* getStageSelected();

    struct ABILITY_INFO
    {
        int id;
        int level;
    };
    typedef vector<ABILITY_INFO> VEC_ABILITY_INFOS;
    VEC_ABILITY_INFOS m_vecAbilities;
};

#endif
