#ifndef __USERDATA_H__
#define	__USERDATA_H__


class CStageMap;

class CUserData : CMultiRefObject
{
public:
    CUserData();
    bool init();
    void load(const char* name);
    void save(const char* name);

    M_SINGLETON(CUserData)

    struct HERO_INFO
    {
        int id;
        string name;
        string desc;
        string hp;
        CAttackValue atkVal;
        CArmorValue armVal;
        float attackSpeed;
        float moveSpeed;
    };

    typedef vector<HERO_INFO> VEC_HEROES;
    VEC_HEROES m_heroes;

    vector<int> m_stageGrades;
    void newGrades(int stage, int grade);
    void updateGrades(CStageMap* stageMap);
    
};

#endif
