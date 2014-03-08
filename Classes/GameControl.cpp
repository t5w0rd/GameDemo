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
    // loadAnimation("Units/Malik/move", "/Malik/move")
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
