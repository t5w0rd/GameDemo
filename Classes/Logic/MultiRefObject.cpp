/* 
 * File:   MutiRefObject.cpp
 * Author: thunderliu
 * 
 * Created on 2013年12月8日, 下午11:37
 */

#include "CommInc.h"
#include "MultiRefObject.h"
#include "Base.h"


// CMultiRefObject
CMultiRefObject::CMultiRefObject()
: m_iRefCount(0)
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
