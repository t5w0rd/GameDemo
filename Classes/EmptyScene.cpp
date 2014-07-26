#include "CommHeader.h"

#include "EmptyScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "ActionForCC.h"
#include "Ability.h"
#include "Archive.h"
#include "LuaScriptEngine.h"
#include "LuaBinding.h"
#include "LuaBindingForCC.h"
#include "GameData.h"
#include "NetworkForCC.h"
#include "StageScene.h"
#include "Archive.h"


// EmptySceneLayer
EmptySceneLayer::EmptySceneLayer()
{
}

EmptySceneLayer::~EmptySceneLayer()
{
}

Scene* EmptySceneLayer::scene()
{
    // 'scene' is an autorelease object
    Scene* pScene = Scene::create();

    EmptySceneLayer* layer = EmptySceneLayer::create();

    // add layer as a child to scene
    if (layer != NULL)
    {
        M_DEF_GC(gc);
        //gc->preloadSound("sounds/Effects/xxxxxxx.mp3");
        pScene->addChild(layer);
    }

    // return the scene
    return pScene;
}

void tranBlink(Color4B* c, GLushort x, GLushort y, GLushort w, GLushort h)
{
    static int ly = -1;
    static int status = 0;
    static int c1 = 0;

    if (y != ly)
    {
        ly = y;
        status = 0;
        c1 = 0;
    }

    if (c->a == 0)
    {
        if (status == 2 && c1 < 60)
        {
            c->a = c->r = c->g = c->b = 255;
        }

        if (status == 1 || status == 3)
        {
            ++status;
        }
    }
    else
    {
        c->a = c->r = c->g = c->b = 0;
        if (status == 0 || status == 2)
        {
            ++status;
        }

        if (status == 1)
        {
            ++c1;
        }
    }
}

void EmptySceneLayer::lab00()
{
    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);

    auto bp = ButtonPanel::create(4, 1, Size(144, 136), 0, 0, nullptr);
    addChild(bp);
    bp->setPosition(Point(wsz.width * 0.5, wsz.height * 0.5));

    auto sp = Sprite::create("UI/Ability/ArmorUp.png");

    auto orgScale = sp->getScale();
    sp->setScale(0.95f);
    auto txSel = Utils::nodeToTexture(sp);
    sp->setScale(orgScale);

    auto txDis = Utils::nodeToTexture(sp, &Utils::tranGrayscale);

    for (auto i = 0; i < 4; ++i)
    {
        auto btn = ButtonNormal::create(
            Sprite::create("UI/Ability/ArmorUp.png"),
            Sprite::createWithTexture(txSel),
            Sprite::createWithTexture(txDis),
            Sprite::create("UI/Ability/AbilityBlink2.png"),
            nullptr,
            5.0f,
            [](Ref* btn)
        {
            ((ButtonBase*)btn)->coolDown();
        },
            nullptr);
        bp->addButtonEx(btn);
    }
    return;
    auto nd = Node::create();
    nd->setAnchorPoint(Point(0.5f, 0.5f));
    nd->setContentSize(bp->getContentSize());
    nd->setPosition(bp->getPosition() + Point(0, 200));

    for (auto i = 0; i < bp->getMaxCount(); ++i)
    {
        sp = Sprite::create("UI/Ability/AbilityBg.png");
        nd->addChild(sp);
        sp->setPosition(bp->index2Point(i));
    }

    Utils::nodeToFile(nd, "output.png");
}


// on "init" you need to initialize your instance
bool EmptySceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    //if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    if (!LayerColor::initWithColor(Color4B(153, 100, 00, 128)))
    {
        return false;
    }

    M_DEF_GC(gc);
    auto mn = Menu::create();
    addChild(mn);
    mn->setPosition(Point::ZERO);

    // ¶ÁÈ¡ipÅäÖÃ
    char path[256];
    sprintf(path, "%s/%s", CCFileUtils::getInstance()->getWritablePath().c_str(), "ip.cfg");
    CValue* ipcfg = nullptr;
    int gamePort = 0;
    string bcAddr;
    if (CArchive::loadValue(path, ipcfg) == false)
    {
        ipcfg = new CValueMap();
        vms_int(ipcfg, "port", 2888);
        vms_str(ipcfg, "bcAddr", "192.168.1.255");
        CArchive::saveValue(path, ipcfg);
    }

    gamePort = vmg_int(ipcfg, "port");
    bcAddr = vmg_str(ipcfg, "bcAddr");
    delete ipcfg;

    CNetwork::instance()->doNetwork([bcAddr](CNetwork* net)
    {
        net->setBroadCastAddress(bcAddr.c_str());
    });

    auto mi = MenuItemFont::create("test");
    mi->setFontSize(48);  // fuck

    mi = MenuItemFont::create("SinglePlayer", [this, mn](Ref*)
    {
        Director::getInstance()->replaceScene(StageSceneLayer::scene());
    });
    mn->addChild(mi);
    mi->setFontSize(48);
    mi->setPosition(Point(wsz().width * 0.2, wsz().height * 0.8));

    mi = MenuItemFont::create("CreateNetGame", [this, mn, gamePort](Ref*)
    {
        CNetwork::instance()->waitForConnection("0.0.0.0", gamePort, [this, mn](bool res, const string& host, int port)
        {
            Director::getInstance()->replaceScene(StageSceneLayer::scene());
            return;
            char sz[1024];
            sprintf(sz, "Connection(%s:%d)", host.c_str(), port);
            auto mi = MenuItemFont::create(sz, [this](Ref*)
            {
                Director::getInstance()->replaceScene(StageSceneLayer::scene());
            });
            mn->addChild(mi);
            mi->setFontSize(48);
            mi->setPosition(Point(wsz().width * 0.5, wsz().height * 0.5));
        });
    });
    mn->addChild(mi);
    mi->setFontSize(48);
    mi->setPosition(Point(wsz().width * 0.5, wsz().height * 0.8));

    mi = MenuItemFont::create("DiscoverNetGame", [this, mn, gamePort](Ref*)
    {
        //m_mapHost.clear();
        CNetwork::instance()->discoverRemote("0.0.0.0", gamePort, [this, mn](bool res, const string& host, int port)
        {
            auto it = m_mapHost.find(host);
            if (it == m_mapHost.end())
            {
                char sz[1024];
                sprintf(sz, "Game(%s:%d)", host.c_str(), 2888);
                auto mi = MenuItemFont::create(sz, [this, mn, host, port](Ref* ref)
                {
                    CNetwork::instance()->connectToRemote(host.c_str(), 2888, [this, ref](bool res)
                    {
                        CNetwork::instance()->doNetwork([](CNetwork* net)
                        {
                            net->getUdpSock().Close();
                        }, []()
                        {
                            Director::getInstance()->replaceScene(StageSceneLayer::scene());
                        });
                    });
                });
                mn->addChild(mi);
                mi->setFontSize(48);
                mi->setPosition(Point(wsz().width * 0.5, wsz().height * 0.6) - Point(0.0f, mi->getContentSize().height * m_mapHost.size()));
                m_mapHost.insert(make_pair(host, port));
            }
        });
    });
    mn->addChild(mi);
    mi->setFontSize(48);
    mi->setPosition(Point(wsz().width * 0.8, wsz().height * 0.8));

#if 0
    
#else
    
#endif
    return true;
}
