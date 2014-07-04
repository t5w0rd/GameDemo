/* 
 * File:   MutiRefObject.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午11:37
 */

#include "CommInc.h"
#include "MultiRefObject.h"
#include "Base.h"
#include "LuaScriptEngine.h"
#include "LuaBinding.h"


// CMultiRefObject
CMultiRefObject::CMultiRefObject()
: m_iRefCount(0)
, m_iScriptHandler(0)
, CONST_ID(CIdGen::nextId())
{
    setDbgClassName("CMutiRefObject");
    CDbgMultiRefObjectManager::instance()->addObject(this);
    
    CAutoReleasePool::instance()->addObject(this);
    //printf("DBG | %s(), Global(%d)\n", getDbgClassName(), m_iGlobal);
}

CMultiRefObject::~CMultiRefObject()
{
    assert(!m_iRefCount);

    if (getScriptHandler() != 0)
    {
        lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();
        luaL_unref(L, LUA_REGISTRYINDEX, getScriptHandler());
    }
    
    CDbgMultiRefObjectManager::instance()->delObject(this);
    //printf("DBG | ~%s(), Global(%d)\n", getDbgClassName(), m_iGlobal);
}

void CMultiRefObject::retain()
{
    CAutoReleasePool::instance()->delObject(this);
    ++m_iRefCount;
}

void CMultiRefObject::release()
{
    assert(m_iRefCount);
    --m_iRefCount;
    if (!m_iRefCount)
    {
        delete this;
    }
}

void CMultiRefObject::tryRelease()
{
    if (!m_iRefCount)
    {
        delete this;
    }
}

void CMultiRefObject::copyScriptHandler(int iScriptHandler)
{
    if (iScriptHandler == 0)
    {
        return;
    }

    lua_State* L = CLuaScriptEngine::instance()->getLuaHandle();

    // copy source lua obj
    lua_getglobal(L, "table");
    lua_getfield(L, -1, "copy");
    luaL_getregistery(L, iScriptHandler);
    if (lua_pcall(L, 1, 1, 0) != LUA_OK)
    {
        const char* err = lua_tostring(L, -1);
        lua_getglobal(L, "log");
        lua_pushvalue(L, -2);
        lua_call(L, 1, 0);
        lua_pop(L, 1);
    }
    else
    {
        lua_pushlightuserdata(L, this);
        lua_setfield(L, -2, "_p");
        setScriptHandler(luaL_ref(L, LUA_REGISTRYINDEX));
    }

    lua_pop(L, 1);  // pop "table"
}

CMultiRefObject* CMultiRefObject::copy()
{
    return new CMultiRefObject;
}

const char* CMultiRefObject::getDbgTag() const
{
    return "";
}

int CMultiRefObject::getId() const
{
    return CONST_ID;
}

// CAutoReleasePool
void CAutoReleasePool::addObject(CMultiRefObject* pObject)
{
    m_setObjs.insert(pObject);
}

void CAutoReleasePool::delObject(CMultiRefObject* pObject)
{
    m_setObjs.erase(pObject);
}

void CAutoReleasePool::releaseObjects()
{
    for (auto& obj : m_setObjs)
    {
        assert(obj->getRefCount() == 0);
        delete obj;
    }
    
    m_setObjs.clear();
}

// CDbgMutiRefObjectManager
void CDbgMultiRefObjectManager::addObject(CMultiRefObject* pObject)
{
    m_setObjs.insert(pObject);
}

void CDbgMultiRefObjectManager::delObject(CMultiRefObject* pObject)
{
    m_setObjs.erase(pObject);
}

void CDbgMultiRefObjectManager::printDbgInfo(const char* pFile, int iLine)
{
    fprintf(stdout, "ObjectName(Tag)(RefCount) {\n");
    for (auto& obj : m_setObjs)
    {
        fprintf(stdout, "    %s(%s)(%d)\n", obj->getDbgClassName(), obj->getDbgTag(), obj->getRefCount());
    }
    fprintf(stdout, "} NumberOfObjects(%zu), %s: %d\n", m_setObjs.size(), pFile, iLine);
}
