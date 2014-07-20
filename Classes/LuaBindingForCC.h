#ifndef __LUABINDINGFORCC_H__
#define __LUABINDINGFORCC_H__


class CUnitDrawForCC;

int luaModuleLoader4cc(lua_State *L);
bool luaL_loadscript4cc(lua_State *L, const char* name, string& err);

int g_loadFrames(lua_State* L);
int g_loadAnimation(lua_State* L);
int g_showDebug(lua_State* L);
int g_playSound(lua_State* L);
int g_isSoundPlaying(lua_State* L);
int g_setSearchPath(lua_State* L);
int g_include(lua_State* L);
int g_addTemplateUnit(lua_State* L);
int g_addTemplateProjectile(lua_State* L);
int g_addAbilityToUserData(lua_State* L);
int g_getWritablePath(lua_State* L);
int g_isFileExist(lua_State* L);
int g_setGameSpeed(lua_State* L);

int Unit4CC_ctor(lua_State* L);
int Unit4CC_addBattleTip(lua_State* L);
int Unit4CC_setGeometry(lua_State* L);
int Unit4CC_getAnchorPointInPoints(lua_State* L);
int Unit4CC_addCtrlSound(lua_State* L);
int Unit4CC_doRotateBy(lua_State* L);

int Projectile4CC_ctor(lua_State* L);

int UnitPath4CC_addPoints(lua_State* L);

CUnitDrawForCC* luaL_tospriteptr(lua_State* L, int idx = 1);
int Sprite_ctor(lua_State* L);
int Sprite_prepareFrame(lua_State* L);
int Sprite_prepareAnimation(lua_State* L);

int Effect_ctor(lua_State* L);
int Effect_addAnimation(lua_State* L);
int Effect_addAnimationSound(lua_State* L);
int Effect_play(lua_State* L);
int Effect_playRelease(lua_State* L);
int Effect_playForever(lua_State* L);
int Effect_stop(lua_State* L);
int Effect_setLogicPositionMode(lua_State* L);
int Effect_isLogicPositionMode(lua_State* L);
int Effect_setLogicPosition(lua_State* L);
int Effect_getLogicPosition(lua_State* L);
int Effect_setLogicHeight(lua_State* L);
int Effect_getLogicHeight(lua_State* L);
int Effect_setPosition(lua_State* L);
int Effect_getPosition(lua_State* L);
int Effect_setAnchorPoint(lua_State* L);
int Effect_getAnchorPoint(lua_State* L);
int Effect_release(lua_State* L);

int luaRegCommFuncsForCC(lua_State* L);

int luaL_includefile(lua_State *L, const char* file);

int g_log(lua_State* L);
int g_createUnit(lua_State* L);
int g_createProjectile(lua_State* L);
int g_endWithVictory(lua_State* L);
int g_endWithDefeat(lua_State* L);
int g_saveUserData(lua_State* L);
int g_setPortrait(lua_State* L);
int g_delPortrait(lua_State* L);
int g_showUnitInfo(lua_State* L);
int g_restartStage(lua_State* L);

int luaRegWorldFuncsForCC(lua_State* L, CWorld* pWorld);

int luaL_includefilelog(lua_State *L, const char* file);

int StatusShowPas_ctor(lua_State* L);


#endif  /* __LUABINDINGFORCC_H__ */
