#include "CommHeader.h"
#include "UserData.h"
#include "GameControl.h"
#include "GameData.h"
#include "Stage.h"


// CUserData
CUserData::CUserData()
{
}

bool CUserData::init()
{
    return true;
}

void CUserData::load( const char* name )
{
    CCUserDefault* ud = CCUserDefault::sharedUserDefault();
    int stageGrades = ud->getIntegerForKey("stageGrades_size");
    for (int i = 0; i < stageGrades; ++i)
    {
        char sz[64];
        sprintf(sz, "stageGrades_%d", i);
        m_stageGrades.push_back(ud->getIntegerForKey(sz));
    }
}

void CUserData::save( const char* name )
{
    CCUserDefault* ud = CCUserDefault::sharedUserDefault();
    ud->setIntegerForKey("stageGrades_size", m_stageGrades.size());
    for (int i = 0; i < (int)m_stageGrades.size(); ++i)
    {
        char sz[64];
        sprintf(sz, "stageGrades_%d", i);
        ud->setIntegerForKey(sz, m_stageGrades[i]);
    }

    ud->flush();
}

void CUserData::newGrades( int stage, int grade )
{
    for (int i = m_stageGrades.size(); i <= stage; ++i)
    {
        m_stageGrades.push_back(0);
    }

    m_stageGrades[stage] = grade;
}

void CUserData::updateGrades( CStageMap* stageMap )
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
