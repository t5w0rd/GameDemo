#include "CommInc.h"
#include "LuaScriptEngine.h"
#include "LuaBinding.h"


// CLuaScriptEngine
CLuaScriptEngine::CLuaScriptEngine()
: m_L(nullptr)
{
    setDbgClassName("CLuaScriptEngine");
    m_L = luaL_newstate();
    luaL_openlibs(m_L);
}

CLuaScriptEngine::~CLuaScriptEngine()
{
    lua_close(m_L);
}
