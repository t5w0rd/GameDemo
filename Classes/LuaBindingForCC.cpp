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
    
    unsigned long codeBufferSize = 0;
    unsigned char* codeBuffer = CCFileUtils::sharedFileUtils()->getFileData(filename.c_str(), "rb", &codeBufferSize);
    
    if (codeBuffer)
    {
        if (luaL_loadbuffer(L, (char*)codeBuffer, codeBufferSize, filename.c_str()) != 0)
        {
            luaL_error(L, "error loading module %s from file %s :\n\t%s",
                lua_tostring(L, 1), filename.c_str(), lua_tostring(L, -1));
        }
        delete []codeBuffer;
    }
    else
    {
        //CCLog("can not get file data of %s", filename.c_str());
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
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(name);
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

luaL_Reg unit4cc_funcs[] = {
    {"ctor", unit4cc_ctor},
    {"addBattleTip", unit4cc_addBattleTip},
    {NULL, NULL}
};

int unit4cc_ctor(lua_State* L)
{
    int sprite = 2;
    const char* name = lua_tostring(L, 3);

    CUnitDrawForCC* d = luaL_tospriteptr(L, sprite);
    CUnit* u = new CUnit(d->getName());
    u->setDraw(d);
    u->setName(name);

    lua_pushlightuserdata(L, u);
    lua_setfield(L, 1, "_p");

    lua_getglobal(L, "_world");
    CWorld* w = (CWorld*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    w->addUnit(u);
    u->setAI(CDefaultAI());

    return 0;
}
int unit4cc_addBattleTip(lua_State* L)
{
    CUnit* _p = luaL_tounitptr(L);
    const char* tip = lua_tostring(L, 2);
    const char* font = lua_tostring(L, 3);
    float fontSize = lua_tonumber(L, 4);
    unsigned int r = lua_tounsigned(L, 5);
    unsigned int g = lua_tounsigned(L, 6);
    unsigned int b = lua_tounsigned(L, 7);
    
    CUnitDrawForCC* d = DCAST(_p->getDraw(), CUnitDrawForCC*);
    d->addBattleTip(tip, font, fontSize, ccc3(r, g, b));
    
    return 0;
}

luaL_Reg UnitPath4CC_funcs[] = {
    {"addPoints", UnitPath4CC_addPoints},
    {NULL, NULL}
};

int UnitPath4CC_addPoints(lua_State* L)
{
    CUnitPathForCC* _p = NULL;
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

luaL_Reg sprite4cc_funcs[] = {
    {"ctor", sprite4cc_ctor},
    {"prepareFrame", sprite4cc_prepareFrame},
    {"prepareAnimation", sprite4cc_prepareAnimation},
    {"setGeometry", sprite4cc_setGeometry},
    {NULL, NULL}
};

int sprite4cc_ctor(lua_State* L)
{
    const char* name = lua_tostring(L, 2);

    CUnitDrawForCC* d = new CUnitDrawForCC(name);
    lua_pushlightuserdata(L, d);
    lua_setfield(L, 1, "_p");

    return 0;
}

int sprite4cc_prepareFrame(lua_State* L)
{
    int id = lua_tointeger(L, 2);
    const char* name = lua_tostring(L, 3);
    
    CUnitDrawForCC* d = luaL_tospriteptr(L);
    d->prepareFrame((CUnitDraw2D::FRM_ID)id, name);

    return 0;
}

int sprite4cc_prepareAnimation(lua_State* L)
{
    int id = lua_tointeger(L, 2);
    const char* name = lua_tostring(L, 3);
    int notify = lua_tointeger(L, 4);

    CUnitDrawForCC* d = luaL_tospriteptr(L);
    d->prepareAnimation((CUnitDraw2D::ANI_ID)id, name, notify);

    return 0;
}

int sprite4cc_setGeometry(lua_State* L)
{
    float fHalfOfWidth = lua_tonumber(L, 2);
    float fHalfOfHeight = lua_tonumber(L, 3);
    CCPoint anchorPoint(lua_tonumber(L, 4), lua_tonumber(L, 5));
    CPoint firePoint(lua_tonumber(L, 6), lua_tonumber(L, 7));

    CUnitDrawForCC* d = luaL_tospriteptr(L);
    d->setGeometry(fHalfOfWidth, fHalfOfHeight, anchorPoint, firePoint);

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
    DCAST(w->getLayer(), CCBattleSceneLayer*)->log("%s", log);
    
    lua_pop(L, 3);  // pop res and "string" and _world
    
    return 0;
}

int g_loadTexture(lua_State* L)
{
    const char* name = lua_tostring(L, 1);

    M_DEF_GC(gc);
    gc->loadTexture(name);

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

int g_createUnit( lua_State* L )
{
    int id = lua_tointeger(L, 1);

    lua_getglobal(L, "_world");
    CBattleWorld* w = (CBattleWorld*)lua_touserdata(L, -1);
    CUnit* _p = w->m_oULib.copyUnit(id);
    w->addUnit(_p);
    lua_pop(L, 1);  // pop _world

    luaL_pushobjptr(L, "Unit", _p);

    return 1;
}

int g_showDebug( lua_State* L )
{
    bool bOn = lua_toboolean(L, 1) != 0;
    
    CCDirector::sharedDirector()->setDisplayStats(bOn);

    return 0;
}

int g_playEffect(lua_State* L)
{
    const char* eff = lua_tostring(L, 1);

    SimpleAudioEngine::sharedEngine()->playEffect(eff);

    return 0;
}

int luaRegWorldFuncsForCC(lua_State* L, CWorld* pWorld)
{
    // TODO: reg global vars

    // TODO: reg global funcs
    lua_register(L, "log", g_log);
    lua_register(L, "loadTexture", g_loadTexture);
    lua_register(L, "loadAnimation", g_loadAnimation);
    lua_register(L, "createUnit", g_createUnit);
    lua_register(L, "showDebug", g_showDebug);
    lua_register(L, "playEffect", g_playEffect);
    
    // TODO: reg global class members
    lua_getglobal(L, "Unit");
    luaL_setfuncs(L, unit4cc_funcs, 0);
    lua_pop(L, 1);

    lua_getglobal(L, "UnitPath");
    luaL_setfuncs(L, UnitPath4CC_funcs, 0);
    lua_pop(L, 1);

    lua_getglobal(L, "class");
    lua_call(L, 0, 1);  // ret a class
    int sprite4cc = lua_gettop(L);
    luaL_setfuncs(L, sprite4cc_funcs, 0);
    lua_pushinteger(L, CUnitDraw::kAniMove);
    lua_setfield(L, sprite4cc, "kAniMove");
    lua_pushinteger(L, CUnitDraw::kAniDie);
    lua_setfield(L, sprite4cc, "kAniDie");
    lua_pushinteger(L, CUnitDraw::kAniAct1);
    lua_setfield(L, sprite4cc, "kAniAct1");
    lua_pushinteger(L, CUnitDraw::kAniAct2);
    lua_setfield(L, sprite4cc, "kAniAct2");
    lua_pushinteger(L, CUnitDraw::kAniAct3);
    lua_setfield(L, sprite4cc, "kAniAct3");
    lua_pushinteger(L, CUnitDraw::kAniAct4);
    lua_setfield(L, sprite4cc, "kAniAct4");
    lua_pushinteger(L, CUnitDraw::kAniAct5);
    lua_setfield(L, sprite4cc, "kAniAct5");
    lua_pushinteger(L, CUnitDraw::kAniAct6);
    lua_setfield(L, sprite4cc, "kAniAct6");
    lua_pushinteger(L, CUnitDraw::kAniAct7);
    lua_setfield(L, sprite4cc, "kAniAct7");
    lua_pushinteger(L, CUnitDraw::kAniAct8);
    lua_setfield(L, sprite4cc, "kAniAct8");
    lua_pushinteger(L, CUnitDraw::kFrmDefault);
    lua_setfield(L, sprite4cc, "kFrmDefault");
    lua_setglobal(L, "Sprite");

    int t = 0;

    // bind a class
    M_LUA_BIND_CLASS_EX(L, StatusShowPas, PassiveAbility);

    return 0;
}

int StatusShowPas_ctor(lua_State* L)
{
    CStatusShowPas* _p = new CStatusShowPas();
    //_p->setScriptHandler(luaL_setregistry(L, 1));
    lua_pushlightuserdata(L, _p);
    lua_setfield(L, 1, "_p");

    return 0;
}
