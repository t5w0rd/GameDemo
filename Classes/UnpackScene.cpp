#include "CommHeader.h"

#include "UnpackScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"


// UnpackScene
UnpackScene::UnpackScene()
{
}

UnpackScene::~UnpackScene()
{
}

bool UnpackScene::init()
{
    return Scene::init();
}


// UnpackSceneLayer
UnpackSceneLayer::UnpackSceneLayer()
: m_fc(nullptr)
, m_sp(nullptr)
{
}

UnpackSceneLayer::~UnpackSceneLayer()
{
    CC_SAFE_RELEASE(m_fc);
}

Scene* UnpackSceneLayer::scene()
{
    // 'scene' is an autorelease object
    UnpackScene* pScene = UnpackScene::create();

    UnpackSceneLayer* layer = UnpackSceneLayer::create();

    // add layer as a child to scene
    if (layer != nullptr)
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
bool UnpackSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(64, 64, 64, 255)))
    {
        return false;
    }

    //Director::getInstance()->setDisplayStats(false);

    FILE* out = stdout;

    Size sz = Director::getInstance()->getVisibleSize();
    fprintf(out, "%d x %d\n", (int)sz.width, (int)sz.height);

    Menu* mn = Menu::create();
    addChild(mn);
    mn->setPosition(Point::ZERO);
    
    MenuItemLabel* lbl = MenuItemLabel::create(Label::createWithTTF("Prev", "fonts/Arial.ttf", 64), CC_CALLBACK_1(UnpackSceneLayer::onLabelPrev, this));
    mn->addChild(lbl);
    lbl->setPosition(Point(sz.width * 0.3, sz.height * 0.95));
    lbl = MenuItemLabel::create(Label::createWithTTF("Next", "fonts/Arial.ttf", 64), CC_CALLBACK_1(UnpackSceneLayer::onLabelNext, this));
    mn->addChild(lbl);
    lbl->setPosition(Point(sz.width * 0.7, sz.height * 0.95));
    lbl = MenuItemLabel::create(Label::createWithTTF("SaveAll", "fonts/Arial.ttf", 64), CC_CALLBACK_1(UnpackSceneLayer::onLabelSave, this));
    mn->addChild(lbl);
    lbl->setPosition(Point(sz.width * 0.5, sz.height * 0.95));

    m_fc = new SpriteFrameCacheEx;
    m_fc->init();
    m_fc->autorelease();
    CC_SAFE_RETAIN(m_fc);
    //m_fc->addSpriteFramesWithFile("Global.plist");
    m_fc->addSpriteFramesWithFile(PLIST_FILE_NAME);

    fprintf(out, "LoadedFileNames:\n");
    std::set<std::string>* set = m_fc->getLoadedFileNames();
    for (auto it = set->begin(); it != set->end(); ++it)
    {
        fprintf(out, "%s\n", it->c_str());
    }
    fprintf(out, "\n");

    Map<std::string, SpriteFrame*>* d = m_fc->getSpriteFrames();
    if (d->size() == 0)
    {
        return false;
    }
    m_arr = d->keys();
    fprintf(out, "Keys(%d):\n", m_arr.size());
    /*
    for (unsigned int i = 0; i < m_arr->count(); ++i)
    {
        CCString* s = DCAST(m_arr->objectAtIndex(i), CCString*);
        fprintf(out, "%s\n", s->getCString());
    }
    */
    fprintf(out, "\n");
    
    m_sp = Sprite::create();
    addChild(m_sp);
    
    m_cur = 0;
    updateSprite();

    return true;
}

void UnpackSceneLayer::onLabelPrev(Ref*)
{
    if (m_cur > 0)
    {
        --m_cur;
    }
    else
    {
        m_cur = m_arr.size() - 1;
    }

    updateSprite();
}

void UnpackSceneLayer::onLabelNext(Ref*)
{
    if (m_cur < m_arr.size() - 1)
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
    CreateDirectoryA(name, nullptr);
#else
#include <sys/stat.h>
    mkdir(name, S_IRWXU | S_IRWXG | S_IRWXO);
#endif

}

int preparePath(const char* name)
{
    char sz[128];
    for (const char* p = strchr(name, '/'); p != nullptr; p = strchr(p + 1, '/'))
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

void UnpackSceneLayer::updateSprite()
{
    string& s = m_arr[m_cur];
    m_sp->setSpriteFrame(m_fc->getSpriteFrameByName(s));
    Size sz = m_sp->getContentSize();
    m_sp->setPosition(Point(sz.width * 0.5, sz.height * 0.5));
}

void UnpackSceneLayer::onLabelSave(Ref*)
{
    vector<string> files;
    GetAllFileName(FileUtils::getInstance()->fullPathForFilename("Unpack").c_str(), files);
    
    for_each(files.begin(), files.end(), [this](string& file)
    {
        char tmp[256];
        SpriteFrameCacheEx fc;
        fc.init();

        sprintf(tmp, "Unpack/%s.plist", file.c_str());
        fc.addSpriteFramesWithFile(tmp);

        Map<std::string, SpriteFrame*>* d = fc.getSpriteFrames();
        CCLOG("%s has %d Frame(s)", file.c_str(), d->size());
        int f = 1;
        for (auto it = d->begin(); it != d->end(); ++it)
        {
            const char* name = it->first.c_str();
            sprintf(tmp, "Unpack/%s", file.c_str());
            CCLOG("%02d%% Saving(%d/%d) %s ..", f * 100 / d->size(), f, d->size(), name);
            this->saveToPng(name, tmp, &fc);
            ++f;
        }
        CCLOG("%d Frame(s) Unpacked", (int)d->size());
    });
    CCLOG("Done. %d File(s) Unpacked", (int)files.size());

    m_cur = 0;
}
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")
#endif
bool UnpackSceneLayer::saveToPng(const char* name, const char* path, SpriteFrameCache* fc)
{
    m_sp->setSpriteFrame(fc->getSpriteFrameByName(name));
    //Size sz = m_sp->getContentSize();
    //m_sp->setPosition(Point(sz.width * 0.5, sz.height * 0.5));
    //RenderTexture rt;
    //rt.initWithWidthAndHeight(sz.width, sz.height, Texture2D::PixelFormat::RGBA8888);

    //rt.begin();
    //m_sp->visit();
    //rt.end();

    char full[256];
    if (path != nullptr)
    {
        sprintf(full, "%s/%s", path, name);
    }
    else
    {
        strcpy(full, name);
    }
    
    preparePath(full);
    bool res = Utils::nodeToFile(m_sp, full);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    static HANDLE h = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(h, &pmc, sizeof(pmc));
    if (pmc.WorkingSetSize > 0x20000000)
    {
        // 512MB
        PoolManager::getInstance()->getCurrentPool()->clear();
    }
#else
    static int rc = 0;
    if (rc < 10)
    {
        ++rc;
    }
    else
    {
        PoolManager::getInstance()->getCurrentPool()->clear();
        rc = 0;
    }
#endif-
    return res;
}
