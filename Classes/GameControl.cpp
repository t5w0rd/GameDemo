#include "CommHeader.h"
#include "GameControl.h"
#include "ComponentForCC.h"


// CCameControler
GameControler::GameControler(void)
: m_fc(nullptr)
, m_ac(nullptr)
, m_ae(nullptr)
, m_sceneCreator(nullptr)
{
}

GameControler::~GameControler()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(GameControler::step), this);
}

bool GameControler::init()
{
    m_fc = SpriteFrameCache::getInstance();
    m_ac = AnimationCache::getInstance();
    m_ae = SimpleAudioEngine::getInstance();
    Director::getInstance()->getScheduler()->schedule(schedule_selector(GameControler::step), this, 0.1f, false);

    return true;
}

void GameControler::loadFrames(const char* pPath)
{
    char szName[256];
    sprintf(szName, "%s.plist", pPath);
    m_fc->addSpriteFramesWithFile(szName);
}

Animation* GameControler::loadAnimation(const char* pPath, const char* pName, float fDelay)
{
    SpriteFrame* pSf;
    Animation* pAni = nullptr;
    char sz[256];
    for (int i = 0; ; ++i)
    {
        sprintf(sz, "%s/%02d.png", pPath, i);
        pSf = m_fc->getSpriteFrameByName(sz);
        if (pSf == nullptr)
        {
            if (i == 0)
            {
                return nullptr;
            }
            break;
        }
        if (!pAni)
        {
            pAni = Animation::create();
        }
        pAni->addSpriteFrame(pSf);
    }

    pAni->setDelayPerUnit(fDelay);
    m_ac->addAnimation(pAni, pName);
    return pAni;
}

SpriteFrame* GameControler::getFrame(const char* pName)
{
    char szName[256];
    sprintf(szName, "%s.png", pName);
    return m_fc->getSpriteFrameByName(szName);
}

Animation* GameControler::getAnimation(const char* pName)
{
    return m_ac->getAnimation(pName);
}

void GameControler::step(float dt)
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

void GameControler::preloadSound(const char* sound)
{
    m_ae->preloadEffect(sound);
}

int GameControler::playSound(const char* sound, float duration)
{
    int id = m_ae->playEffect(sound);
    if (duration > 0.0f)
    {
        m_mapSoundDur[id] = duration;
    }

    return id;
}

bool GameControler::isSoundPlaying(int id) const
{
    return m_mapSoundDur.find(id) != m_mapSoundDur.end();
}

void GameControler::stopSound(int id)
{
    m_ae->stopEffect(id);
    m_mapSoundDur.erase(id);
}

void GameControler::preloadMusic(const char* music)
{
    m_ae->preloadBackgroundMusic(music);
}

void GameControler::playMusic(const char* music, bool loop)
{
    m_ae->playBackgroundMusic(music, loop);
}

Layer* GameControler::defaultLoadingLayer()
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    
    TouchMaskLayer* layer = TouchMaskLayer::create(Color4B(0, 0, 0, 0), 255);
    layer->setMaskEnabled(true);
    Sprite* sp = Sprite::create("UI/Loading.png");
    layer->addChild(sp);
    sp->setScale(wsz.width / sp->getContentSize().width * 0.3f);
    sp->setPosition(Point(wsz.width * 0.5, wsz.height * 0.5));

    return layer;
}

void GameControler::replaceSceneWithLoading(SCENE_CREATOR sceneCreator, Layer* loading)
{
    if (loading == nullptr)
    {
        loading = defaultLoadingLayer();
    }

    Director::getInstance()->getRunningScene()->addChild(loading, INT_MAX);

    playSound("sounds/Effects/GUITransitionOpen.mp3");
    m_sceneCreator = sceneCreator;
    loading->runAction(Sequence::createWithTwoActions(FadeIn::create(0.1f), CallFuncN::create(CC_CALLBACK_1(GameControler::onReplaceScene, this))));

    //Director::getInstance()->getTextureCache()()->addImageAsync()
    //SpriteFrameCache::getInstance()->addSpriteFramesWithDictionary(Dictionary::createWithContentsOfFileThreadSafe())
}

void GameControler::onReplaceScene(Node* node)
{
    Scene* sc = (*m_sceneCreator)();
    Director::getInstance()->replaceScene(sc);
}

void GameControler::loadTexturesAsync(const vector<string>& frames, const vector<string>& otherTextures, const FUNC_VOID& onLoadingProgressing, const FUNC_VOID& onLoadingDone)
{
    TextureCache* tc = Director::getInstance()->getTextureCache();
    m_loadFrames = frames;
    m_loadCount = frames.size() + otherTextures.size();
    m_loaded = 0;
    m_onLoadingProgressing = onLoadingProgressing;
    m_onLoadingDone = onLoadingDone;

    M_VEC_FOREACH(frames)
    {
        char sz[1024];
        sprintf(sz, "%s.png", M_VEC_IT->c_str());
        tc->addImageAsync(sz, CC_CALLBACK_1(GameControler::onLoadingProgressing, this));
        M_VEC_NEXT;
    }

    M_VEC_FOREACH(otherTextures)
    {
        char sz[1024];
        sprintf(sz, "%s%s", M_VEC_IT->c_str(), M_VEC_IT->at(M_VEC_IT->length() - 4) == '.' ? "" : ".png");
        tc->addImageAsync(sz, CC_CALLBACK_1(GameControler::onLoadingProgressing, this));
        M_VEC_NEXT;
    }
}

void GameControler::addTexturesLoadedToFramesCache()
{
    M_VEC_FOREACH(m_loadFrames)
    {
        char sz[1024];
        sprintf(sz, "%s.plist", M_VEC_IT->c_str());
        getfc()->addSpriteFramesWithFile(sz);
        M_VEC_NEXT;
    }
}

void GameControler::onLoadingProgressing(Ref* obj)
{
     if (m_loaded < (int)m_loadFrames.size())
     {
         char sz[1024];
         sprintf(sz, "%s.plist", m_loadFrames[m_loaded].c_str());
         getfc()->addSpriteFramesWithFile(sz, DCAST(obj, Texture2D*));
     }

    ++m_loaded;

    if (m_onLoadingProgressing)
    {
        m_onLoadingProgressing();
    }

    if (m_loaded >= m_loadCount)
    {
        // done
        //addTexturesLoadedToFramesCache();

        if (m_onLoadingDone)
        {
            addTexturesLoadedToFramesCache();
            m_onLoadingDone();
        }
        m_loaded = 0;
        m_loadFrames.clear();
        
        return;
    }
}
