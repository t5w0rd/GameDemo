#include "CommHeader.h"
#include "GameData.h"
#include "GameControl.h"
#include "AbilityLibrary.h"
#include "LuaBinding.h"
#include "LuaBindingForCC.h"
#include "LuaScriptEngine.h"
#include "Archive.h"


// CGameData
CGameData::CGameData()
{
    setDbgClassName("CGameData");

    auto L = CLuaScriptEngine::instance()->getLuaHandle();
    luaL_insertloader(L, luaModuleLoader4cc);
    luaRegCommFuncs(L);
    luaRegCommFuncsForCC(L);

    lua_getglobal(L, "setSearchPath");
    lua_call(L, 0, 0);
}

void CGameData::load()
{
    auto L = CLuaScriptEngine::instance()->getLuaHandle();

    int res = luaL_includefile(L, "LoadGame.lua");
    if (res != LUA_OK)
    {
        return;
    }

    initStageData();
    initAbilityData();
}

void CGameData::initStageData()
{
    const char* CONST_STAGE_MAP_NAME = "StageMap.tbl";

    STAGE_INFO stage;

    CValue* stages = nullptr;
    char path[256];
    sprintf(path, "%s/%s", CCFileUtils::getInstance()->getWritablePath().c_str(), CONST_STAGE_MAP_NAME);
    if (CArchive::loadValue(path, stages) == true)
    {
        int num = vmg_int(stages, "num");
        for (auto i = 1; i <= num; ++i)
        {
            auto st = vmg_vm(stages, i);
            stage.name = vmg_str(st, "name");
            stage.desc = vmg_str(st, "desc");
            stage.btnNorName = vmg_str(st, "btnNorName");
            stage.btnDisName = vmg_str(st, "btnDisName");
            stage.pos.x = vmg_flt(st, "posX");
            stage.pos.y = vmg_flt(st, "posY");
            stage.script = vmg_str(st, "script");
            stage.background = vmg_str(st, "background");
            
            stage.prevIndex.clear();
            auto prev = vmg_vm(st, "prev");
            for (auto j = 1; vmg_v(prev, j) != nullptr; ++j)
            {
                stage.prevIndex.push_back(vmg_int(prev, j) - 1);
            }

            m_stages.push_back(stage);
        }

        delete stages;
        return;
    }

    // stage 0
    stage.name = "The Sunwell";
    stage.desc = "The Sunwell is a fount of mystical power located in Quel\'Thalas";
    stage.btnNorName = "UI/Stage/Stage00Nor.png";
    stage.btnDisName = "UI/Stage/Stage00Dis.png";
    stage.pos = Point(200, 200);
    stage.prevIndex.clear();
    stage.script = "stage00.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 1
    stage.name = "The Death of the Captain";
    stage.desc = "It was created from a vial of water from the Well of Eternity";
    stage.btnNorName = "UI/Stage/Stage01Nor.png";
    stage.btnDisName = "UI/Stage/Stage01Dis.png";
    stage.pos = Point(500, 200);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(0);
    stage.script = "stage01.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 2
    stage.name = "Strike Back";
    stage.desc = "It empowered the high elves for approximately nine thousand years, until Arthas used it to resurrect Kel\'Thuzad as a lich";
    stage.btnNorName = "UI/Stage/Stage02Nor.png";
    stage.btnDisName = "UI/Stage/Stage02Dis.png";
    stage.pos = Point(800, 200);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(1);
    stage.script = "stage02.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 3
    stage.name = "Doubt";
    stage.desc = "His resurrection tainted the Sunwell in the process, severing the high elves from their source of power";
    stage.btnNorName = "UI/Stage/Stage03Nor.png";
    stage.btnDisName = "UI/Stage/Stage03Dis.png";
    stage.pos = Point(500, 550);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(1);
    stage.script = "stage03.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 4
    stage.name = "Siege";
    stage.desc = "Kael\'thas attempted to summon Kil\'jaeden into Azeroth through the Sunwell using Anveena\'s powers";
    stage.btnNorName = "UI/Stage/Stage04Nor.png";
    stage.btnDisName = "UI/Stage/Stage04Dis.png";
    stage.pos = Point(1100, 200);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(2);
    stage.prevIndex.push_back(3);
    stage.script = "stage04.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    // stage 5
    stage.name = "The End";
    stage.desc = "In World of Warcraft, the Sunwell appears in the level 70 raid dungeon, Sunwell Plateau, on the Isle of Quel\'Danas";
    stage.btnNorName = "UI/Stage/Stage05Nor.png";
    stage.btnDisName = "UI/Stage/Stage05Dis.png";
    stage.pos = Point(1100, 550);
    stage.prevIndex.clear();
    stage.prevIndex.push_back(4);
    stage.script = "stage05.lua";
    stage.background = "backgrounds/BackgroundHD01.png";
    m_stages.push_back(stage);

    stages = new CValueMap();
    vms_int(stages, "num", m_stages.size());
    
    for (int i = 0; i < (int)m_stages.size(); ++i)
    {
        auto& e = m_stages[i];
        auto st = new CValueMap();

        vms_str(st, "name", e.name);
        vms_str(st, "desc", e.desc);
        vms_str(st, "btnNorName", e.btnNorName);
        vms_str(st, "btnDisName", e.btnDisName);
        vms_flt(st, "posX", e.pos.x);
        vms_flt(st, "posY", e.pos.y);
        vms_str(st, "script", e.script);
        vms_str(st, "background", e.background);
        
        auto prev = new CValueMap();
        for (int j = 0; j < (int)e.prevIndex.size(); ++j)
        {
            vms_int(prev, j + 1, e.prevIndex[j] + 1);
        }
        vms_v(st, "prev", prev);

        vms_v(stages, i + 1, st);
    }

    CArchive::saveValue(path, stages);
    delete stages;
}

void CGameData::initAbilityData()
{
}
