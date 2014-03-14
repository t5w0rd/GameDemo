#include "CommHeader.h"
#include "GameControl.h"


CCGameControler::CCGameControler(void)
{

}

bool CCGameControler::init()
{
    m_fc = CCSpriteFrameCache::sharedSpriteFrameCache();
    m_ac = CCAnimationCache::sharedAnimationCache();

    return true;
}

void CCGameControler::loadTexture(const char* pPath)
{
    char szName[256];
    sprintf(szName, "%s.plist", pPath);
    m_fc->addSpriteFramesWithFile(szName);
}

CCAnimation* CCGameControler::loadAnimation(const char* pPath, const char* pName, float fDelay)
{
    CCSpriteFrame* pSf;
    CCAnimation* pAni = NULL;
    char sz[256];
    for (int i = 0; ; ++i)
    {
        sprintf(sz, "%s/%02d.png", pPath, i);
        pSf = m_fc->spriteFrameByName(sz);
        if (pSf == NULL)
        {
            if (i == 0)
            {
                return NULL;
            }
            break;
        }
        if (!pAni)
        {
            pAni = CCAnimation::create();
        }
        pAni->addSpriteFrame(pSf);
    }

    pAni->setDelayPerUnit(fDelay);
    m_ac->addAnimation(pAni, pName);
    return pAni;
}

CCSpriteFrame* CCGameControler::getFrame(const char* pName)
{
    char szName[256];
    sprintf(szName, "%s.png", pName);
    return m_fc->spriteFrameByName(szName);
}

CCAnimation* CCGameControler::getAnimation(const char* pName)
{
    return m_ac->animationByName(pName);
}

// CCSpriteFrameCacheEx
CCDictionary* CCSpriteFrameCacheEx::getSpriteFrames()
{
    return m_pSpriteFrames;
}

CCDictionary* CCSpriteFrameCacheEx::getSpriteFramesAliases()
{
    return m_pSpriteFramesAliases;
}

SET_STR* CCSpriteFrameCacheEx::getLoadedFileNames()
{
    return m_pLoadedFileNames;
}

// CCGameFile
CCGameFile::CCGameFile(void)
{
}


CCGameFile::~CCGameFile(void)
{
}

bool CCGameFile::init( const char* pFileName, const char* pMode )
{
    M_DEF_FU(pFu);
    m_uPos = m_uSize = 0;
    m_pData = pFu->getFileData(pFu->fullPathForFilename(pFileName).c_str(), pMode, &m_uSize);
    if (!m_pData)
    {
        return false;
    }
    return true;
}

size_t CCGameFile::tell() const
{
    return m_uPos;
}

bool CCGameFile::eof() const
{
    return m_uPos >= m_uSize;
}

bool CCGameFile::seek( long lOffset, FILE_ORIGIN eOrigin )
{
    unsigned long uPos = 0;
    switch (eOrigin)
    {
    case kBegin:
        uPos = lOffset;
        break;

    case kCur:
        uPos = m_uPos + lOffset;
        break;

    case kEnd:
        uPos = m_uSize + lOffset;
        break;

    default:
        return false;
    }

    if (uPos < 0 || uPos > m_uSize)
    {
        return false;
    }

    m_uPos = uPos;
    return true;
}
