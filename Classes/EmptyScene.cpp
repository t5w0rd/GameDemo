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

    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);

    lab00();

    return true;
}
