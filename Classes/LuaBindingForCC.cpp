#include "CommHeader.h"

#include "LuaBindingForCC.h"
#include "LuaBinding.h"
#include "DrawForCC.h"
#include "GameControl.h"
#include "AbilityForCC.h"
#include "BattleScene.h"


int luaModuleLoader4cc(lua_State *L)
{
    std::string filename(luaL_checkstring(L, 1));
    size_t pos = filename.rfind(".lua");
    if (pos != std::string::npos)
    {
        filename = filename.substr(0, pos);
    }

    pos = filename.find_first_of(".");
    while (pos != std::string::npos)
    {
        filename.replace(pos, 1, "/");
        pos = filename.find_first_of(".");
    }
    filename.append(".lua");

    Data data = FileUtils::getInstance()->getDataFromFile(filename);

    if (!data.isNull())
    {
        if (luaL_loadbuffer(L, (char*)data.getBytes(), data.getSize(), filename.c_str()) != 0)
        {
            luaL_error(L, "error loading module %s from file %s :\n\t%s",
                lua_tostring(L, 1), filename.c_str(), lua_tostring(L, -1));
        }
    }
    else
    {
        log("can not get file data of %s", filename.c_str());
    }

    return 1;
}

bool luaL_loadscript4cc(lua_State *L, const char* name, string& err)
{
#if 0 && (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string code("require \"");
    code.append(name);
    code.append("\"");
    int nRet = luaL_dostring(L, code.c_str());
#else
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(name);
    int nRet = luaL_dofile(L, fullPath.c_str());
#endif
    if (nRet != 0)
    {
        err = lua_tostring(L, -1);
        lua_pop(L, 1);

        return false;
    }

    return true;
}

luaL_Reg Unit4CC_funcs[] = {
    { "ctor", Unit4CC_ctor },
    { "addBattleTip", Unit4CC_addBattleTip },
    { "setGeometry", Unit4CC_setGeometry },
    { nullptr, nullptr }
};

int Unit4CC_ctor(lua_State* L)
{
    CSpriteInfo* si = nullptr;
    luaL_toobjptr(L, 2, si);
    const char* name = lua_tostring(L, 3);

    CUnitDrawForCC* d = new CUnitDrawForCC(si);
    CUnit* u = new CUnit(d);
    u->setName(name);

    lua_pushlightuserdata(L, u);
    lua_setfield(L, 1, "_p");

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    w->addUnit(u);

    return 0;
}

int Unit4CC_addBattleTip(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    const char* tip = lua_tostring(L, 2);
    const char* font = lua_tostring(L, 3);
    float fontSize = lua_tonumber(L, 4);
    unsigned int r = lua_tounsigned(L, 5);
    unsigned int g = lua_tounsigned(L, 6);
    unsigned int b = lua_tounsigned(L, 7);

    CUnitDrawForCC* d = DCAST(_p->getDraw(), CUnitDrawForCC*);
    d->addBattleTip(tip, font, fontSize, Color3B(r, g, b));

    return 0;
}

int Unit4CC_setGeometry(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float fHalfOfWidth = lua_tonumber(L, 2);
    float fHalfOfHeight = lua_tonumber(L, 3);
    Point anchorPoint(lua_tonumber(L, 4), lua_tonumber(L, 5));
    CPoint firePoint(lua_tonumber(L, 6), lua_tonumber(L, 7));

    CUnitDrawForCC* d = DCAST(u->getDraw(), CUnitDrawForCC*);
    d->setGeometry(fHalfOfWidth, fHalfOfHeight, anchorPoint, firePoint);

    return 0;
}

luaL_Reg Projectile4CC_funcs[] = {
    { "ctor", Projectile4CC_ctor },
    { nullptr, nullptr }
};

int Projectile4CC_ctor(lua_State* L)
{
    CSpriteInfo* si = nullptr;
    luaL_toobjptr(L, 2, si);

    CProjectileForCC* p = new CProjectileForCC(si);

    lua_pushlightuserdata(L, p);
    lua_setfield(L, 1, "_p");

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    w->addProjectile(p);

    return 0;
}

luaL_Reg UnitPath4CC_funcs[] = {
    { "addPoints", UnitPath4CC_addPoints },
    { nullptr, nullptr }
};

int UnitPath4CC_addPoints(lua_State* L)
{
    CUnitPathForCC* _p = nullptr;
    luaL_toobjptr(L, 1, _p);
    const char* f = lua_tostring(L, 2);

    _p->addPoints(f);

    return 0;
}

CUnitDrawForCC* luaL_tospriteptr(lua_State* L, int idx /*= 1*/)
{
    lua_getfield(L, idx, "_p");
    CUnitDrawForCC* ret = (CUnitDrawForCC*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    return ret;
}

luaL_Reg Sprite_funcs[] = {
    { "ctor", Sprite_ctor },
    { "prepareFrame", Sprite_prepareFrame },
    { "prepareAnimation", Sprite_prepareAnimation },
    { nullptr, nullptr }
};

int Sprite_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);

    CSpriteInfo* d = new CSpriteInfo(name);
    lua_pushlightuserdata(L, d);
    lua_setfield(L, 1, "_p");

    return 0;
}

int Sprite_prepareFrame(lua_State* L)
{
    CSpriteInfo* si = nullptr;
    luaL_toobjptr(L, 1, si);
    int id = lua_tointeger(L, 2);
    const char* name = lua_tostring(L, 3);

    si->prepareFrame(id, name);

    return 0;
}

int Sprite_prepareAnimation(lua_State* L)
{
    CSpriteInfo* si = nullptr;
    luaL_toobjptr(L, 1, si);
    int id = lua_tointeger(L, 2);
    const char* name = lua_tostring(L, 3);
    int notify = lua_tointeger(L, 4);

    si->prepareAnimation(id, name, notify);

    return 0;
}

int g_log(lua_State* L)
{
    lua_getglobal(L, "string");
    lua_getfield(L, -1, "format");
    int n = lua_gettop(L);
    for (int i = 1; i <= n; ++i)
    {
        lua_pushvalue(L, i);
    }
    lua_call(L, n, 1);

    const char* log = lua_tostring(L, -1);
    CCLOG("%s", log);

    lua_getglobal(L, "_world");
    CWorldForCC* w = (CWorldForCC*)lua_touserdata(L, -1);
    DCAST(w->getLayer(), BattleSceneLayer*)->log("%s", log);

    lua_pop(L, 3);  // pop res and "string" and _world

    return 0;
}

int g_loadFrames(lua_State* L)
{
    const char* name = lua_tostring(L, 1);

    M_DEF_GC(gc);
    gc->loadFrames(name);

    return 0;
}

int g_loadAnimation(lua_State* L)
{
    const char* path = lua_tostring(L, 1);
    const char* name = lua_tostring(L, 2);
    float delay = lua_tonumber(L, 3);

    M_DEF_GC(gc);
    gc->loadAnimation(path, name, delay);

    return 0;
}

int g_addTemplateUnit(lua_State* L)
{
    int id = lua_tointeger(L, 1);
    CUnit* u = luaL_tounitptr(L, 2);

    lua_getglobal(L, "_world");
    CBattleWorld* w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    w->m_oULib.addUnit(id, u);

    return 0;
}

int g_addTemplateProjectile(lua_State* L)
{
    int id = lua_tointeger(L, 1);
    CProjectile* p = nullptr;
    luaL_toobjptr(L, 2, p);

    lua_getglobal(L, "_world");
    CBattleWorld* w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    w->m_oULib.addProjectile(id, p);

    return 0;
}

int g_createUnit(lua_State* L)
{
    int id = lua_tointeger(L, 1);

    lua_getglobal(L, "_world");
    CBattleWorld* w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    CUnit* _p = w->m_oULib.copyUnit(id);
    w->addUnit(_p);

    luaL_pushobjptr(L, "Unit", _p);

    return 1;
}

int g_createProjectile(lua_State* L)
{
    int id = lua_tointeger(L, 1);

    lua_getglobal(L, "_world");
    CBattleWorld* w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    CProjectile* _p = w->m_oULib.copyProjectile(id);
    w->addProjectile(_p);

    luaL_pushobjptr(L, "Projectile", _p);

    return 1;
}

int g_showDebug(lua_State* L)
{
    bool bOn = lua_toboolean(L, 1) != 0;

    Director::getInstance()->setDisplayStats(bOn);

    return 0;
}

int g_playSound(lua_State* L)
{
    const char* eff = lua_tostring(L, 1);

    SimpleAudioEngine::getInstance()->playEffect(eff);

    return 0;
}

string g_sLuaSearchPath;

int g_setSearchPath(lua_State* L)
{
    if (lua_gettop(L) == 0)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        g_sLuaSearchPath = "/sdcard/ts/gamedemo";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        g_sLuaSearchPath = "/var/mobile/Documents";
#else
        g_sLuaSearchPath = "scripts";
#endif
    }
    else
    {
        const char* path = lua_tostring(L, 1);
        if (path != nullptr)
        {
            g_sLuaSearchPath = path;
        }
    }

    return 0;
}

int g_include(lua_State* L)
{
    const char* inc = lua_tostring(L, 1);

    char sz[256];

    sprintf(sz, "%s/%s", g_sLuaSearchPath.c_str(), inc);

    string err;
    if (luaL_loadscript4cc(L, sz, err) == false)
    {
        return luaL_error(L, "%s", err.c_str());
    }

    return 0;
}

int g_endWithVictory(lua_State* L)
{
    int grade = lua_tointeger(L, 1);

    lua_getglobal(L, "_world");
    CBattleWorld* w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world
    DCAST(w->getLayer(), BattleSceneLayer*)->endWithVictory(grade);

    return 0;
}

int g_endWithDefeat(lua_State* L)
{
    lua_getglobal(L, "_world");
    CBattleWorld* w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world
    DCAST(w->getLayer(), BattleSceneLayer*)->endWithDefeat();

    return 0;
}

int g_save(lua_State* L)
{
    lua_getglobal(L, "_world");
    CBattleWorld* w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    CUserData::instance()->getHeroSelected()->exp = w->getHero()->getExp();
    CUserData::instance()->save("");

    return 0;
}

int luaRegWorldFuncsForCC(lua_State* L, CWorld* pWorld)
{
    // TODO: reg global vars

    // TODO: reg global funcs
    lua_register(L, "log", g_log);
    lua_register(L, "loadFrames", g_loadFrames);
    lua_register(L, "loadAnimation", g_loadAnimation);
    lua_register(L, "addTemplateUnit", g_addTemplateUnit);
    lua_register(L, "addTemplateProjectile", g_addTemplateProjectile);
    lua_register(L, "createUnit", g_createUnit);
    lua_register(L, "createProjectile", g_createProjectile);
    lua_register(L, "showDebug", g_showDebug);
    lua_register(L, "playSound", g_playSound);
    lua_register(L, "setSearchPath", g_setSearchPath);
    lua_register(L, "include", g_include);
    lua_register(L, "endWithVictory", g_endWithVictory);
    lua_register(L, "endWithDefeat", g_endWithDefeat);
    lua_register(L, "save", g_save);

    // TODO: patch global class members
    M_LUA_PATCH_CLASS_WITH_FUNCS(L, Unit, Unit4CC);
    M_LUA_PATCH_CLASS_WITH_FUNCS(L, Projectile, Projectile4CC);
    M_LUA_PATCH_CLASS_WITH_FUNCS(L, UnitPath, UnitPath4CC);

    // bind a class
    M_LUA_BIND_CLASS_WITH_FUNCS(L, Sprite);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, StatusShowPas, PassiveAbility);

    return 0;
}

int StatusShowPas_ctor(lua_State* L)
{
    CShowStatusPas* _p = new CShowStatusPas();
    //_p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}
