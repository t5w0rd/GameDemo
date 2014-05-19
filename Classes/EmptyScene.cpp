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

    auto bp = ButtonPanel::create(4, 1, Size(144, 136), 0, 0, nullptr);
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

class WinFormPanel : public WinLayer
{
public:
    WinFormPanel();
    virtual ~WinFormPanel();
    bool init(int iRow, int iColumn, int iWinRow, int iWinColumn, const Size& rNodeSize, float fHorInnerBorderWidth, float fVerInnerBorderWidth, float fHorBorderWidth = 0.0f, float fVerBorderWidth = 0.0f, Node* pBackground = nullptr, float fBackgroundOffsetX = 0.0f, float fBackgroundOffsetY = 0.0f);
    M_CREATE_FUNC_PARAM(WinFormPanel, (int iRow, int iColumn, int iWinRow, int iWinColumn, const Size& rNodeSize, float fHorInnerBorderWidth, float fVerInnerBorderWidth, float fHorBorderWidth = 0.0f, float fVerBorderWidth = 0.0f, Node* pBackground = nullptr, float fBackgroundOffsetX = 0.0f, float fBackgroundOffsetY = 0.0f), iRow, iColumn, iWinRow, iWinColumn, rNodeSize, fHorInnerBorderWidth, fVerInnerBorderWidth, fHorBorderWidth, fVerBorderWidth, pBackground, fBackgroundOffsetX, fBackgroundOffsetY);

    void addNode(Node* pNode, int iIndex); // org
    void addNode(Node* pNode, int iX, int iY);

    void delNode(int iIndex); // org
    void delNode(int iX, int iY);
    void delNode(Node* pNode);

    enum ADD_HORIZONTAL
    {
        kLeftToRight,
        kRightToLeft
    };

    enum ADD_VERTICAL
    {
        kBottomToTop,
        kTopToBottom
    };

    void moveNode(int iIndexSrc, int iIndexDst); // org

    void addNodeEx(Node* pNode, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    int allotSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);
    int allotSlot(int iStartX, int iStartY, int iEndX, int iEndY, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor);
    void clearUpSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    Node* getNode(int iX, int iY) const;
    Node* getNode(int iIndex) const;
    Node* getNode(const function<bool(Node*)>& match) const;

    int getNodeIndex(Node* pNode) const;

    int index2Y(int iIndex) const;
    int index2X(int iIndex) const;
    int toIndex(int iX, int iY) const;
    Point index2Point(int iIndex);

    M_SYNTHESIZE(int, m_iRow, Row);
    M_SYNTHESIZE(int, m_iColumn, Column);
    M_SYNTHESIZE_READONLY_PASS_BY_REF(Size, m_oNodeSize, NodeSize);
    M_SYNTHESIZE(float, m_fHorInnerBorderWidth, HorInnerBorderWidth);
    M_SYNTHESIZE(float, m_fVerInnerBorderWidth, VerInnerBorderWidth);
    M_SYNTHESIZE_READONLY(Node*, m_pBackground, Background);
    void setBackground(Node* pBackground, float fBackgroundOffsetX, float fBackgroundOffsetY);

    M_SYNTHESIZE_READONLY(int, m_iCount, Count);

    M_SYNTHESIZE(float, m_fHorBorderWidth, HorBorderWidth);
    M_SYNTHESIZE(float, m_fVerBorderWidth, VerBorderWidth);

protected:
    Node** m_ppNodes;
};

// WinFormPanel
WinFormPanel::WinFormPanel()
: m_iRow(0)
, m_iColumn(0)
, m_fHorInnerBorderWidth(0.0f)
, m_fVerInnerBorderWidth(0.0f)
, m_pBackground(nullptr)
, m_iCount(0)
, m_ppNodes(nullptr)
, m_fHorBorderWidth(0.0f)
, m_fVerBorderWidth(0.0f)
{
}

WinFormPanel::~WinFormPanel()
{
    if (m_ppNodes)
    {
        delete[] m_ppNodes;
    }
}

bool WinFormPanel::init(int iRow, int iColumn, int iWinRow, int iWinColumn, const Size& rNodeSize, float fHorInnerBorderWidth, float fVerInnerBorderWidth, float fHorBorderWidth, float fVerBorderWidth, Node* pBackground, float fBackgroundOffsetX, float fBackgroundOffsetY)
{
    if (WinLayer::init() == false)
    {
        return false;
    }

    m_iRow = iRow;
    m_iColumn = iColumn;
    m_oNodeSize = rNodeSize;
    m_fHorInnerBorderWidth = fHorInnerBorderWidth;
    m_fVerInnerBorderWidth = fVerInnerBorderWidth;
    m_fHorBorderWidth = fHorBorderWidth;
    m_fVerBorderWidth = fVerBorderWidth;

    setAnchorPoint(Point(0.5f, 0.5f));
    setContentSize(Size(fHorInnerBorderWidth * (iColumn - 1) + rNodeSize.width * iColumn + fHorBorderWidth * 2, fVerInnerBorderWidth * (iRow - 1) + rNodeSize.height * iRow + fVerBorderWidth * 2));
    setWinSize(Size(fHorInnerBorderWidth * (iWinColumn - 1) + rNodeSize.width * iWinColumn, fVerInnerBorderWidth * (iWinRow - 1) + rNodeSize.height * iWinRow));

    if (pBackground)
    {
        m_pBackground = pBackground;
        addChild(m_pBackground);
        m_pBackground->setPosition(getAnchorPointInPoints() + Point(fBackgroundOffsetX, fBackgroundOffsetY));
    }
    else
    {
        m_pBackground = nullptr;
    }

    if (m_ppNodes)
    {
        delete[] m_ppNodes;
    }

    size_t uCount = iRow * iColumn;
    m_ppNodes = new Node*[uCount];
    memset(m_ppNodes, 0, sizeof(Node*)* uCount);

    m_iCount = 0;

    setCascadeOpacityEnabled(true);

    return true;
}

void WinFormPanel::addNode(Node* pNode, int iIndex)
{
    CCAssert(iIndex < m_iRow * m_iColumn, "Break Bounds");
    CCAssert(m_iCount <= m_iRow * m_iColumn, "already full");
    Node* pBtn = getNode(iIndex);
    if (pBtn)
    {
        delNode(iIndex);
    }

    addChild(pNode);
    pNode->setPosition(index2Point(iIndex));
    //pNode->setNodeIndex(iIndex);

    m_ppNodes[iIndex] = pNode;

    ++m_iCount;
}

void WinFormPanel::addNode(Node* pNode, int iX, int iY)
{
    CCAssert(iY < m_iRow && iX < m_iColumn, "Break Bounds");
    addNode(pNode, toIndex(iX, iY));
}

void WinFormPanel::delNode(int iIndex)
{
    --m_iCount;
    removeChild(m_ppNodes[iIndex], true);
    m_ppNodes[iIndex] = nullptr;
}

void WinFormPanel::delNode(Node* pNode)
{
    int iIndex = getNodeIndex(pNode);
    CCAssert(iIndex >= 0, "button not found");
    delNode(iIndex);
}

void WinFormPanel::delNode(int iX, int iY)
{
    CCAssert(iY < m_iRow && iX < m_iColumn, "Break Bounds");
    delNode(toIndex(iX, iY));
}

void WinFormPanel::moveNode(int iIndexSrc, int iIndexDst)
{
    Node* pSrc = getNode(iIndexSrc);
    m_ppNodes[iIndexDst] = pSrc;
    m_ppNodes[iIndexSrc] = nullptr;
    pSrc->setPosition(index2Point(iIndexDst));
}

void WinFormPanel::addNodeEx(Node* pNode, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/)
{
    addNode(pNode, allotSlot(eVer, eHor));
}

int WinFormPanel::allotSlot(ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/)
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iStartY = bY ? 0 : (m_iRow - 1);
    int iStartX = bX ? 0 : (m_iColumn - 1);

    int iEndY = bY ? (m_iRow - 1) : 0;
    int iEndX = bX ? (m_iColumn - 1) : 0;

    return allotSlot(iStartX, iStartY, iEndX, iEndY, eVer, eHor);
}

int WinFormPanel::allotSlot(int iStartX, int iStartY, int iEndX, int iEndY, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor)
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iY = bY ? 1 : -1;
    int iX = bX ? 1 : -1;

    int iIndex;
    for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
    {
        int iStartX0 = (y == iStartY ? iStartX : (bX ? 0 : (m_iColumn - 1)));
        int iEndX0 = (y == iEndY ? iEndX : (bX ? (m_iColumn - 1) : 0));
        for (int x = iStartX0; bX ? (x <= iEndX0) : (x >= iEndX0); x += iX)
        {
            iIndex = toIndex(x, y);
            if (!getNode(iIndex))
            {
                return iIndex;
            }
        }
    }

    return -1;
}

void WinFormPanel::clearUpSlot(ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/)
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iY = bY ? 1 : -1;
    int iX = bX ? 1 : -1;

    int iStartY = bY ? 0 : (m_iRow - 1);
    int iEndY = bY ? (m_iRow - 1) : 0;

    int iStartX = bX ? 0 : (m_iColumn - 1);
    int iEndX = bX ? (m_iColumn - 1) : 0;

    int iEmpty = -1;
    int iIndex;
    for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
    {
        for (int x = iStartX; bX ? (x <= iEndX) : (x >= iEndX); x += iX)
        {
            iIndex = toIndex(x, y);
            if (getNode(iIndex))
            {
                if (iEmpty >= 0)
                {
                    // 遇到按钮后且前面有空槽，移动按钮
                    moveNode(iIndex, iEmpty);
                    // 在前面找一块新空槽
                    iEmpty = allotSlot(index2X(iEmpty), index2Y(iEmpty), x, y, eVer, eHor);
                }
            }
            else
            {
                if (iEmpty < 0)
                {
                    // 首次遇到空槽
                    iEmpty = iIndex;
                }
            }
        }
    }
}

Node* WinFormPanel::getNode(int iX, int iY) const
{
    return getNode(toIndex(iX, iY));
}

Node* WinFormPanel::getNode(int iIndex) const
{
    CCAssert(iIndex < m_iRow * m_iColumn, "Break Bounds");
    return m_ppNodes[iIndex];
}

Node* WinFormPanel::getNode(const function<bool(Node*)>& match) const
{
    int n = m_iRow * m_iColumn;
    for (int i = 0; i < n; ++i)
    {
        if (m_ppNodes[i] && match(m_ppNodes[i]))
        {
            return m_ppNodes[i];
        }
    }

    return nullptr;
}

int WinFormPanel::getNodeIndex(Node* pNode) const
{
    int n = m_iRow * m_iColumn;
    for (int i = 0; i < n; ++i)
    {
        if (m_ppNodes[i] == pNode)
        {
            return i;
        }
    }

    return -1;
}

int WinFormPanel::index2Y(int iIndex) const
{
    return iIndex / m_iColumn;
}

int WinFormPanel::index2X(int iIndex) const
{
    return iIndex % m_iColumn;
}

int WinFormPanel::toIndex(int iX, int iY) const
{
    return iY * m_iColumn + iX;
}

Point WinFormPanel::index2Point(int iIndex)
{
    const Size& roSz = getContentSize();
    int iX = index2X(iIndex);
    int iY = index2Y(iIndex);
    return Point(
        m_oNodeSize.width * 0.5 + (m_oNodeSize.width + m_fHorInnerBorderWidth) * iX + m_fHorBorderWidth,
        m_oNodeSize.height * 0.5 + (m_oNodeSize.height + m_fVerInnerBorderWidth) * iY + m_fVerBorderWidth);
}

void WinFormPanel::setBackground(Node* pBackground, float fBackgroundOffsetX, float fBackgroundOffsetY)
{
    if (m_pBackground == pBackground)
    {
        return;
    }

    if (m_pBackground != nullptr)
    {
        m_pBackground->removeFromParentAndCleanup(true);
        m_pBackground->release();
    }

    if (pBackground != nullptr)
    {
        pBackground = pBackground;
        addChild(pBackground);
        pBackground->setPosition(getAnchorPointInPoints() + Point(fBackgroundOffsetX, fBackgroundOffsetY));
    }

    m_pBackground = pBackground;
}

// on "init" you need to initialize your instance
bool EmptySceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    //if (!LayerColor::initWithColor(Color4B(153, 100, 00, 128)))
    {
        return false;
    }

    //lab00();
    //return true;

    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);

    gc->loadFrames("Global0");
    gc->loadFrames("Global1");

    auto a = new CBuffMakerAct("BMA", "雷霆一击", 5.0f, CCommandTarget::kNoTarget, CUnitForce::kEnemy, 1.0f, 0);
    a->setImageName("UI/Ability/ThunderCap.png");
    a->setMaxLevel(3);
    a->setLevel(3);
    a->setGrade(CAbility::kEpic);
    a->setCost(9);
    auto ai = AbilityItem::create(a);

    auto bp = WinFormPanel::create(6, 3, 3, 3, ai->getContentSize(), 10, 10, 0.0f, 20.0f);
    bp->setColor(Color3B::GRAY);
    bp->setOpacity(128);
    addChild(bp);
    bp->setBufferEffectParam(1.0f, 0.9f, 20.0f, 0.1f);
    bp->setWinPosition(getAnchorPointInPoints() - Point(bp->getWinSize().width * 0.5, bp->getWinSize().height * 0.5));
    bp->setOffsetPosition(Point(0, -9999));

    bp->addNodeEx(ai, WinFormPanel::kTopToBottom);
    
    // 镂空
    auto sp = Scale9Sprite::create("UI/ScaleBorder2_109x69_45x157.png");
    sp->setInsetTop(69);
    sp->setInsetBottom(69);
    sp->setInsetLeft(45);
    sp->setInsetRight(45);
    sp->setContentSize(bp->getWinSize() + Size(52, 52));

    addChild(sp, 1);
    sp->setPosition(getAnchorPointInPoints());

    // Clipping Node
    auto sn = LayerColor::create(Color4B(0, 0, 0, 255), sp->getContentSize().width, sp->getContentSize().height);
    sn->ignoreAnchorPointForPosition(false);
    sn->setPosition(sp->getPosition());
    sn->setScaleX((sp->getContentSize().width - 10) / sp->getContentSize().width);
    sn->setScaleY((sp->getContentSize().height - 10) / sp->getContentSize().height);

    auto cn = ClippingNode::create(sn);
    cn->setContentSize(getContentSize());
    cn->setInverted(true);

    auto bg = Sprite::create("backgrounds/MapBackground.png");
    cn->addChild(bg, 10);
    bg->setPosition(getAnchorPointInPoints());

    addChild(cn);

    a = new CBuffMakerAct("BMA", "引力漩涡", 5.0f, CCommandTarget::kNoTarget, CUnitForce::kEnemy, 1.0f, 0);
    a->setImageName("UI/Ability/GravitySurf.png");
    a->setMaxLevel(2);
    a->setLevel(1);
    a->setGrade(CAbility::kRare);
    a->setCost(2);
    ai = AbilityItem::create(a);
    bp->addNodeEx(ai, WinFormPanel::kTopToBottom);
    

    a = new CBuffMakerAct("BMA", "群体诅咒", 5.0f, CCommandTarget::kNoTarget, CUnitForce::kEnemy, 1.0f, 0);
    a->setImageName("UI/Ability/AbilityCurse.png");
    a->setMaxLevel(3);
    a->setLevel(0);
    a->setGrade(CAbility::kNormal);
    a->setCost(1);
    ai = AbilityItem::create(a);
    bp->addNodeEx(ai, WinFormPanel::kTopToBottom);

    a->setGrade((CAbility::GRADE)(rand() % 4));
    ai = AbilityItem::create(a->copy());
    bp->addNodeEx(ai, WinFormPanel::kTopToBottom);

    a->setGrade((CAbility::GRADE)(rand() % 4));
    ai = AbilityItem::create(a->copy());
    bp->addNodeEx(ai, WinFormPanel::kTopToBottom);

    a->setGrade((CAbility::GRADE)(rand() % 4));
    ai = AbilityItem::create(a->copy());
    bp->addNodeEx(ai, WinFormPanel::kTopToBottom);

    a->setGrade((CAbility::GRADE)(rand() % 4));
    ai = AbilityItem::create(a->copy());
    bp->addNodeEx(ai, WinFormPanel::kTopToBottom);

    // Pop panel
    sp = Scale9Sprite::create("UI/PopBorder.png");
    sp->setInsetLeft(24);
    sp->setInsetRight(24);
    sp->setInsetTop(24);
    sp->setInsetBottom(24);
    sp->setContentSize(Size(620, ai->getContentSize().height * 3.5));
    addChild(sp, 10);
    sp->setPosition(getAnchorPointInPoints());

    a->setCoolDown(15.0f);
    a->setDescribe("诅咒一片区域，受诅咒的英雄受到20点/秒的伤害，持续13秒，每隔4秒每损失100点的生命值就会受到40点的额外伤害");
    a->setLevel(2);
    a->setGrade(CAbility::kLegend);
    a->setCost(12);
    auto aii = Sprite::create(a->getImageName());
    sp->addChild(aii, 1);
    aii->setPosition(Point(aii->getContentSize().width * 0.5 + 30, sp->getContentSize().height - aii->getContentSize().height * 0.5 - 30));

    char sz[1024];
    auto ainame = Label::createWithTTF(gbk_to_utf8(a->getName(), sz), "fonts/DFYuanW7-GB2312.ttf", 48);
    //ainame->setAnchorPoint(Point(0.5f, 0.5f));
    sp->addChild(ainame, 2);
    ainame->setPosition(aii->getPosition() + Point(182, 20));
    ainame->setColor(Color3B::BLACK);

    auto ainame2 = Label::createWithTTF(ainame->getTTFConfig(), ainame->getString());
    //ainame2->setAnchorPoint(Point(0.0f, 0.5f));
    sp->addChild(ainame2, 3);
    ainame2->setPosition(ainame->getPosition() + Point(-3, 3));
    
    Color3B color;
    switch (a->getGrade())
    {
    case CAbility::kNormal:
        color = Color3B::WHITE;
        break;

    case CAbility::kRare:
        color = Color3B(42, 97, 255);
        break;

    case CAbility::kEpic:
        color = Color3B(180, 0, 255);
        break;

    case CAbility::kLegend:
        color = Color3B(226, 155, 17);
        break;

    default:
        return false;
    }
    ainame2->setColor(color);

    const Point aistarCenter = aii->getPosition() + Point(182, -32);
    const float aistarBetween = 5;
    auto aistar = Sprite::createWithSpriteFrameName(a->getLevel() > 0 ? "UI/Ability/AbilityItemStar.png" : "UI/Ability/AbilityItemUnstar.png");
    float starBaseWidth = aistarCenter.x - (a->getMaxLevel() - 1) * aistar->getContentSize().width * 0.5 - (a->getMaxLevel() - 1) * aistarBetween * 0.5;
    sp->addChild(aistar, 1, 100);
    aistar->setPosition(Point(starBaseWidth, aistarCenter.y));
    for (auto i = 1; i < a->getMaxLevel(); ++i)
    {
        aistar = Sprite::createWithSpriteFrameName(a->getLevel() > i ? "UI/Ability/AbilityItemStar.png" : "UI/Ability/AbilityItemUnstar.png");
        sp->addChild(aistar, 1, 100 + i);
        aistar->setPosition(Point(starBaseWidth + i * (aistar->getContentSize().width + aistarBetween), aistarCenter.y));
    }

    char str[1024];
    sprintf(str, "主动(%d秒)\n所需精力: %d", toInt(a->getCoolDown()), a->getCost());
    auto lbl = Label::createWithTTF(gbk_to_utf8(str, sz), "fonts/DFYuanW7-GB2312.ttf", 28);
    lbl->setAnchorPoint(Point(0.0f, 0.5f));
    sp->addChild(lbl);
    lbl->setColor(Color3B(132, 142, 255));
    lbl->setPosition(aii->getPosition() + Point(295, 0.0f));

    lbl = Label::createWithTTF(gbk_to_utf8(a->getDescribe(), sz), "fonts/DFYuanW7-GB2312.ttf", 28, Size(520, 160));
    lbl->setAnchorPoint(Point(0.0f, 1.0f));
    sp->addChild(lbl);
    lbl->setPosition(aii->getPosition() + Point(-55, -100));

    
    float fBaseHeightDelta = -350;
    auto sp2 = Sprite::create("UI/Ability/AbilityLevel.png");
    sp->addChild(sp2);
    sp2->setPosition(Point(sp->getContentSize().width * 0.5, aii->getPosition().y + fBaseHeightDelta + 50));

    sprintf(str, "%d/%d", a->getLevel(), a->getMaxLevel());
    lbl = Label::createWithTTF(gbk_to_utf8(str, sz), "fonts/DFYuanW7-GB2312.ttf", 28);
    lbl->setAnchorPoint(Point(0.0f, 0.5f));
    sp->addChild(lbl);
    lbl->setPosition(sp2->getPosition() + Point(50, 0.0f));
    //lbl->setColor(Color3B::RED);

    auto tmp = "伤害\n强化";
    lbl = Label::createWithTTF(gbk_to_utf8(tmp, sz), "fonts/DFYuanW7-GB2312.ttf", 28);
    lbl->setAnchorPoint(Point(0.0f, 1.0f));
    sp->addChild(lbl);
    lbl->setPosition(aii->getPosition() + Point(-55, fBaseHeightDelta));
    lbl->setColor(Color3B::RED);

    sp2 = Sprite::create("UI/Ability/AbilityUpgraded.png");
    sp->addChild(sp2);
    sp2->setPosition(lbl->getPosition() + Point(465, -lbl->getContentSize().height * 0.5));

    tmp = "每损失100的生命时所受到的伤害提高40点";
    lbl = Label::createWithTTF(gbk_to_utf8(tmp, sz), "fonts/DFYuanW7-GB2312.ttf", 28, Size(300, 80));
    lbl->setAnchorPoint(Point(0.0f, 1.0f));
    sp->addChild(lbl);
    lbl->setPosition(aii->getPosition() + Point(20, fBaseHeightDelta));

    tmp = "特效\n附加";
    lbl = Label::createWithTTF(gbk_to_utf8(tmp, sz), "fonts/DFYuanW7-GB2312.ttf", 28);
    lbl->setAnchorPoint(Point(0.0f, 1.0f));
    sp->addChild(lbl);
    lbl->setPosition(aii->getPosition() + Point(-55, fBaseHeightDelta - 95));
    lbl->setColor(Color3B::BLUE);

    sp2 = Sprite::create("UI/Ability/AbilityUpgraded.png");
    sp->addChild(sp2);
    sp2->setPosition(lbl->getPosition() + Point(465, -lbl->getContentSize().height * 0.5));

    tmp = "同时降低单位15%的移动速度";
    lbl = Label::createWithTTF(gbk_to_utf8(tmp, sz), "fonts/DFYuanW7-GB2312.ttf", 28, Size(300, 80));
    lbl->setAnchorPoint(Point(0.0f, 1.0f));
    sp->addChild(lbl);
    lbl->setPosition(aii->getPosition() + Point(20, fBaseHeightDelta - 95));

    tmp = "持久\n强化";
    lbl = Label::createWithTTF(gbk_to_utf8(tmp, sz), "fonts/DFYuanW7-GB2312.ttf", 28);
    lbl->setAnchorPoint(Point(0.0f, 1.0f));
    sp->addChild(lbl);
    lbl->setPosition(aii->getPosition() + Point(-55, fBaseHeightDelta - 95 * 2));
    lbl->setColor(Color3B::YELLOW);

    tmp = "诅咒效果持续17秒";
    lbl = Label::createWithTTF(gbk_to_utf8(tmp, sz), "fonts/DFYuanW7-GB2312.ttf", 28, Size(300, 80));
    lbl->setAnchorPoint(Point(0.0f, 1.0f));
    sp->addChild(lbl);
    lbl->setPosition(aii->getPosition() + Point(20, fBaseHeightDelta - 95 * 2));

    Utils::nodeToFile(sp, "AbilityPopPanel.png");

    return true;
}
