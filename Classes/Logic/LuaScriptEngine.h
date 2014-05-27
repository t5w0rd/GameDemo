#ifndef __LUASCRIPTENGINE_H__
#define __LUASCRIPTENGINE_H__

#include "MultiRefObject.h"


class CLuaScriptEngine : public CMultiRefObject
{
public:
    CLuaScriptEngine();
    ~CLuaScriptEngine();

    M_SINGLETON(CLuaScriptEngine);

    M_SYNTHESIZE_READONLY(lua_State*, m_L, LuaHandle);

};


#endif
