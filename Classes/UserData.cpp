#include "CommHeader.h"
#include "UserData.h"
#include "GameControl.h"
#include "GameData.h"
#include "Stage.h"


// CUserData
CUserData::CUserData()
: m_heroSel(0)
, m_stageSel(0)
{
    setDbgClassName("CUserData");
}

void CUserData::reset()
{
    m_heroes.clear();
    m_heroSel = 0;
    m_stageGrades.clear();
    m_stageSel = 0;
}

void CUserData::load(const char* name)
{
    reset();

    UserDefault* ud = UserDefault::getInstance();
    int stageGrades = ud->getIntegerForKey("stageGrades_size");
    for (int i = 0; i < stageGrades; ++i)
    {
        char sz[64];
        sprintf(sz, "stageGrades_%d", i);
        m_stageGrades.push_back(ud->getIntegerForKey(sz));
    }

    int heroes = ud->getIntegerForKey("heroes_size");
    for (int i = 0; i < heroes; ++i)
    {
        HERO_INFO hi;
        char sz[64];

        sprintf(sz, "hero_%d_id", i);
        hi.id = ud->getIntegerForKey(sz);

        sprintf(sz, "hero_%d_name", i);
        hi.name = ud->getStringForKey(sz);

        sprintf(sz, "hero_%d_desc", i);
        hi.desc = ud->getStringForKey(sz);

        sprintf(sz, "hero_%d_hp", i);
        hi.hp = ud->getFloatForKey(sz);

        sprintf(sz, "hero_%d_atkVal_type", i);
        hi.atkVal.setType(ud->getIntegerForKey(sz));

        sprintf(sz, "hero_%d_atkVal_value", i);
        hi.atkVal.setValue(ud->getFloatForKey(sz));

        sprintf(sz, "hero_%d_armVal_type", i);
        hi.armVal.setType(ud->getIntegerForKey(sz));

        sprintf(sz, "hero_%d_armVal_value", i);
        hi.armVal.setValue(ud->getFloatForKey(sz));

        sprintf(sz, "hero_%d_attackSpeed", i);
        hi.attackSpeed = ud->getFloatForKey(sz);

        sprintf(sz, "hero_%d_moveSpeed", i);
        hi.moveSpeed = ud->getFloatForKey(sz);

        sprintf(sz, "hero_%d_exp", i);
        hi.exp = ud->getIntegerForKey(sz);

        m_heroes.push_back(hi);
    }
}

void CUserData::save(const char* name)
{
    UserDefault* ud = UserDefault::getInstance();
    ud->setIntegerForKey("stageGrades_size", m_stageGrades.size());
    for (int i = 0; i < (int)m_stageGrades.size(); ++i)
    {
        char sz[64];
        sprintf(sz, "stageGrades_%d", i);
        ud->setIntegerForKey(sz, m_stageGrades[i]);
    }

    ud->setIntegerForKey("heroes_size", m_heroes.size());
    for (int i = 0; i < (int)m_heroes.size(); ++i)
    {
        HERO_INFO& hi = m_heroes[i];
        char sz[64];
        
        sprintf(sz, "hero_%d_id", i);
        ud->setIntegerForKey(sz, hi.id);

        sprintf(sz, "hero_%d_name", i);
        ud->setStringForKey(sz, hi.name);

        sprintf(sz, "hero_%d_desc", i);
        ud->setStringForKey(sz, hi.desc);

        sprintf(sz, "hero_%d_hp", i);
        ud->setFloatForKey(sz, hi.hp);

        sprintf(sz, "hero_%d_atkVal_type", i);
        ud->setIntegerForKey(sz, hi.atkVal.getType());

        sprintf(sz, "hero_%d_atkVal_value", i);
        ud->setFloatForKey(sz, hi.atkVal.getValue());

        sprintf(sz, "hero_%d_armVal_type", i);
        ud->setIntegerForKey(sz, hi.armVal.getType());

        sprintf(sz, "hero_%d_armVal_value", i);
        ud->setFloatForKey(sz, hi.armVal.getValue());

        sprintf(sz, "hero_%d_attackSpeed", i);
        ud->setFloatForKey(sz, hi.attackSpeed);

        sprintf(sz, "hero_%d_moveSpeed", i);
        ud->setFloatForKey(sz, hi.moveSpeed);

        sprintf(sz, "hero_%d_exp", i);
        ud->setIntegerForKey(sz, hi.exp);
    }

    ud->flush();
}

void CUserData::newGrades(int stage, int grade)
{
    for (int i = m_stageGrades.size(); i <= stage; ++i)
    {
        m_stageGrades.push_back(0);
    }

    if (grade > m_stageGrades[stage])
    {
        m_stageGrades[stage] = grade;
    }
}

void CUserData::updateGrades(CStageMap* stageMap)
{
    CStageMap::VEC_STAGE_INFOS& stages = stageMap->getStages();
    for (size_t i = m_stageGrades.size(); i < stages.size(); ++i)
    {
        m_stageGrades.push_back(0);
    }
    
    for (size_t i = 0; i < stages.size(); ++i)
    {
        m_stageGrades[i] = stages[i].pStage->getGrade();
    }
}

CGameData::STAGE_INFO* CUserData::getStageSelected()
{
    CGameData* gd = CGameData::instance();
    if (m_stageSel >= (int)gd->m_stages.size())
    {
        return nullptr;
    }

    return &gd->m_stages[m_stageSel];
}

CUserData::HERO_INFO* CUserData::getHeroSelected()
{
    if (m_heroSel >= (int)m_heroes.size())
    {
        return nullptr;
    }

    return &m_heroes[m_heroSel];
}
