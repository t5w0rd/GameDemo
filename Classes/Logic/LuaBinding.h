/* 
 * File:   LuaBinding.h
 * Author: thunderliu
 *
 * Created on 2014年3月2日, 上午11:48
 */

#ifndef __LUABINDING_H__
#define	__LUABINDING_H__


// common
int lua_getcopy(lua_State* L, int idx);

int obj_sctor(lua_State* L);
int obj_ctor(lua_State* L);
int obj_init(lua_State* L);
int class_type_new(lua_State* L);
int g_class(lua_State* L);

int luaRegCommFunc(lua_State* L);

class CUnit;
class CAbility;
class CWorld;

// game
#define M_LUA_BIND_CLASS(l, name) \
    do \
    { \
        lua_getglobal((l), "class"); \
        lua_call((l), 0, 1); \
        lua_pushcfunction((l), name##_init); \
        lua_setfield((l), -2, "init"); \
        lua_setglobal((l), #name); \
    } while (false)

template <typename PTYPE>
PTYPE getObjPtr(PTYPE& ptr, lua_State* L, int idx = 1);

CUnit* getUnitPtr(lua_State* L, int idx = 1);
CAbility* getAbilityPtr(lua_State* L, int idx);

int unit_ctor(lua_State* L);
int unit_init(lua_State* L);
int unit_setMaxHp(lua_State* L);
int unit_setHp(lua_State* L);
int unit_setForceByIndex(lua_State* L);
int unit_setAlly(lua_State* L);
int unit_setBaseArmor(lua_State* L);

int unit_addActiveAbility(lua_State* L);
int unit_addPassiveAbility(lua_State* L);
int unit_addBuffAbility(lua_State* L);

int uint2d_setBaseMoveSpeed(lua_State* L);
int unit2d_setPosition(lua_State* L);

int g_addUnit(lua_State* L);
int g_setControlUnit(lua_State* L);

int luaRegWorldFuncs(lua_State* L, CWorld* pWorld);

int AttackAct_init(lua_State* L);




///////////////////////////////// Inline //////////////////////////////////////
template <typename PTYPE>
PTYPE getObjPtr( PTYPE& ptr, lua_State* L, int idx /*= 1*/ )
{
    lua_getfield(L, idx, "_p");
    PTYPE* ret = (PTYPE*)lua_touserdata(L, lua_gettop(L));
    lua_pop(L, 1);

    return ret;
}


#endif	/* __LUABINDING_H__ */

