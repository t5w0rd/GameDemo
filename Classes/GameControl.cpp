#include "CommHeader.h"
#include "GameControl.h"


// CCameControler
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

void CCGameControler::step( float dt )
{
    M_MAP_FOREACH(m_mapEffectDur)
    {
        if (it->second - dt <= 0.0f)
        {
            M_MAP_DEL_CUR_NEXT(m_mapEffectDur);
        }
        else
        {
            it->second -= dt;
            M_MAP_NEXT;
        }
    }
}

int CCGameControler::playEffect(const char* effect, float duration)
{
    int id = m_ae->playEffect(effect);
    if (duration > 0.0f)
    {
        m_mapEffectDur[id] = duration;
    }

    return id;
}

bool CCGameControler::isEffectPlaying( int id ) const
{
    return m_mapEffectDur.find(id) != m_mapEffectDur.end();
}

void CCGameControler::stopEffect( int id )
{
    m_ae->stopEffect(id);
    m_mapEffectDur.erase(id);
}

void CCGameControler::playMusic( const char* music )
{
    m_ae->playBackgroundMusic(music);
}
