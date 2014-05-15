#include "CommHeader.h"

#include "EmptyScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "ActionForCC.h"
#include "Ability.h"
#include "utils/strnormalize.h"


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

class AbilityItem : public Node
{
CC_CONSTRUCTOR_ACCESS:
    AbilityItem();

public:
    bool initWithAbility(CAbility* ability);
    M_CREATE_INITWITH_FUNC_PARAM(Ability, AbilityItem, (CAbility* ability), ability);

protected:
    Sprite* m_aib;
};

// AbilityItem
AbilityItem::AbilityItem()
{
}

bool AbilityItem::initWithAbility(CAbility* ability)
{
    auto aibg = Sprite::create("UI/Ability/AbilityItemBackground.png");
    setContentSize(aibg->getContentSize());
    setAnchorPoint(Point(0.5f, 0.5f));

    addChild(aibg);
    aibg->setPosition(getAnchorPointInPoints());

    auto aib = Sprite::create("UI/Ability/AbilityItemBorder.png");
    aibg->addChild(aib, 1);
    aib->setPosition(aibg->getAnchorPointInPoints());

    auto aii = Sprite::create(ability->getImageName());
    aibg->addChild(aii, 1);
    aii->setPosition(Point(96, 85));

    char sz[1024];
    auto ainame = Label::createWithTTF(gbk_to_utf8(ability->getName(), sz), "fonts/DFYuanW7-GB2312.ttf", 28);
    aibg->addChild(ainame, 2);
    ainame->setPosition(Point(301, 116));
    ainame->setColor(Color3B::BLACK);

    auto ainame2 = Label::createWithTTF(ainame->getTTFConfig(), ainame->getString());
    aibg->addChild(ainame2, 3);
    ainame2->setPosition(ainame->getPosition() + Point(-2, 2));

    Color3B color;
    switch (ability->getGrade())
    {
    case CAbility::kNormal:
        color = Color3B::WHITE;
        break;

    case CAbility::kRare:
        color = Color3B(0, 60, 255);
        break;

    case CAbility::kEpic:
        color = Color3B(180, 0, 255);
        break;

    case CAbility::kLegend:
        color = Color3B(255, 90, 0);
        break;

    default:
        return false;
    }
    aib->setColor(color);

    auto aicost = Sprite::create("UI/Ability/AbilityItemCost.png");
    aibg->addChild(aicost, 1);
    //aicost->setPosition(Point(45, 132));
    aicost->setPosition(Point(45, 48));

    sprintf(sz, "%d", ability->getCost());
    auto aicost2 = Label::createWithTTF(sz, "fonts/Comic Book.ttf", 28);
    aicost->addChild(aicost2, 1);
    aicost2->setPosition(aicost->getAnchorPointInPoints() + Point(-1, -2));
    aicost2->setColor(Color3B(78, 43, 7));

    const Point aistarCenter(299, 60);
    const float aistarBetween = 5;
    auto aistar = Sprite::createWithSpriteFrameName(ability->getLevel() > 0 ? "UI/Ability/AbilityItemStar.png" : "UI/Ability/AbilityItemUnstar.png");
    float starBaseWidth = aistarCenter.x - (ability->getMaxLevel() - 1) * aistar->getContentSize().width * 0.5 - (ability->getMaxLevel() - 1) * aistarBetween * 0.5;
    aibg->addChild(aistar, 1, 100);
    aistar->setPosition(Point(starBaseWidth, aistarCenter.y));
    for (auto i = 1; i < ability->getMaxLevel(); ++i)
    {
        aistar = Sprite::createWithSpriteFrameName(ability->getLevel() > i ? "UI/Ability/AbilityItemStar.png" : "UI/Ability/AbilityItemUnstar.png");
        aibg->addChild(aistar, 1, 100 + i);
        aistar->setPosition(Point(starBaseWidth + i * (aistar->getContentSize().width + aistarBetween), aistarCenter.y));
    }

    return true;
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

    gc->loadFrames("Global0");
    gc->loadFrames("Global1");

    auto a = new CBuffMakerAct("BMA", "À×öªÒ»»÷", 5.0f, CCommandTarget::kNoTarget, CUnitForce::kEnemy, 1.0f, 0);
    a->setImageName("UI/Ability/ThunderCap.png");
    a->setMaxLevel(3);
    a->setLevel(3);
    a->setGrade(CAbility::kEpic);
    a->setCost(9);
    auto ai = AbilityItem::create(a);

    addChild(ai);
    ai->setPosition(getAnchorPointInPoints());

    a = new CBuffMakerAct("BMA", "ÒýÁ¦äöÎÐ", 5.0f, CCommandTarget::kNoTarget, CUnitForce::kEnemy, 1.0f, 0);
    a->setImageName("UI/Ability/GravitySurf.png");
    a->setMaxLevel(2);
    a->setLevel(1);
    a->setGrade(CAbility::kRare);
    a->setCost(2);
    auto ai2 = AbilityItem::create(a);

    addChild(ai2);
    ai2->setPosition(ai->getPosition() + Point(0, -ai->getContentSize().height));

    a = new CBuffMakerAct("BMA", "×çÖä", 5.0f, CCommandTarget::kNoTarget, CUnitForce::kEnemy, 1.0f, 0);
    a->setImageName("UI/Ability/AbilityCurse.png");
    a->setMaxLevel(3);
    a->setLevel(0);
    a->setGrade(CAbility::kNormal);
    a->setCost(1);
    auto ai3 = AbilityItem::create(a);

    addChild(ai3);
    ai3->setPosition(ai->getPosition() + Point(0, ai->getContentSize().height));

    return true;
}
