/* 
 * File:   ComDef.h
 * Author: thunderliu
 *
 * Created on 2013年12月8日, 下午10:34
 */

#ifndef __COMDEF_H__
#define	__COMDEF_H__


#define DCAST(var, type) dynamic_cast<type>(var)

#define M_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: inline virtual varType get##funName(void) const { return varName; }

#define M_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline virtual varType& get##funName(void) { return varName; }

#define M_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline virtual varType get##funName(void) const { return varName; }\
public: inline virtual void set##funName(varType var){ varName = var; }

#define M_SYNTHESIZE_BOOL(funName)\
protected: bool m_b##funName;\
public: inline virtual bool is##funName(void) const { return m_b##funName; }\
public: inline virtual void set##funName(bool b##funName = true){ m_b##funName = b##funName; }

#define M_SYNTHESIZE_STR(funName)\
protected: string m_s##funName;\
public: inline virtual const char* get##funName(void) const { return m_s##funName.c_str(); }\
public: inline virtual void set##funName(const char* p##funName){ m_s##funName = p##funName; }

#define M_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline virtual varType& get##funName(void) { return varName; }\
public: inline virtual void set##funName(const varType& var){ varName = var; }

#define M_LOGIC_CONSTRUCTOR(sub, super) \
    sub(int iKey): super(iKey) {}

#define M_CREATE_FUNC_PARAM(__TYPE__, __PARAM__, ...) \
    static __TYPE__* create __PARAM__ \
    { \
        __TYPE__ *pRet = new __TYPE__(); \
        if (pRet && pRet->init(__VA_ARGS__)) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
            return NULL; \
        } \
    }

#define M_DECLARE_CREATE_WITH_SPRITE_FRAME_NAME(type) \
    static type* createWithSpriteFrameName(const char *pszSpriteFrameName)

#define M_IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME(type) \
    type* type::createWithSpriteFrameName(const char *pszSpriteFrameName) \
    { \
        type* pSprite = new type(); \
        if (pSprite && pSprite->initWithSpriteFrameName(pszSpriteFrameName)) \
        { \
            pSprite->autorelease(); \
            return pSprite; \
        } \
        CC_SAFE_DELETE(pSprite); \
        return NULL; \
    }

#define M_DECLARE_CREATE_WITH_SPRITE_FRAME_NAME_LOGIC(type) \
    static type* createWithSpriteFrameNameAndLogic(const char *pszSpriteFrameName, CBullet* pBullet)

#define M_IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME_LOGIC(type) \
    type* type::createWithSpriteFrameNameAndLogic(const char *pszSpriteFrameName, CBullet* pBullet) \
    { \
        type* pSprite = new type(); \
        if (pSprite && pSprite->initWithSpriteFrameNameAndLogic(pszSpriteFrameName, pBullet)) \
        { \
            pSprite->autorelease(); \
            return pSprite; \
        } \
        CC_SAFE_DELETE(pSprite); \
        return NULL; \
    }

#define M_DECLARE_CREATE_WITH_SPRITE_FRAME_NAME_GUN(type) \
    static type* createWithSpriteFrameNameAndLogicGun(const char *pszSpriteFrameName, CTank* pTank, CCSprite* pGunSprite)

#define M_IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME_GUN(type) \
    type* type::createWithSpriteFrameNameAndLogicGun(const char *pszSpriteFrameName, CTank* pTank, CCSprite* pGunSprite) \
    { \
        type* pSprite = new type(); \
        if (pSprite && pSprite->initWithSpriteFrameNameAndLogicGun(pszSpriteFrameName, pTank, pGunSprite)) \
        { \
            pSprite->autorelease(); \
            return pSprite; \
        } \
        CC_SAFE_DELETE(pSprite); \
        return NULL; \
    }

#define M_CREATEWITH_FUNC_PARAM(__WITH__, __TYPE__, __PARAM__, ...) \
    static __TYPE__* createWith##__WITH__ __PARAM__ \
    { \
        __TYPE__* pRet = new __TYPE__(); \
        if (pRet && pRet->initWith##__WITH__(__VA_ARGS__)) \
        { \
           pRet->autorelease(); \
           return pRet; \
        } \
        else \
        { \
           delete pRet; \
           pRet = NULL; \
           return NULL; \
        } \
    }

#define M_CREATE_INITWITH_FUNC_PARAM(__WITH__, __TYPE__, __PARAM__, ...) \
    static __TYPE__* create __PARAM__ \
    { \
        __TYPE__* pRet = new __TYPE__(); \
        if (pRet && pRet->initWith##__WITH__(__VA_ARGS__)) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
            return NULL; \
        } \
    }

#define M_PTM_RATIO 32

#define M_SKILL_PATH(name)                                      name "/BTN" name ".png"
#define M_SKILL_PAS_PATH(name)                                  name "/PASBTN" name ".png"
#define M_SKILL_DOWN_PATH(name)                                 name "/BTN" name "DOWN.png"
#define M_SKILL_DIS_PATH(name)                                  name "/DISBTN" name ".png"
#define M_SKILL_ON_PATH(name)                                   name "/BTN" name "ON.png"
#define M_CREATE_SKILL(name, unit, skill, layer)                CCSkillButtonAdvance::create(M_SKILL_PATH(name), M_SKILL_DOWN_PATH(name), M_SKILL_DIS_PATH(name), M_SKILL_PATH("white"), "mask/mask.png", (unit), (skill), (layer))
#define M_CREATE_SKILL_PAS(name, unit, skill, layer)            CCSkillButtonAdvance::create(M_SKILL_PAS_PATH(name), M_SKILL_PAS_PATH(name), M_SKILL_DIS_PATH(name), NULL, NULL, (unit), (skill), (layer))

#define M_RAND_HIT(probability) (rand() % 100 < (int)((probability) * 100))

#define M_GET_TYPE_KEY \
    public: inline virtual int getTypeKey() const\
    { \
        static const int iTypeKey = CGameManager::keygen(); \
        return iTypeKey; \
    }

#define M_SKILL_BUTTON_CANCEL(node) dynamic_cast<CCSkillButtonBase*>((node))->setClickRetCode(-1)

#define M_DEF_DR(var) CCDirector* var = CCDirector::sharedDirector()
#define M_DEF_FU(var) CCFileUtils* var = CCFileUtils::sharedFileUtils()
#define M_DEF_GM(var) CGameManager* var = CGameManager::sharedGameManager()
#define M_DEF_FC(var) CCSpriteFrameCache* var = CCSpriteFrameCache::sharedSpriteFrameCache()
#define M_DEF_AC(var) CCAnimationCache* var = CCAnimationCache::sharedAnimationCache()
#define M_DEF_SM(var) CCSkillManager* var = CCSkillManager::sharedSkillManager()
#define M_DEF_PM(var) CProjectileManager* var = CProjectileManager::sharedProjectileManager()
#define M_DEF_UM(var) CUnitManager* var = CUnitManager::sharedUnitManager()
#define M_DEF_UPM(var) CUnitInfoPatchManager* var = CUnitInfoPatchManager::sharedUnitInfoPatchManager()
#define M_DEF_TB(var) CTowerBuilder* var = CTowerBuilder::sharedTowerBuilder()
#define M_DEF_OU(var) COrgUnitInfo* var = COrgUnitInfo::sharedOrgUnitInfo()
#define M_DEF_OS(var) COrgSkillInfo* var = COrgSkillInfo::sharedOrgSkillInfo()
#define M_DEF_PI(var) CPlayerInfo* var = CPlayerInfo::sharedPlayerInfo()

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

#define CONDITION(func) ((FUNC_UNIT_CONDITION)(&func))

template <typename TYPE, typename ASTYPE = TYPE>
class CInitArray
{
public:
    inline CInitArray(int iCount, ...)
    : m_iCount(iCount)
    {
        m_pData = new TYPE[iCount];
        va_list argv;
        va_start(argv, iCount);
        for (int i = 0; i < iCount; ++i)
        {
            m_pData[i] = (TYPE)va_arg(argv, ASTYPE);
        }
        va_end(argv);
    }

    inline ~CInitArray()
    {
        delete[] m_pData;
    }

    inline operator TYPE*()
    {
        return m_pData;
    }

    inline operator const TYPE*() const
    {
        return m_pData;
    }

    inline int count() const
    {
        return m_iCount;
    }

protected:
    TYPE* m_pData;
    int m_iCount;
};



#ifdef WIN32
const char* GBKToUTF8(const char* pGBKStr);
#define UTEXT(str) GBKToUTF8(str)
#else
#define UTEXT(str) str
#endif

#define M_FIX_BASE_X(width0, width1, delta) ((width0 + width1) * 0.5 + delta)

#define M_BIT_32U(index) ((uint32_t)(1 << index))
#define M_IS_BIT_SET_32U(u32, index) ((uint32_t)(u32) & (uint32_t)(1 << (uint32_t)(index)))

#define M_MAP_FOREACH(mapVar) for (auto it = (mapVar).begin(); it != (mapVar).end(); )
#define M_MAP_EACH (it->second)
#define M_MAP_IT it

// 不得继续引用M_MAP_EACH、M_MAP_IT，必须立即完成本次循环continue/break/return
#define M_MAP_DEL_CUR_NEXT(mapVar) (mapVar).erase(it++)
// 需立即continue
#define M_MAP_NEXT ++it

#define M_VEC_FOREACH(vecVar) for (auto it = (vecVar).begin(); it != (vecVar).end(); )
#define M_VEC_EACH (*it)
#define M_VEC_IT it

// 不得继续引用M_VEC_EACH、M_VEC_IT，必须立即完成本次循环continue/break/return
#define M_VEC_DEL_CUR_NEXT(vecVar) it = (vecVar).erase(it)

// 需立即continue
#define M_VEC_NEXT ++it

#define M_SINGLETON(name) \
    inline static C##name* shared##name() \
    { \
        static C##name* pInst = NULL; \
        if (!pInst) \
        { \
            pInst = new C##name; \
            CMultiRefObject* pObj = DCAST(pInst, CMultiRefObject*); \
            if (pObj) \
            { \
                pObj->retain(); \
            } \
        } \
        \
        return pInst; \
    }

inline int toInt(double fValue)
{
    return (int)((fValue > 0.0 ? 0.5 : -0.5) + fValue);
}


#endif	/* __COMDEF_H__ */

