#ifndef __LUABINDINGFORCC_H__
#define __LUABINDINGFORCC_H__


class CUnitDrawForCC;

int unit4cc_ctor(lua_State* L);

CUnitDrawForCC* getSpritePtr(lua_State* L, int idx = 1);
int sprite4cc_ctor(lua_State* L);
int sprite4cc_prepareFrame(lua_State* L);
int sprite4cc_prepareAnimation(lua_State* L);
int sprite4cc_setGeometry(lua_State* L);

int g_loadTexture(lua_State* L);
int g_loadAnimation(lua_State* L);


int luaRegWorldFuncsForCC(lua_State* L, CWorld* pWorld);

int StatusShowPas_ctor(lua_State* L);


#endif  /* __LUABINDINGFORCC_H__ */