#include "CommHeader.h"
#include "GameControl.h"
#include "ComponentForCC.h"


// CCameControler
CCGameControler::CCGameControler(void)
    : m_fc(NULL)
    , m_ac(NULL)
    , m_ae(NULL)
    , m_sceneCreator(NULL)
{
}

bool CCGameControler::init()
{
    m_fc = CCSpriteFrameCache::sharedSpriteFrameCache();
    m_ac = CCAnimationCache::sharedAnimationCache();

    return true;
}

void CCGameControler::loadFrames(const char* pPath)
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
    M_MAP_FOREACH(m_mapSoundDur)
    {
        if (it->second - dt <= 0.0f)
        {
            M_MAP_DEL_CUR_NEXT(m_mapSoundDur);
        }
        else
        {
            it->second -= dt;
            M_MAP_NEXT;
        }
    }
}

void CCGameControler::preloadSound(const char* sound)
{
    m_ae->preloadEffect(sound);
}

int CCGameControler::playSound(const char* sound, float duration)
{
    int id = m_ae->playEffect(sound);
    if (duration > 0.0f)
    {
        m_mapSoundDur[id] = duration;
    }

    return id;
}

bool CCGameControler::isSoundPlaying( int id ) const
{
    return m_mapSoundDur.find(id) != m_mapSoundDur.end();
}

void CCGameControler::stopSound( int id )
{
    m_ae->stopEffect(id);
    m_mapSoundDur.erase(id);
}

void CCGameControler::preloadMusic( const char* music )
{
    m_ae->preloadBackgroundMusic(music);
}

void CCGameControler::playMusic( const char* music, bool loop )
{
    m_ae->playBackgroundMusic(music, loop);
}

CCLayer* CCGameControler::defaultLoadingLayer()
{
    static CCSize wsz = CCDirector::sharedDirector()->getVisibleSize();
    
    CCTouchMaskLayer* layer = CCTouchMaskLayer::create(ccc4(0, 0, 0, 255));
    CCSprite* sp = CCSprite::create("UI/Loading.png");
    layer->addChild(sp);
    sp->setScale(wsz.width / sp->getContentSize().width * 0.3f);
    sp->setPosition(ccp(wsz.width * 0.5, wsz.height * 0.5));

    return layer;
}

void CCGameControler::replaceSceneWithLoading( SCENE_CREATOR sceneCreator, CCLayer* loading )
{
    if (loading == NULL)
    {
        loading = defaultLoadingLayer();
    }

    CCDirector::sharedDirector()->getRunningScene()->addChild(loading, INT_MAX);

    playSound("sounds/Effect/GUITransitionOpen.mp3");
    m_sceneCreator = sceneCreator;
    loading->runAction(CCSequence::createWithTwoActions(CCFadeIn::create(0.1f), CCCallFuncN::create(this, callfuncN_selector(CCGameControler::onReplaceScene))));

    //CCTextureCache::sharedTextureCache()->addImageAsync()
    //CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithDictionary(CCDictionary::createWithContentsOfFileThreadSafe())
}

void CCGameControler::onReplaceScene( CCNode* node )
{
    CCScene* sc = (*m_sceneCreator)();
    CCDirector::sharedDirector()->replaceScene(sc);
}

void CCGameControler::loadTexturesAsync( const vector<string>& frames, const vector<string>& otherTextures, CCObject* target, SEL_CallFuncO loadingProgressing, SEL_CallFuncO loadingDone )
{
    CCTextureCache* tc = CCTextureCache::sharedTextureCache();
    m_loadFrames = frames;
    m_loadCount = frames.size() + otherTextures.size();
    m_loaded = 0;
    m_loadingTarget = target;
    m_loadingProgressing = loadingProgressing;
    m_loadingDone = loadingDone;

    M_VEC_FOREACH(frames)
    {
        char sz[1024];
        sprintf(sz, "%s.png", M_VEC_IT->c_str());
        tc->addImageAsync(sz, this, callfuncO_selector(CCGameControler::onLoadingProgressing));
        M_VEC_NEXT;
    }

    M_VEC_FOREACH(otherTextures)
    {
        char sz[1024];
        sprintf(sz, "%s%s", M_VEC_IT->c_str(), M_VEC_IT->at(M_VEC_IT->length() - 4) == '.' ? "" : ".png");
        tc->addImageAsync(sz, this, callfuncO_selector(CCGameControler::onLoadingProgressing));
        M_VEC_NEXT;
    }
}

void CCGameControler::onLoadingProgressing( CCObject* obj )
{
    ++m_loaded;

    if (m_loadingTarget != NULL && m_loadingProgressing != NULL)
    {
        (m_loadingTarget->*m_loadingProgressing)(obj);
    }

    if (m_loaded >= m_loadCount)
    {
        // done
        M_VEC_FOREACH(m_loadFrames)
        {
            char sz[1024];
            sprintf(sz, "%s.plist", M_VEC_IT->c_str());
            getfc()->addSpriteFramesWithFile(sz);
            M_VEC_NEXT;
        }

        if (m_loadingTarget != NULL && m_loadingProgressing != NULL)
        {
            (m_loadingTarget->*m_loadingDone)(obj);
        }
        
        return;
    }
}
