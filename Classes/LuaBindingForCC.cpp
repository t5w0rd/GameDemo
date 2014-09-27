#include "CommHeader.h"

#include "LuaBindingForCC.h"
#include "LuaBinding.h"
#include "DrawForCC.h"
#include "GameControl.h"
#include "AbilityForCC.h"
#include "BattleScene.h"



int luaL_throwerror(lua_State* L, bool tellWhere, const char* format, ...)
{
    va_list argp;
    va_start(argp, format);
    if (tellWhere)
    {
        luaL_where(L, 1);
    }
    lua_pushvfstring(L, format, argp);
    va_end(argp);
    if (tellWhere)
    {
        lua_concat(L, 2);
    }

    return lua_error(L);
}

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

int g_showDebug(lua_State* L)
{
    bool bOn = lua_toboolean(L, 1) != 0;

    Director::getInstance()->setDisplayStats(bOn);

    return 0;
}

int g_playMusic(lua_State* L)
{
    auto music = lua_tostring(L, 1);
    auto loop = luaL_tobooleandef(L, 2, true);

    M_DEF_GC(gc);
    gc->playMusic(music, loop);

    return 0;
}

int g_playSound(lua_State* L)
{
    auto sound = lua_tostring(L, 1);
    float dur = luaL_tonumberdef(L, 2, 0.0f);
    
    M_DEF_GC(gc);
    lua_pushinteger(L, gc->playSound(sound, dur));

    return 1;
}

int g_isSoundPlaying(lua_State* L)
{
    int sid = lua_tointeger(L, 1);

    M_DEF_GC(gc);
    lua_pushboolean(L, gc->isSoundPlaying(sid));

    return 1;
}

string g_sLuaSearchPath;

int g_setSearchPath(lua_State* L)
{
    if (lua_gettop(L) == 0)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        g_sLuaSearchPath = "/sdcard/ts/gamedemo/scripts";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        g_sLuaSearchPath = "/var/mobile/Documents/scripts";
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

int g_addTemplateUnit(lua_State* L)
{
    int id = lua_tointeger(L, 1);
    CUnit* u = luaL_tounitptr(L, 2);

    CUnitLibraryForCC::instance()->addUnit(id, u);

    return 0;
}

int g_addTemplateProjectile(lua_State* L)
{
    int id = lua_tointeger(L, 1);
    CProjectile* p = nullptr;
    luaL_toobjptr(L, 2, p);

    CUnitLibraryForCC::instance()->addProjectile(id, p);

    return 0;
}

int g_addAbilityToUserData(lua_State* L)
{
    auto id = lua_tointeger(L, 1);
    auto lvl = lua_tointeger(L, 2);
    CUserData::ABILITY_INFO info;
    info.id = id;
    info.level = lvl;
    CUserData::instance()->m_vecAbilities.push_back(info);
    
    return 0;
}

int g_getWritablePath(lua_State* L)
{
    auto path = FileUtils::getInstance()->getWritablePath();
    
    lua_pushstring(L, path.c_str());
    
    return 1;
}

int g_isFileExist(lua_State* L)
{
    auto file = lua_tostring(L, 1);

    lua_pushboolean(L, FileUtils::getInstance()->isFileExist(file));

    return 1;
}

int g_setGameSpeed(lua_State* L)
{
    float spd = lua_tonumber(L, 1);

    Director::getInstance()->getScheduler()->setTimeScale(spd);

    return 0;
}

int g_msgBox(lua_State* L)
{
    char msgu[1024];
    char titleu[1024];
    auto msg = luaL_toutf8string(L, 1, msgu);
    auto title = luaL_toutf8string(L, 2, titleu);

    MessageBox(msgu, titleu); vector<char>v;

    return 0;
}

int g_setStageGrade(lua_State* L)
{
    auto index = lua_tointeger(L, 1) - 1;
    auto grade = lua_tointeger(L, 2);

    if (index < 0)
    {
        return luaL_throwerror(L, true, "index(%d) must be greater than zero");
    }

    auto& sg = CUserData::instance()->m_stageGrades;
    if (index > (int)sg.size())
    {
        lua_pushnil(L);
        return 1;
    }
    else if (index == sg.size())
    {
        sg.push_back(grade);
    }
    else if (sg[index] < grade)
    {
        sg[index] = grade;
    }

    lua_pushinteger(L, sg[index]);

    return 1;
}

int g_getCurStageIndex(lua_State* L)
{
    lua_pushinteger(L, CUserData::instance()->m_stageSel + 1);

    return 1;
}

luaL_Reg Unit4CC_funcs[] = {
    { "ctor", Unit4CC_ctor },
    { "addBattleTip", Unit4CC_addBattleTip },
    { "setGeometry", Unit4CC_setGeometry },
    { "getAnchorPointInPoints", Unit4CC_getAnchorPointInPoints },
    { "addCtrlSound", Unit4CC_addCtrlSound },
    { "doRotateBy", Unit4CC_doRotateBy },
    
    
    { nullptr, nullptr }
};

int Unit4CC_ctor(lua_State* L)
{
    CSpriteInfo* si = nullptr;
    luaL_toobjptr(L, 2, si);
    const char* name = lua_tostring(L, 3);

    CUnitDrawForCC* d = new CUnitDrawForCC(si);
    CUnit* u = new CUnit(si->getName(), d);
    u->setName(name);

    lua_pushlightuserdata(L, u);
    lua_setfield(L, 1, "_p");

    //lua_getglobal(L, "_world");
    //CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    //lua_pop(L, 1);

    //w->addUnit(u);

    return 0;
}

int Unit4CC_addBattleTip(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    const char* tip = lua_tostring(L, 2);
    const char* font = lua_isnil(L, 3) ? nullptr : lua_tostring(L, 3);
    float fontSize = lua_tonumber(L, 4);
    bool ignoreColor = lua_gettop(L) < 5;
    
    CUnitDrawForCC* d = DCAST(_p->getDraw(), CUnitDrawForCC*);
    if (ignoreColor == false)
    {
        unsigned int r = lua_tounsigned(L, 5);
        unsigned int g = lua_tounsigned(L, 6);
        unsigned int b = lua_tounsigned(L, 7);
        d->addBattleTip(tip, font, fontSize, Color3B(r, g, b));
    }
    else
    {
        d->addBattleTip(tip, font, fontSize, Color3B(), true);
    }
    

    
    

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

int Unit4CC_getAnchorPointInPoints(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);

    CUnitDrawForCC* d = DCAST(u->getDraw(), CUnitDrawForCC*);

    auto& p = d->getSprite()->getAnchorPointInPoints();

    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);

    return 2;
}

int Unit4CC_addCtrlSound(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    auto sound = lua_tostring(L, 2);
    auto dur = lua_tonumber(L, 3);

    CUnitDrawForCC* d = DCAST(u->getDraw(), CUnitDrawForCC*);
    d->addCtrlSound(sound, dur);

    return 0;
}

int Unit4CC_doRotateBy(lua_State* L)
{
    CUnit* u = luaL_tounitptr(L);
    float delta = lua_tonumber(L, 2);
    float dur = lua_tonumber(L, 3);
    int times = luaL_tointegerdef(L, 4, 1);
    float spd = luaL_tonumberdef(L, 5, 1.0f);

    CUnitDrawForCC* d = DCAST(u->getDraw(), CUnitDrawForCC*);
    int tag = d->doRotateBy(delta, dur, times, nullptr, spd);

    lua_pushinteger(L, tag);

    return tag;
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

    //lua_getglobal(L, "_world");
    //CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    //lua_pop(L, 1);

    //w->addProjectile(p);

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

    int lastArg = lua_gettop(L);

    si->prepareAnimation(id, name, notify);

    for (int i = 5; i <= lastArg; ++i)
    {
        auto sound = lua_tostring(L, i);
        si->addAnimationSound(id, sound);
    }

    return 0;
}

luaL_Reg Effect_funcs[] = {
    { "ctor", Effect_ctor },
    { "addAnimation", Effect_addAnimation },
    { "addAnimationSound", Effect_addAnimationSound },
    { "play", Effect_play },
    { "playRelease", Effect_playRelease },
    { "playForever", Effect_playForever },
    { "stop", Effect_stop },
    { "setLogicPositionMode", Effect_setLogicPositionMode },
    { "isLogicPositionMode", Effect_isLogicPositionMode },
    { "setLogicPosition", Effect_setLogicPosition },
    { "getLogicPosition", Effect_getLogicPosition },
    { "setLogicHeight", Effect_setLogicHeight },
    { "getLogicHeight", Effect_getLogicHeight },
    { "setPosition", Effect_setPosition },
    { "getPosition", Effect_getPosition },
    { "setAnchorPoint", Effect_setAnchorPoint },
    { "getAnchorPoint", Effect_getAnchorPoint },
    { "release", Effect_release },
    
    { nullptr, nullptr }
};

int Effect_ctor(lua_State* L)
{
    auto path = lua_tostring(L, 2);
    auto delay = lua_tonumber(L, 3);
    auto ax = luaL_tonumberdef(L, 4, 0.5f);
    auto ay = luaL_tonumberdef(L, 5, 0.5f);
    auto sound = luaL_tostringdef(L, 6, nullptr);

    Effect* eff = Effect::create(path, delay, sound);
    if (eff == nullptr)
    {
        return luaL_throwerror(L, true, "effect(%s) is not exist", path);
    }

    lua_pushlightuserdata(L, eff);
    lua_setfield(L, 1, "_p");

    eff->setAnchorPoint(Point(ax, ay));

    if (lua_gettop(L) < 7)
    {
        lua_getglobal(L, "_world");
        auto w = (CBattleWorld*)lua_touserdata(L, -1);
        lua_pop(L, 1);  // pop _world
        w->getLayer()->addChild(eff); 
    }
    else
    {
        CUnit* u = luaL_tounitptr(L, 7);
        CUnitDrawForCC* d = DCAST(u->getDraw(), CUnitDrawForCC*);
        d->getSprite()->getShadow()->addChild(eff);
    }

    return 0;
}

int Effect_addAnimation(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto path = lua_tostring(L, 2);
    auto delay = lua_tonumber(L, 3);
    auto sound = luaL_tostringdef(L, 4, nullptr);

    eff->addAnimation(path, delay, sound);

    return 0;
}

int Effect_addAnimationSound(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto index = lua_tointeger(L, 2);
    auto sound = lua_tostring(L, 3);

    eff->addAnimationSound(index, sound);

    return 0;
}

int Effect_play(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto index = luaL_tointegerdef(L, 2, 0);
    auto speed = luaL_tonumberdef(L, 3, 1.0f);
    auto times = luaL_tointegerdef(L, 4, 1);

    eff->play(index, speed, times);

    return 0;
}

int Effect_playRelease(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto index = luaL_tointegerdef(L, 2, 0);
    auto speed = luaL_tonumberdef(L, 3, 1.0f);
    auto times = luaL_tointegerdef(L, 4, 1);

    eff->playRelease(index, speed, times);

    return 0;
}

int Effect_playForever(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto index = luaL_tointegerdef(L, 2, 0);
    auto speed = luaL_tonumberdef(L, 3, 1.0f);

    eff->playForever(index, speed);

    return 0;
}

int Effect_stop(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);

    eff->stop();

    return 0;
}

int Effect_setLogicPositionMode(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto b = lua_toboolean(L, 2) != 0;

    eff->setLogicPositionMode(b);

    return 0;
}

int Effect_isLogicPositionMode(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);

    lua_pushboolean(L, eff->isLogicPositionMode());

    return 1;
}

int Effect_setLogicPosition(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto x = lua_tonumber(L, 2);
    auto y = lua_tonumber(L, 3);

    eff->setLogicPosition(CPoint(x, y));

    return 0;
}

int Effect_getLogicPosition(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);

    auto p = eff->getLogicPosition();

    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);

    return 2;
}

int Effect_setLogicHeight(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto h = lua_tonumber(L, 2);

    eff->setLogicHeight(h);

    return 0;
}

int Effect_getLogicHeight(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    
    lua_pushnumber(L, eff->getLogicHeight());

    return 1;
}

int Effect_setPosition(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto x = lua_tonumber(L, 2);
    auto y = lua_tonumber(L, 3);

    eff->setPosition(Point(x, y));

    return 0;
}

int Effect_getPosition(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto& p = eff->getPosition();

    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);

    return 2;
}

int Effect_setAnchorPoint(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto x = lua_tonumber(L, 2);
    auto y = lua_tonumber(L, 3);

    eff->setAnchorPoint(Point(x, y));

    return 0;
}

int Effect_getAnchorPoint(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);
    auto& p = eff->getAnchorPoint();

    lua_pushnumber(L, p.x);
    lua_pushnumber(L, p.y);

    return 2;
}

int Effect_release(lua_State* L)
{
    Effect* eff = nullptr;
    luaL_toobjptr(L, 1, eff);

    eff->removeFromParentAndCleanup(true);

    return 0;
}

int luaRegCommFuncsForCC(lua_State* L)
{
    // TODO: reg global funcs
    lua_register(L, "loadFrames", g_loadFrames);
    lua_register(L, "loadAnimation", g_loadAnimation);
    lua_register(L, "showDebug", g_showDebug);
    lua_register(L, "playMusic", g_playMusic);
    lua_register(L, "playSound", g_playSound);
    lua_register(L, "isSoundPlaying", g_isSoundPlaying);
    lua_register(L, "setSearchPath", g_setSearchPath);
    lua_register(L, "include", g_include);
    lua_register(L, "addTemplateUnit", g_addTemplateUnit);
    lua_register(L, "addTemplateProjectile", g_addTemplateProjectile);
    lua_register(L, "addAbilityToUserData", g_addAbilityToUserData);
    lua_register(L, "getWritablePath", g_getWritablePath);
    lua_register(L, "isFileExist", g_isFileExist);
    lua_register(L, "setGameSpeed", g_setGameSpeed);
    lua_register(L, "msgBox", g_msgBox);
    lua_register(L, "setStageGrade", g_setStageGrade);
    lua_register(L, "getCurStageIndex", g_getCurStageIndex);

    // TODO: patch global class members
    M_LUA_PATCH_CLASS_WITH_FUNCS(L, Unit, Unit4CC);
    M_LUA_PATCH_CLASS_WITH_FUNCS(L, Projectile, Projectile4CC);
    M_LUA_PATCH_CLASS_WITH_FUNCS(L, UnitPath, UnitPath4CC);

    // TODO: reg global classes
    M_LUA_BIND_CLASS_WITH_FUNCS(L, Sprite);
    M_LUA_BIND_CLASS_WITH_FUNCS(L, Effect);
    M_LUA_BIND_CLASS_WITH_CTOR_EX(L, StatusShowPas, PassiveAbility);

    return 0;
}

int luaL_includefile(lua_State* L, const char* file)
{
    int res = 0;

    lua_getglobal(L, "include");
    lua_pushstring(L, file);
    res = lua_pcall(L, 1, 0, 0);
    if (res != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        CCLOG("ERR | LuaErr(%s: %d): %s", __FILE__, (int)__LINE__, err);
        lua_pop(L, 1);
    }

    return res;
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

    if (lua_pcall(L, n, 1, 0) != LUA_OK)
    {
        auto err = lua_tostring(L, -1);
        return luaL_throwerror(L, true, err);
    }

    const char* log = lua_tostring(L, -1);
    CCLOG("%s", log);

    lua_getglobal(L, "_world");
    CWorldForCC* w = (CWorldForCC*)lua_touserdata(L, -1);
    DCAST(w->getLayer(), BattleSceneLayer*)->log("%s", log);

    lua_pop(L, 3);  // pop res and "string" and _world

    return 0;
}

int g_createUnit(lua_State* L)
{
    int id = lua_tointeger(L, 1);

    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    CUnit* _p = CUnitLibraryForCC::instance()->copyUnit(id);
    if (_p == nullptr)
    {
        lua_pushnil(L);
        return 1;
    }

    w->addUnit(_p);

    luaL_pushobjptr(L, "Unit", _p);

    return 1;
}

int g_createProjectile(lua_State* L)
{
    int id = lua_tointeger(L, 1);

    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    CProjectile* _p = CUnitLibraryForCC::instance()->copyProjectile(id);
    if (_p == nullptr)
    {
        lua_pushnil(L);
        return 1;
    }

    w->addProjectile(_p);

    luaL_pushobjptr(L, "Projectile", _p);

    return 1;
}

int g_endWithVictory(lua_State* L)
{
    int grade = lua_tointeger(L, 1);

    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world
    DCAST(w->getLayer(), BattleSceneLayer*)->endWithVictory(grade);

    return 0;
}

int g_endWithDefeat(lua_State* L)
{
    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world
    DCAST(w->getLayer(), BattleSceneLayer*)->endWithDefeat();

    return 0;
}

int g_setPortrait(lua_State* L)
{
    int idx = lua_tointeger(L, 1) - 1;
    CUnit* u = luaL_tounitptr(L, 2);

    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    auto l = DCAST(w->getLayer(), BattleSceneLayer*);
    l->m_pp.setPortrait(idx, u, CC_CALLBACK_1(BattleSceneLayer::onClickHeroPortrait, l));

    return 0;
}

int g_delPortrait(lua_State* L)
{
    int idx = lua_tointeger(L, 1);
    bool follow = luaL_tobooleandef(L, 2, false);

    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    auto l = DCAST(w->getLayer(), BattleSceneLayer*);
    l->m_pp.delPortrait(idx, follow);

    return 0;
}

int g_showUnitInfo(lua_State* L)
{
    auto id = luaL_tounitid(L, 1);

    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    auto l = DCAST(w->getLayer(), BattleSceneLayer*);
    l->updateTargetInfo(id);

    return 0;
}

int g_restartStage(lua_State* L)
{
    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    auto l = DCAST(w->getLayer(), BattleSceneLayer*);
    l->onClickRestart(nullptr);

    return 0;
}

int g_setHero(lua_State* L)
{
    auto u = luaL_tounitptr(L, 1);

    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world
    
    w->setHero(u);

    return 0;
}

int g_getHero(lua_State* L)
{
    lua_getglobal(L, "_world");
    auto w = (CBattleWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);  // pop _world

    auto u = w->getHero();
    
    luaL_pushobjptr(L, "Unit", u);

    return 1;
}

int luaRegWorldFuncsForCC(lua_State* L, CWorld* pWorld)
{
    // TODO: reg global vars

    // TODO: reg global world funcs
    lua_register(L, "log", g_log);
    lua_register(L, "createUnit", g_createUnit);
    lua_register(L, "createProjectile", g_createProjectile);
    lua_register(L, "endWithVictory", g_endWithVictory);
    lua_register(L, "endWithDefeat", g_endWithDefeat);
    lua_register(L, "setPortrait", g_setPortrait);
    lua_register(L, "delPortrait", g_delPortrait);
    lua_register(L, "showUnitInfo", g_showUnitInfo);
    lua_register(L, "restartStage", g_restartStage);
    lua_register(L, "setHero", g_setHero);
    lua_register(L, "getHero", g_getHero);

    return 0;
}

int luaL_includefilelog(lua_State* L, const char* file)
{
    int res = 0;

    lua_getglobal(L, "include");
    lua_pushstring(L, file);
    res = lua_pcall(L, 1, 0, 0);
    if (res != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        CCLOG("ERR | LuaErr(%s: %d): %s", __FILE__, (int)__LINE__, err);
        lua_pop(L, 1);

        lua_getglobal(L, "_world");
        auto w = (CBattleWorld*)lua_touserdata(L, -1);
        lua_pop(L, 1);  // pop _world
        if (w != nullptr)
        {
            DCAST(w->getLayer(), BattleSceneLayer*)->log("%s", err);
        }

        return LUA_ERRERR;
    }

    return LUA_OK;
}

int StatusShowPas_ctor(lua_State* L)
{
    CShowStatusPas* _p = new CShowStatusPas();
    //_p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}
