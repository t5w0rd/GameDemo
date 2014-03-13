#ifndef __GAMECONTROL_H__
#define __GAMECONTROL_H__


#include <cocos2d.h>

class CCGameControler : public cocos2d::CCObject
{
public:
    CCGameControler(void);

    virtual bool init();
    CREATE_FUNC(CCGameControler);

    CC_SINGLETON(GameControler);

    void loadTexture(const char* pPath);
    CCAnimation* loadAnimation(const char* pPath, const char* pName, float fDelay);

    CCSpriteFrame* getFrame(const char* pName);
    CCAnimation* getAnimation(const char* pName);

    M_SYNTHESIZE_READONLY(CCSpriteFrameCache*, m_fc, fc);
    M_SYNTHESIZE_READONLY(CCAnimationCache*, m_ac, ac);
};

typedef std::set<std::string> SET_STR;
class CCSpriteFrameCacheEx : public CCSpriteFrameCache
{
public:
    CCDictionary* getSpriteFrames();
    CCDictionary* getSpriteFramesAliases();
    SET_STR*  getLoadedFileNames();
};


class CCGameFile : public CCObject
{
public:
    enum FILE_ORIGIN
    {
        kBegin = SEEK_SET,
        kCur = SEEK_CUR,
        kEnd = SEEK_END
    };

public:
    CCGameFile(void);
    virtual ~CCGameFile(void);
    virtual bool init(const char* pFileName, const char* pMode);
    M_CREATE_FUNC_PARAM(CCGameFile, (const char* pFileName, const char* pMode), pFileName, pMode);

    template <typename TYPE>
    size_t read(TYPE* pData, size_t uCount = 1);
    size_t tell() const;
    bool eof() const;
    bool seek(long lOffset, FILE_ORIGIN eOrigin);

protected:
    uint8_t* m_pData;
    unsigned long m_uSize;
    unsigned long m_uPos;
};

template <typename TYPE>
size_t CCGameFile::read( TYPE* pData, size_t uCount /*= 1*/ )
{
    size_t uRead;
    TYPE* pCur = (TYPE*)((size_t)m_pData + m_uPos);
    for (uRead = 0; uRead < uCount && m_uPos + (uRead + 1) * sizeof(TYPE) <= m_uSize; ++uRead);
    size_t uReadSize = uRead * sizeof(TYPE);
    memmove(pData, pCur, uReadSize);
    m_uPos += uReadSize;
    return uRead;
}



#endif  /* __GAMECONTROL_H__ */

