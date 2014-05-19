/* 
 * File:   Level.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午10:31
 */

#include "CommInc.h"
#include "Level.h"
#include "Unit.h"
#include "LuaBinding.h"


// CLevelUpdate
CLevelUpdate::CLevelUpdate()
: m_iScriptHandler(0)
{
    setDbgClassName("CLevelUpdate");
}

CLevelUpdate::~CLevelUpdate()
{
    if (getScriptHandler() != 0)
    {
        lua_State* L = CWorld::getLuaHandle();
        luaL_unref(L, LUA_REGISTRYINDEX, getScriptHandler());
    }
}

CLevelUpdate* CLevelUpdate::copy()
{
    this->retain();
    return this;
}

void CLevelUpdate::updateExpRange(CLevelExp* pLevel)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CWorld::getLuaHandle();
    int lu = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, lu, "updateExpRange");
    lua_pushvalue(L, lu);
    luaL_pushobjptr(L, "LevelExp", pLevel);
    if (lua_pcall(L, 2, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'lu'
}

void CLevelUpdate::onChangeLevel(CLevelExp* pLevel, int iChanged)
{
    if (getScriptHandler() == 0)
    {
        return;
    }

    lua_State* L = CWorld::getLuaHandle();
    int lu = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, lu, "onChangeLevel");
    lua_pushvalue(L, lu);
    luaL_pushobjptr(L, "LevelExp", pLevel);
    lua_pushinteger(L, iChanged);
    if (lua_pcall(L, 3, 0, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop 'lu'
}

int CLevelUpdate::calcExp(int iLevel)
{
    if (getScriptHandler() == 0)
    {
        return iLevel;
    }

    lua_State* L = CWorld::getLuaHandle();
    int le = luaL_getregistery(L, getScriptHandler());

    lua_getfield(L, le, "calcExp");
    lua_pushvalue(L, le);
    lua_pushinteger(L, iLevel);

    int res = iLevel;
    if (lua_pcall(L, 2, 1, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    else
    {
        res = lua_tointeger(L, -1);
    }

    lua_pop(L, 1);  // pop 'lu'

    return res;
}

// CLevelExp
CLevelExp::CLevelExp()
: m_iLvl(1)
, m_iMaxLvl(1)
, m_iExp(0)
, m_iBaseExp(0)
, m_iMaxExp(1)
, m_pUpdate(nullptr)
{
    setDbgClassName("CLevelExp");
}

CLevelExp::~CLevelExp()
{
    M_SAFE_RELEASE(m_pUpdate);
}

CLevelExp* CLevelExp::copy()
{
    CLevelExp* le = new CLevelExp;
    le->m_iLvl = m_iLvl;
    le->m_iMaxLvl = m_iMaxLvl;
    le->m_iExp = m_iExp;
    le->m_iBaseExp = m_iBaseExp;
    le->m_iMaxExp = m_iMaxExp;
    le->m_pUpdate = m_pUpdate ? m_pUpdate->copy() : nullptr;

    return le;
}

void CLevelExp::updateExpRange()
{
    if (m_pUpdate)
    {
        m_pUpdate->updateExpRange(this);
    }
    else
    {
        m_iBaseExp = m_iExp;
        m_iMaxExp = m_iExp * 1.5;
    }
}

void CLevelExp::onChangeLevel(int32_t iChanged)
{
}

void CLevelExp::addLevel(int iLvl)
{
    setLevel(m_iLvl + iLvl);
}

void CLevelExp::addExp(int iExp)
{
    if (m_iLvl == m_iMaxLvl)
    {
        return;
    }

    m_iExp += iExp;
    while (m_iExp >= m_iMaxExp && m_iLvl < m_iMaxLvl)
    {
        ++m_iLvl;
        updateExpRange();
        if (m_pUpdate)
        {
            m_pUpdate->onChangeLevel(this, 1);
        }
        onChangeLevel(1);
    }

    if (m_iLvl == m_iMaxLvl)
    {
        m_iExp = m_iBaseExp;
    }
}

void CLevelExp::setExpRange(int iFrom, int iTo)
{
    setBaseExp(iFrom);
    setMaxExp(iTo);
}

void CLevelExp::setLevel(int iLvl)
{
    int iOldLvl = m_iLvl;

    if (iLvl >= m_iMaxLvl)
    {
        iLvl = m_iMaxLvl;
    }
    else if (iLvl < 1)
    {
        iLvl = 1;
    }

    m_iLvl = iLvl;

    int32_t iChanged = m_iLvl - iOldLvl;

    if (iChanged)
    {
        if (m_iLvl == m_iMaxLvl)
        {
            m_iExp = 0;
        }
        if (m_pUpdate)
        {
            m_pUpdate->onChangeLevel(this, iChanged);
        }
        onChangeLevel(iChanged);
        updateExpRange();
    }
}

void CLevelExp::setMaxLevel(int iMaxLvl)
{
    iMaxLvl <= 0 && (iMaxLvl = 1);
    m_iMaxLvl = iMaxLvl;
    setLevel(m_iLvl);
}

void CLevelExp::setLevelUpdate(CLevelUpdate* pUpdate)
{
    if (pUpdate == m_pUpdate)
    {
        return;
    }

    M_SAFE_RETAIN(pUpdate);
    M_SAFE_RELEASE(m_pUpdate);
    m_pUpdate = pUpdate;
}

bool CLevelExp::canIncreaseExp() const
{
    return m_iLvl < m_iMaxLvl;
}
