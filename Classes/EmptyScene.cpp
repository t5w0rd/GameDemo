#include "CommHeader.h"

#include "EmptyScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"


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

    auto bp = ButtonPanel::create(4, 1, 144, 136, 0, 0, nullptr);
    addChild(bp);
    bp->setPosition(Point(wsz.width * 0.5, wsz.height * 0.5));

    auto sp = Sprite::create("UI/Ability/Ability00.png");

    auto orgScale = sp->getScale();
    sp->setScale(0.95f);
    auto txSel = Utils::nodeToTexture(sp);
    sp->setScale(orgScale);

    auto txDis = Utils::nodeToTexture(sp, &Utils::tranGrayscale);

    for (auto i = 0; i < 4; ++i)
    {
        auto btn = ButtonNormal::create(
            Sprite::create("UI/Ability/Ability00.png"),
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
    if (!LayerColor::initWithColor(Color4B(190, 217, 84, 255)))
    {
        return false;
    }

    //lab00();
    //return true;
    
    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);

    auto sp = Scale9Sprite::create("UI/TalkBubble2.png");
    addChild(sp);
    sp->setPosition(getAnchorPointInPoints());

    sp->setInsetLeft(53);
    sp->setInsetRight(21);
    sp->setInsetTop(21);
    sp->setInsetBottom(33);

    sp->setContentSize(Size(500, 200));

    auto lbl = Label::createWithBMFont(
        "fonts/Comic Book.fnt",
        //"libjpeg-turbo-1.3.0 Introduction to libjpeg-turbo libjpeg-turbo is a fork of the original IJG libjpeg which uses SIMD to accelerate baseline JPEG");
        "BAAAAAABBBBBB");
    sp->addChild(lbl);

    lbl->setMaxLineWidth(500);
    lbl->setColor(Color3B::BLACK);

    sp->setContentSize(lbl->getContentSize() + Size(50, 50));
    lbl->setPosition(sp->getAnchorPointInPoints() + Point(1, 6));

    return true;
}
