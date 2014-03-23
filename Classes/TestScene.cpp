#include "CommHeader.h"

#include "TestScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"


// CCTestScene
CCTestScene::CCTestScene()
{
}

CCTestScene::~CCTestScene()
{
}

bool CCTestScene::init()
{
    return CCScene::init();
}


// CCTestSceneLayer
CCTestSceneLayer::CCTestSceneLayer()
    : m_fc(NULL)
    , m_arr(NULL)
    , m_sp(NULL)
{
}

CCTestSceneLayer::~CCTestSceneLayer()
{
    CC_SAFE_RELEASE(m_fc);
    CC_SAFE_RELEASE(m_arr);
}

CCScene* CCTestSceneLayer::scene()
{
    // 'scene' is an autorelease object
    CCTestScene* pScene = CCTestScene::create();

    CCTestSceneLayer* layer = CCTestSceneLayer::create();

    // add layer as a child to scene
    if (layer != NULL)
    {
        pScene->addChild(layer);
    }

    // return the scene
    return pScene;
}

#define PLIST_NAME "Global0"
#define PLIST_FILE_NAME ("Global0.plist")
#define PLIST_SAVE_NAME ("Unpack/Global0")

// on "init" you need to initialize your instance
bool CCTestSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!CCLayerColor::initWithColor(ccc4(64, 64, 64, 255)))
    {
        return false;
    }

    //CCDirector::sharedDirector()->setDisplayStats(false);

    FILE* out = stdout;

    CCSize sz = CCDirector::sharedDirector()->getVisibleSize();
    fprintf(out, "%d x %d\n", (int)sz.width, (int)sz.height);

    CCMenu* mn = CCMenu::create();
    addChild(mn);
    mn->setPosition(CCPointZero);
    
    CCMenuItemLabel* lbl = CCMenuItemLabel::create(CCLabelTTF::create("Prev", "", 32), this, menu_selector(CCTestSceneLayer::onLabelPrev));
    mn->addChild(lbl);
    lbl->setPosition(ccp(sz.width * 0.3, sz.height * 0.95));
    lbl = CCMenuItemLabel::create(CCLabelTTF::create("Next", "", 32), this, menu_selector(CCTestSceneLayer::onLabelNext));
    mn->addChild(lbl);
    lbl->setPosition(ccp(sz.width * 0.7, sz.height * 0.95));
    lbl = CCMenuItemLabel::create(CCLabelTTF::create("SaveAll", "", 32), this, menu_selector(CCTestSceneLayer::onLabelSave));
    mn->addChild(lbl);
    lbl->setPosition(ccp(sz.width * 0.5, sz.height * 0.95));

    m_fc = new CCSpriteFrameCacheEx;
    m_fc->init();
    m_fc->autorelease();
    CC_SAFE_RETAIN(m_fc);
    //m_fc->addSpriteFramesWithFile("Global.plist");
    m_fc->addSpriteFramesWithFile(PLIST_FILE_NAME);

    fprintf(out, "LoadedFileNames:\n");
    SET_STR* set = m_fc->getLoadedFileNames();
    for (auto it = set->begin(); it != set->end(); ++it)
    {
        fprintf(out, "%s\n", it->c_str());
    }
    fprintf(out, "\n");

    CCDictionary* d = m_fc->getSpriteFrames();
    if (d->count() == 0)
    {
        return false;
    }
    m_arr = d->allKeys();
    CC_SAFE_RETAIN(m_arr);
    fprintf(out, "Keys(%d):\n", m_arr->count());
    /*
    for (unsigned int i = 0; i < m_arr->count(); ++i)
    {
        CCString* s = DCAST(m_arr->objectAtIndex(i), CCString*);
        fprintf(out, "%s\n", s->getCString());
    }
    */
    fprintf(out, "\n");
    
    m_sp = CCSprite::create();
    addChild(m_sp);
    
    m_cur = 0;
    updateSprite();

    return true;
}

void CCTestSceneLayer::onLabelPrev( CCObject* )
{
    if (m_cur > 0)
    {
        --m_cur;
    }
    else
    {
        m_cur = m_arr->count() - 1;
    }

    updateSprite();
}

void CCTestSceneLayer::onLabelNext( CCObject* )
{
    if (m_cur < m_arr->count() - 1)
    {
        ++m_cur;
    }
    else
    {
        m_cur = 0;
    }

    updateSprite();
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
void GetAllFileName(const char* path, vector<string>& rFileNames)
{
    char szFind[1024];
    sprintf(szFind, "%s\\*.plist", path);
    WIN32_FIND_DATAA wfd;
    HANDLE h = FindFirstFileA(szFind, &wfd);
    for (BOOL res = (h != INVALID_HANDLE_VALUE); res; res = FindNextFileA(h, &wfd))
    {
        string f(wfd.cFileName);
        rFileNames.push_back(f.substr(0, f.rfind('.')));
        CCLOG("Add %s to Saving List", rFileNames.back().c_str());
    }
}
#endif

void MakeDir(const char* name)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    CreateDirectoryA(name, NULL);
#else
#include <sys/stat.h>
    mkdir(name, S_IRWXU | S_IRWXG | S_IRWXO);
#endif

}

int preparePath(const char* name)
{
    char sz[128];
    for (const char* p = strchr(name, '/'); p != NULL; p = strchr(p + 1, '/'))
    {
        size_t len = p - name;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
        if (len == 2 && *(p - 1) == ':')
#else
        if (len == 0)
#endif
        {
            continue;
        }
        
        memmove(sz, name, len);
        sz[len] = 0;
        MakeDir(sz);
    }

    return 0;
}

void CCTestSceneLayer::updateSprite()
{
    CCString* s = DCAST(m_arr->objectAtIndex(m_cur), CCString*);
    m_sp->setDisplayFrame(m_fc->spriteFrameByName(s->getCString()));
    CCSize sz = m_sp->getContentSize();
    m_sp->setPosition(ccp(sz.width * 0.5, sz.height * 0.5));
}

void CCTestSceneLayer::onLabelSave( CCObject* )
{
    vector<string> files;
    GetAllFileName(CCFileUtils::sharedFileUtils()->fullPathForFilename("Unpack").c_str(), files);
    
    for (auto it = files.begin(); it != files.end(); ++it)
    {
        char tmp[256];
        CCSpriteFrameCacheEx fc;
        fc.init();

        sprintf(tmp, "Unpack/%s.plist", it->c_str());
        fc.addSpriteFramesWithFile(tmp);

        CCDictionary* d = fc.getSpriteFrames();
        CCLOG("%s has %d Frame(s)", it->c_str(), d->count());
        CCDictElement* e = NULL;
        int f = 1;
        CCDICT_FOREACH(d, e)
        {
            const char* name = e->getStrKey();
            sprintf(tmp, "Unpack/%s", it->c_str());
            CCLOG("%02d%% Saving(%d/%d) %s ..", f * 100 / d->count(), f, d->count(), name);
            saveToPng(name, tmp, &fc);
            ++f;
        }
        CCLOG("%d Frame(s) Unpacked", d->count());
    }

    m_cur = 0;
}
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")
#endif
bool CCTestSceneLayer::saveToPng( const char* name, const char* path, CCSpriteFrameCache* fc )
{
    m_sp->setDisplayFrame(fc->spriteFrameByName(name));
    CCSize sz = m_sp->getContentSize();
    m_sp->setPosition(ccp(sz.width * 0.5, sz.height * 0.5));
    CCRenderTexture rt;
    rt.initWithWidthAndHeight(sz.width, sz.height, kTexture2DPixelFormat_RGBA8888);

    rt.begin();
    m_sp->visit();
    rt.end();

    char full[256];
    if (path != NULL)
    {
        sprintf(full, "%s/%s", path, name);
    }
    else
    {
        strcpy(full, name);
    }
    
    preparePath(full);
    CCImage* img = rt.newCCImage();
    bool res = img->saveToFile(full, false);
    delete img;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    static HANDLE h = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(h, &pmc, sizeof(pmc));
    if (pmc.WorkingSetSize > 0x20000000)
    {
        // 512MB
        CCPoolManager::sharedPoolManager()->pop();
    }
#else
    static int rc = 0;
    if (rc < 10)
    {
        ++rc;
    }
    else
    {
        CCPoolManager::sharedPoolManager()->pop();
        rc = 0;
    }
#endif
    return res;
}
