#include "CommHeader.h"
#include "UserData.h"
#include "GameControl.h"
#include "GameData.h"
#include "Stage.h"
#include "LuaScriptEngine.h"
#include "LuaBinding.h"
#include "LuaBindingForCC.h"
#include "UnitLibraryForCC.h"


// CUserData
CUserData::CUserData()
: m_heroSel(0)
, m_stageSel(0)
{
    setDbgClassName("CUserData");
}

void CUserData::load()
{
    reset();
    onLuaLoadingUserData();
    HERO_INFO hero;
    hero.id = CUnitLibraryForCC::kBarracks;
    m_heroes.push_back(hero);
    CTcpSocket s;
    
    CSockAddrIn sa(nullptr, 2888);
    s.Bind(&sa, true);
    s.Listen();
    CTcpSocket c;
    //s.Accept(&c);
}

void CUserData::reset()
{
    m_heroes.clear();
    m_heroSel = 0;
    m_stageGrades.clear();
    m_stageSel = 0;
}

bool CUserData::onLuaLoadingUserData()
{
    // lua
    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();

    lua_getglobal(L, "onLoadingUserData");
    int res = lua_pcall(L, 0, 0, 0);
    if (res != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        CCLOG("ERR | LuaErr(%s: %d): %s", __FILE__, (int)__LINE__, err);
        lua_pop(L, 1);

        return false;
    }

    assert(lua_gettop(L) == 0);

    return true;
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
