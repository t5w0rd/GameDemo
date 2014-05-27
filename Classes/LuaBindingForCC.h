#ifndef __LUABINDINGFORCC_H__
#define __LUABINDINGFORCC_H__


class CUnitDrawForCC;

int luaModuleLoader4cc(lua_State *L);
bool luaL_loadscript4cc(lua_State *L, const char* name, string& err);

int g_loadFrames(lua_State* L);
int g_loadAnimation(lua_State* L);
int g_showDebug(lua_State* L);
int g_playSound(lua_State* L);
int g_setSearchPath(lua_State* L);
int g_include(lua_State* L);
int g_addTemplateUnit(lua_State* L);
int g_addTemplateProjectile(lua_State* L);

int Unit4CC_ctor(lua_State* L);
int Unit4CC_addBattleTip(lua_State* L);
int Unit4CC_setGeometry(lua_State* L);

int Projectile4CC_ctor(lua_State* L);

int UnitPath4CC_addPoints(lua_State* L);

CUnitDrawForCC* luaL_tospriteptr(lua_State* L, int idx = 1);
int Sprite_ctor(lua_State* L);
int Sprite_prepareFrame(lua_State* L);
int Sprite_prepareAnimation(lua_State* L);

int luaRegCommFuncForCC(lua_State* L);

int luaL_includefile(lua_State *L, const char* file);

int g_log(lua_State* L);
int g_createUnit(lua_State* L);
int g_createProjectile(lua_State* L);
int g_endWithVictory(lua_State* L);
int g_endWithDefeat(lua_State* L);
int g_save(lua_State* L);

int luaRegWorldFuncsForCC(lua_State* L, CWorld* pWorld);

int luaL_includefilelog(lua_State *L, const char* file);

int StatusShowPas_ctor(lua_State* L);


#endif  /* __LUABINDINGFORCC_H__ */
