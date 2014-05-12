/* 
 * File:   MutiRefObject.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午11:37
 */

#ifndef __MULTIREFOBJECT_H__
#define	__MULTIREFOBJECT_H__


class CMultiRefObject
{
public:
    CMultiRefObject();
    virtual ~CMultiRefObject();
    
    void retain();
    void release();
    
    virtual CMultiRefObject* copy();
    
    M_SYNTHESIZE_READONLY(int, m_iRefCount, RefCount);
    M_SYNTHESIZE_STR(DbgClassName);
    virtual const char* getDbgTag() const;
    
    int getId() const;
    
    template <typename PTYPE>
    PTYPE dcast(PTYPE& rTo);
    
private:
    void tryRelease();
    
protected:
    const int CONST_ID;
    
};

typedef void (CMultiRefObject::*FUNC_CALLFUNC_ND)(CMultiRefObject*, void*);
typedef void (CMultiRefObject::*FUNC_CALLFUNC_N)(CMultiRefObject*);

class CAutoReleasePool
{
public:
    typedef set<CMultiRefObject*> SET_OBJS;
    
public:    
    M_SYNTHESIZE_READONLY_PASS_BY_REF(SET_OBJS, m_setObjs, Objects);
    void addObject(CMultiRefObject* pObject);
    void delObject(CMultiRefObject* pObject);
    void releaseObjects();
    
    M_SINGLETON(CAutoReleasePool);
};

class CDbgMultiRefObjectManager
{
public:
    typedef set<CMultiRefObject*> SET_OBJS;
    
public:
    M_SYNTHESIZE_READONLY_PASS_BY_REF(SET_OBJS, m_setObjs, Objects);
    void addObject(CMultiRefObject* pObject);
    void delObject(CMultiRefObject* pObject);
    void printDbgInfo(const char* pFile, int iLine);
#define OBJS_INFO CDbgMultiRefObjectManager::sharedDbgMultiRefObjectManager()->printDbgInfo(__FILE__, __LINE__)
    M_SINGLETON(CDbgMultiRefObjectManager);
};

template <typename MULTIREF_ID_TYPE_PTR>
class CMultiRefMap : public std::map<int, MULTIREF_ID_TYPE_PTR>
{
public:
    typedef typename std::map<int, MULTIREF_ID_TYPE_PTR>::iterator iterator;
    typedef typename std::map<int, MULTIREF_ID_TYPE_PTR>::const_iterator const_iterator;
    
public:
    ~CMultiRefMap();
    
    void addObject(MULTIREF_ID_TYPE_PTR pObj);
    void addObject(int id, MULTIREF_ID_TYPE_PTR pObj);
    void delObject(int id);
    void delObject(iterator it);
    MULTIREF_ID_TYPE_PTR getObject(int id) const;
    MULTIREF_ID_TYPE_PTR getObject(const const_iterator it) const;
    void delAllObjects();
    
};

template <typename MULTIREF_ID_TYPE_PTR>
class CMultiRefVec : public std::vector<MULTIREF_ID_TYPE_PTR>
{
public:
    ~CMultiRefVec();
    
    void addObject(MULTIREF_ID_TYPE_PTR pObj);
    void setObject(int iIndex, MULTIREF_ID_TYPE_PTR pObj);
    void delObject(int iIndex);
    void delAllObjects();
};



// ----------- Inline Implementation--------------

// CMultiRefObject
template <typename PTYPE>
inline PTYPE CMultiRefObject::dcast(PTYPE& rTo)
{
    rTo = DCAST(this, PTYPE);
    return rTo;
}

// CMultiRefMap
template <typename MULTIREF_ID_TYPE_PTR>
inline CMultiRefMap<MULTIREF_ID_TYPE_PTR>::~CMultiRefMap()
{
    delAllObjects();
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefMap<MULTIREF_ID_TYPE_PTR>::addObject(MULTIREF_ID_TYPE_PTR pObj)
{
    this->addObject(pObj->getId(), pObj);
}


template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefMap<MULTIREF_ID_TYPE_PTR>::addObject(int id, MULTIREF_ID_TYPE_PTR pObj)
{
    pObj->retain();
    this->insert(make_pair(id, pObj));
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefMap<MULTIREF_ID_TYPE_PTR>::delObject(int id)
{
    auto it = this->find(id);
    delObject(it);
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefMap<MULTIREF_ID_TYPE_PTR>::delObject(iterator it)
{
    if (it == this->end())
    {
        return;
    }
    
    MULTIREF_ID_TYPE_PTR pDel = it->second;
    this->erase(it);
    pDel->release();
}

template <typename MULTIREF_ID_TYPE_PTR>
inline MULTIREF_ID_TYPE_PTR CMultiRefMap<MULTIREF_ID_TYPE_PTR>::getObject(int id) const
{
    auto it = this->find(id);
    return getObject(it);
}

template <typename MULTIREF_ID_TYPE_PTR>
inline MULTIREF_ID_TYPE_PTR CMultiRefMap<MULTIREF_ID_TYPE_PTR>::getObject(const const_iterator it) const
{
    return it != this->end() ? it->second : nullptr;
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefMap<MULTIREF_ID_TYPE_PTR>::delAllObjects()
{
    M_MAP_FOREACH(*this)
    {
        MULTIREF_ID_TYPE_PTR pObj = M_MAP_EACH;
        M_SAFE_RELEASE(pObj);
        M_MAP_NEXT;
    }
    this->clear();
}

// CMultiRefVec
template <typename MULTIREF_ID_TYPE_PTR>
inline CMultiRefVec<MULTIREF_ID_TYPE_PTR>::~CMultiRefVec()
{
    delAllObjects();
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefVec<MULTIREF_ID_TYPE_PTR>::addObject(MULTIREF_ID_TYPE_PTR pObj)
{
    pObj->retain();
    this->push_back(pObj);
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefVec<MULTIREF_ID_TYPE_PTR>::setObject(int iIndex, MULTIREF_ID_TYPE_PTR pObj)
{
    if (iIndex < 0 || iIndex >= (int)this->size())
    {
        return;
    }
    
    M_SAFE_RETAIN(pObj);
    M_SAFE_RELEASE((*this)[iIndex]);
    (*this)[iIndex] = pObj;
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefVec<MULTIREF_ID_TYPE_PTR>::delObject(int iIndex)
{
    if (iIndex < 0 || iIndex >= (int)this->size())
    {
        return;
    }
    
    if ((*this)[iIndex] == nullptr)
    {
        return;
    }
    
    (*this)[iIndex]->release();
    (*this)[iIndex] = nullptr;
}

template <typename MULTIREF_ID_TYPE_PTR>
inline void CMultiRefVec<MULTIREF_ID_TYPE_PTR>::delAllObjects()
{
    M_VEC_FOREACH(*this)
    {
        MULTIREF_ID_TYPE_PTR pObj = M_VEC_EACH;
        M_SAFE_RELEASE(pObj);
        
        M_MAP_NEXT;
    }
    this->clear();
}

#endif	/* __MUTIREFOBJECT_H__ */

