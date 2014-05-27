#include "CommHeader.h"

#include "AbilityScene.h"
#include "GameControl.h"
#include "ComponentForCC.h"
#include "Ability.h"
#include "UserData.h"
#include "GameData.h"
#include "AbilityLibrary.h"


// AbilitySceneLayer
AbilitySceneLayer::AbilitySceneLayer()
{
}

AbilitySceneLayer::~AbilitySceneLayer()
{
}

Scene* AbilitySceneLayer::scene()
{
    // 'scene' is an autorelease object
    Scene* pScene = Scene::create();

    AbilitySceneLayer* layer = AbilitySceneLayer::create();

    // add layer as a child to scene
    if (layer != NULL)
    {
        M_DEF_GC(gc);
        //gc->preloadSound("sounds/Effect/xxxxxxx.mp3");

        pScene->addChild(layer);
    }

    // return the scene
    return pScene;
}

class AbilityDetails : public Scale9Sprite
{
public:
    bool initWithAbility(CAbility* ability);
    M_CREATE_INITWITH_FUNC_PARAM(Ability, AbilityDetails, (CAbility* ability), ability);

    void updateContent(CAbility* ability);

    M_SYNTHESIZE_READONLY(CAbility*, m_ability, Ability);
    void setAbility(CAbility* ability);

CC_CONSTRUCTOR_ACCESS:
    AbilityDetails();
    virtual ~AbilityDetails();

protected:
    Sprite* starByIndex(int index, bool on);
    Sprite* iconByType(int index, int type);

    void onClickUpgrade(Ref* ref);

protected:
    MenuEx* m_mn;
    Sprite* m_aIcon;  // ability icon
    Label* m_aNameBg;
    Label* m_aNameFg;
    Sprite* m_aStars[3];
    Label* m_aTypeCost;
    Label* m_aDesc;
    Sprite* m_aUpTitle;
    Label* m_aLevel;
    Sprite* m_aUpIcon[3];
    Label* m_aUpDesc[3];
    Sprite* m_aUpgraded[3];
    MenuItem* m_aUpBtn;
};


// AbilityDetails
AbilityDetails::AbilityDetails()
: m_ability(nullptr)
, m_mn(nullptr)
, m_aIcon(nullptr)
, m_aNameBg(nullptr)
, m_aNameFg(nullptr)
, m_aTypeCost(nullptr)
, m_aDesc(nullptr)
, m_aUpTitle(nullptr)
, m_aLevel(nullptr)
, m_aUpBtn(nullptr)
{
    memset(m_aStars, 0, sizeof(m_aStars));
    memset(m_aUpIcon, 0, sizeof(m_aUpIcon));
    memset(m_aUpDesc, 0, sizeof(m_aUpDesc));
    memset(m_aUpgraded, 0, sizeof(m_aUpgraded));
}

AbilityDetails::~AbilityDetails()
{
    M_SAFE_RELEASE(m_ability);
}

bool AbilityDetails::initWithAbility(CAbility* ability)
{
    //if (Scale9Sprite::initWithFile("UI/PopBorder.png") == false)
    if (Scale9Sprite::initWithFile("UI/ScaleBorder2_109x69_45x157.png") == false)
    {
        return false;
    }

    setAbility(ability);

    //setInsetLeft(24);
    //setInsetRight(24);
    //setInsetTop(24);
    //setInsetBottom(24);
    setInsetTop(69);
    setInsetBottom(69);
    setInsetLeft(45);
    setInsetRight(45);
    setContentSize(Size(620, 763));
    
    auto l = LayerColor::create(Color4B(83, 83, 83, 255));
    l->setContentSize(getContentSize() - Size(10, 10));
    addChild(l, -1);
    l->setPosition(Point::ZERO + Point(5, 5));

    m_mn = MenuEx::create();
    m_mn->setContentSize(l->getContentSize());
    addChild(m_mn, 4);
    m_mn->setPosition(l->getPosition());

    updateContent(ability);

    return true;
}

void AbilityDetails::updateContent(CAbility* ability)
{
    static auto fc = SpriteFrameCache::getInstance();
    static auto tc = Director::getInstance()->getTextureCache();

    char str[1024];  // buffer for sprintf
    char sz[1024];  //  buffer for gbk2utf8

    setAbility(ability);

    // icon
    if (m_aIcon == nullptr)
    {
        m_aIcon = Sprite::create(ability->getImageName());
        addChild(m_aIcon);
        m_aIcon->setPosition(Point(m_aIcon->getContentSize().width * 0.5 + 30, getContentSize().height - m_aIcon->getContentSize().height * 0.5 - 30));
    }
    else
    {
        auto tx = Director::getInstance()->getTextureCache()->addImage(ability->getImageName());
        auto sz = tx->getContentSize();
        m_aIcon->setSpriteFrame(SpriteFrame::createWithTexture(tx, Rect(0.0f, 0.0f, sz.width, sz.height)));
    }


    // name
    gbk_to_utf8(ability->getName(), sz);
    if (m_aNameBg == nullptr)
    {
        m_aNameBg = Label::createWithTTF(sz, "fonts/DFYuanW7-GB2312.ttf", 48);
        addChild(m_aNameBg, 1);
        m_aNameBg->setColor(Color3B::BLACK);
        m_aNameBg->setPosition(m_aIcon->getPosition() + Point(182, 20));
    }
    else
    {
        m_aNameBg->setString(sz);
    }
    
    if (m_aNameFg == nullptr)
    {
        m_aNameFg = Label::createWithTTF(m_aNameBg->getTTFConfig(), m_aNameBg->getString());
        addChild(m_aNameFg, 2);
        m_aNameFg->setPosition(m_aNameBg->getPosition() + Point(-3, 3));
    }
    else
    {
        m_aNameFg->setString(m_aNameBg->getString());
    }
    m_aNameFg->setColor(AbilityItem::abilityGradeColor3B(ability->getGrade()));

    
    // star
    const Point aistarCenter = m_aIcon->getPosition() + Point(182, -32);
    const float aistarBetween = 5;
    
    auto star = starByIndex(0, ability->getLevel() > 0);
    float starBaseWidth = aistarCenter.x - (ability->getMaxLevel() - 1) * star->getContentSize().width * 0.5 - (ability->getMaxLevel() - 1) * aistarBetween * 0.5;
    
    star->setPosition(Point(starBaseWidth, aistarCenter.y));
    
    for (auto i = 1; i < 3; ++i)
    {
        if (i < ability->getMaxLevel())
        {
            star = starByIndex(i, ability->getLevel() > i);
            star->setVisible(true);
            star->setPosition(Point(starBaseWidth + i * (star->getContentSize().width + aistarBetween), aistarCenter.y));
        }
        else if (m_aStars[i] != nullptr)
        {
            m_aStars[i]->setVisible(false);
        }
        
    }


    // ability type & cost
    const char* type;
    if (DCAST(ability, CActiveAbility*) != nullptr)
    {
        type = "主动";
    }
    else if (DCAST(ability, CPassiveAbility*) != nullptr)
    {
        type = "被动";
    }
    else
    {
        type = "未知";
    }

    if (ability->getCoolDown() > 0.0f)
    {
        sprintf(str, "%s(%d秒)\n所需精力: %d", type, toInt(ability->getCoolDown()), ability->getCost());
    }
    else
    {
        sprintf(str, "%s\n所需精力: %d", type, ability->getCost());
    }
    
    gbk_to_utf8(str, sz);
    if (m_aTypeCost == nullptr)
    {
        m_aTypeCost = Label::createWithTTF(sz, "fonts/DFYuanW7-GB2312.ttf", 28);
        m_aTypeCost->setAnchorPoint(Point(0.0f, 0.5f));
        addChild(m_aTypeCost);
        m_aTypeCost->setColor(Color3B(132, 142, 255));
        m_aTypeCost->setPosition(m_aIcon->getPosition() + Point(295, 0.0f));
    }
    else
    {
        m_aTypeCost->setString(sz);
    }


    // describe
    gbk_to_utf8(ability->getDescribe(), sz);
    if (m_aDesc == nullptr)
    {
        m_aDesc = Label::createWithTTF(sz, "fonts/DFYuanW7-GB2312.ttf", 28, Size(520, 160));
        m_aDesc->setAnchorPoint(Point(0.0f, 1.0f));
        addChild(m_aDesc);
        m_aDesc->setPosition(m_aIcon->getPosition() + Point(-55, -100));
    }
    else
    {
        m_aDesc->setString(sz);
    }
    
    
    // level up
    float fBaseHeightDelta = -350;
    if (m_aUpTitle == nullptr)
    {
        m_aUpTitle = Sprite::create("UI/Ability/AbilityLevel.png");
        addChild(m_aUpTitle);
        m_aUpTitle->setPosition(Point(getContentSize().width * 0.5, m_aIcon->getPosition().y + fBaseHeightDelta + 50));
    }

    sprintf(str, "%d/%d", ability->getLevel(), ability->getMaxLevel());
    gbk_to_utf8(str, sz);
    if (m_aLevel == nullptr)
    {
        m_aLevel = Label::createWithTTF(sz, "fonts/DFYuanW7-GB2312.ttf", 28);
        m_aLevel->setAnchorPoint(Point(0.0f, 0.5f));
        addChild(m_aLevel);
        m_aLevel->setPosition(m_aUpTitle->getPosition() + Point(50, 0.0f));
    }
    else
    {
        m_aLevel->setString(sz);
    }

    if (m_aUpBtn != nullptr)
    {
        m_aUpBtn->setVisible(false);
    }
    
    for (auto i = 0; i < 3; ++i)
    {
        if (i < ability->getMaxLevel())
        {
            auto type = ability->getLevelType(i + 1);
            if (type < 0)
            {
                break;
            }

            auto upIcon = iconByType(i, type);
            upIcon->setPosition(m_aIcon->getPosition() + Point(-27, fBaseHeightDelta - 95 * i - 28));
            upIcon->setVisible(true);

            auto desc = ability->getLevelDescribe(i + 1);
            if (desc == nullptr)
            {
                break;
            }

            gbk_to_utf8(desc, sz);
            if (m_aUpDesc[i] == nullptr)
            {
                m_aUpDesc[i] = Label::createWithTTF(sz, "fonts/DFYuanW7-GB2312.ttf", 28, Size(300, 300));
                m_aUpDesc[i]->setAnchorPoint(Point(0.0f, 1.0f));
                addChild(m_aUpDesc[i]);
                m_aUpDesc[i]->setPosition(m_aIcon->getPosition() + Point(20, fBaseHeightDelta - 95 * i));
            }
            else
            {
                m_aUpDesc[i]->setString(sz);
                m_aUpDesc[i]->setVisible(true);
            }
            //tmp = "每损失100的生命时所受到的伤害提高40点";
            
        }
        else
        {
            if (m_aUpIcon[i] != nullptr)
            {
                m_aUpIcon[i]->setVisible(false);
            }

            if (m_aUpDesc[i] != nullptr)
            {
                m_aUpDesc[i]->setVisible(false);
            }
        }

        if (i < ability->getLevel())
        {
            if (m_aUpgraded[i] == nullptr)
            {
                m_aUpgraded[i] = Sprite::create("UI/Ability/AbilityUpgraded.png");
                addChild(m_aUpgraded[i]);
                m_aUpgraded[i]->setPosition(m_aUpIcon[i]->getPosition() + Point(439, 0));
            }
            else
            {
                m_aUpgraded[i]->setVisible(true);
            }
        }
        else
        {
            if (i == ability->getLevel() && i < ability->getMaxLevel())
            {
                // upgrade button
                if (m_aUpBtn == nullptr)
                {
                    m_aUpBtn = MenuItemFont::create("Upgrade", CC_CALLBACK_1(AbilityDetails::onClickUpgrade, this));
                    m_mn->addChild(m_aUpBtn);
                    m_aUpBtn->setColor(Color3B::YELLOW);
                }
                else
                {
                }
                m_aUpBtn->setVisible(true);
                m_aUpBtn->setPosition(m_aUpIcon[i]->getPosition() + Point(439, 0));
            }

            if (m_aUpgraded[i] != nullptr)
            {
                m_aUpgraded[i]->setVisible(false);
            }
        }
    }

    //Utils::nodeToFile(this, "AbilityPopPanel.png");
}

void AbilityDetails::setAbility(CAbility* ability)
{
    if (ability == m_ability)
    {
        return;
    }

    M_SAFE_RELEASE(m_ability);
    M_SAFE_RETAIN(ability);
    m_ability = ability;
}

Sprite* AbilityDetails::starByIndex(int index, bool on)
{
    static auto fc = SpriteFrameCache::getInstance();
    static auto tc = Director::getInstance()->getTextureCache();

    auto file = on ? "UI/Ability/AbilityItemStar.png" : "UI/Ability/AbilityItemUnstar.png";
    if (m_aStars[index] == nullptr)
    {
        m_aStars[index] = Sprite::create(file);
        addChild(m_aStars[index], 1);
    }
    else
    {
        auto tx = Director::getInstance()->getTextureCache()->addImage(file);
        auto sz = tx->getContentSize();
        m_aStars[index]->setSpriteFrame(SpriteFrame::createWithTexture(tx, Rect(0.0f, 0.0f, sz.width, sz.height)));
    }

    return m_aStars[index];
}

Sprite* AbilityDetails::iconByType(int index, int type)
{
    static auto fc = SpriteFrameCache::getInstance();
    static auto tc = Director::getInstance()->getTextureCache();

    auto file = "UI/Ability/AbilityLevel1.png";
    switch (type)
    {
    case 0:
        file = "UI/Ability/AbilityLevel1.png";
        break;

    case 1:
        file = "UI/Ability/AbilityLevel2.png";
        break;

    case 2:
        file = "UI/Ability/AbilityLevel3.png";
        break;

    }

    if (m_aUpIcon[index] == nullptr)
    {
        m_aUpIcon[index] = Sprite::create(file);
        addChild(m_aUpIcon[index]);
    }
    else
    {
        auto tx = Director::getInstance()->getTextureCache()->addImage(file);
        auto sz = tx->getContentSize();
        m_aUpIcon[index]->setSpriteFrame(SpriteFrame::createWithTexture(tx, Rect(0.0f, 0.0f, sz.width, sz.height)));
    }

    return m_aUpIcon[index];
}

void AbilityDetails::onClickUpgrade(Ref* ref)
{
    assert(m_ability->getLevel() < m_ability->getMaxLevel());

    auto ai = (AbilityItem*)getUserData();
    ai->setEquipped(false);
    
    m_ability->setLevel(m_ability->getLevel() + 1);
    updateContent(m_ability);
    
    ai->updateContent(m_ability);
}

// on "init" you need to initialize your instance
bool AbilitySceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    // test data
    CUserData::ABILITY_INFO info;
    info.id = 0;
    info.level = 1;
    CUserData::instance()->m_vecAbilitys.push_back(info);
    info.id = 1;
    info.level = 0;
    CUserData::instance()->m_vecAbilitys.push_back(info);

    CUserData::instance()->getHeroSelected()->energy = 12;

    static Size wsz = Director::getInstance()->getVisibleSize();
    M_DEF_GC(gc);

    gc->loadFrames("Global0");
    gc->loadFrames("Global1");

    auto tx = Director::getInstance()->getTextureCache()->addImage("UI/Ability/AbilityItemBackground.png");
    Size aiSz = tx->getContentSize();
    
    // 滚动层
    m_abilityItemsPanel = WinFormPanel::create(30, 2, 4, 2, aiSz, 10, 10, 0.0f, 20.0f);
    m_abilityItemsPanel->setColor(Color3B::GRAY);
    m_abilityItemsPanel->setOpacity(128);
    addChild(m_abilityItemsPanel);
    m_abilityItemsPanel->setWinSize(Size(m_abilityItemsPanel->getWinSize().width, 763 - 52));
    m_abilityItemsPanel->setBufferEffectParam(1.0f, 0.9f, 20.0f, 0.1f);

    // 镂空边框
    auto sp = Scale9Sprite::create("UI/ScaleBorder2_109x69_45x157.png");
    //auto sp = Scale9Sprite::create("UI/PopBorder.png");
    sp->setInsetTop(69);
    sp->setInsetBottom(69);
    sp->setInsetLeft(45);
    sp->setInsetRight(45);
    sp->setContentSize(Size(m_abilityItemsPanel->getWinSize().width + 52, 763));
    addChild(sp, 1);
    m_winFormBorder = sp;

    sp->setPosition(Point(sp->getContentSize().width * 0.5 + 10, wsz.height - sp->getContentSize().height * 0.5f - 10));
    m_abilityItemsPanel->setWinPosition(sp->getPosition() - Point(m_abilityItemsPanel->getWinSize().width * 0.5, m_abilityItemsPanel->getWinSize().height * 0.5));
    m_abilityItemsPanel->setOffsetPosition(Point(0, -9999));

    // Clipping Node
    auto sn = LayerColor::create(Color4B(0, 0, 0, 255), sp->getContentSize().width, sp->getContentSize().height);
    sn->ignoreAnchorPointForPosition(false);
    sn->setContentSize(sp->getContentSize() - Size(10, 10));
    sn->setPosition(sp->getPosition());

    auto cn = ClippingNode::create(sn);
    cn->setContentSize(getContentSize());
    cn->setInverted(true);

    auto bg = Sprite::create("backgrounds/MapBackground.png");
    cn->addChild(bg, 10);
    bg->setPosition(getAnchorPointInPoints());

    addChild(cn);

    // add ability items
    CUserData::VEC_ABILITY_INFOS& vec = CUserData::instance()->m_vecAbilitys;
    for (auto i = 0; i < (int)vec.size(); ++i)
    {
        auto& item = vec[i];
        auto a = CAbilityLibrary::instance()->copyAbility(item.id);
        a->setLevel(item.level);
        auto ai = AbilityItem::create(a);
        ai->setOnChangeEquippedCallback(CC_CALLBACK_2(AbilitySceneLayer::onChangeAbilityItemEquipped, this));
        m_abilityItemsPanel->addNodeEx(ai, WinFormPanel::kTopToBottom);
    }

    m_abilityItemsPanel->setClickNodeCallback(bind(&AbilitySceneLayer::onClickAbilityItems, this, placeholders::_1));

    // 已装备技能栏
    tx = Director::getInstance()->getTextureCache()->addImage("UI/Ability/AbilityBg.png");
    aiSz = tx->getContentSize();
    m_abilityEquippedPanel = ButtonPanel::create(1, 12, aiSz, 0, 0, nullptr);
    addChild(m_abilityEquippedPanel, 10);
    
    sp = Scale9Sprite::create("UI/ScaleBorder2_109x69_45x157.png");
    sp->setInsetTop(69);
    sp->setInsetBottom(69);
    sp->setInsetLeft(45);
    sp->setInsetRight(45);
    sp->setContentSize(m_abilityEquippedPanel->getContentSize() + Size(52, 52));
    auto l = LayerColor::create(Color4B(83, 83, 83, 255));
    l->setContentSize(sp->getContentSize() - Size(10, 10));
    sp->addChild(l, -1);
    l->setPosition(Point::ZERO + Point(5, 5));
    m_abilityEquippedPanel->setBackground(sp, 0.0f, 5.0f);
    m_abilityEquippedPanel->getBackground()->setLocalZOrder(-1);

    m_abilityEquippedPanel->setPosition(Point(wsz.width * 0.5, sp->getContentSize().height * 0.5 + 15));

    char str[1024];
    char sz[1024];
    m_energyCost = 0;
    sprintf(str, "精力: %d/%d", CUserData::instance()->getHeroSelected()->energy - m_energyCost, CUserData::instance()->getHeroSelected()->energy);
    gbk_to_utf8(str, sz);
    m_energy = Label::createWithTTF(sz, "fonts/DFYuanW7-GB2312.ttf", 32);

    addChild(m_energy, 11);
    m_energy->setDimensions(m_energy->getContentSize().width * 1.5, m_energy->getContentSize().height);
    m_energy->setPosition(Point(m_energy->getContentSize().width * 0.5 + wsz.width * 0.5 - m_abilityEquippedPanel->getContentSize().width * 0.5 + 0, m_energy->getContentSize().height * 0.5 + m_abilityEquippedPanel->getPosition().y + m_abilityEquippedPanel->getContentSize().height * 0.5 + 40));
    

    return true;
}

void AbilitySceneLayer::onClickAbilityItems(Node* abilityItem)
{
    auto ai = DCAST(abilityItem, AbilityItem*);
    auto a = ai->getAbility();

    auto pop = DCAST(getChildByTag(101), AbilityDetails*);
    if (pop == nullptr)
    {
        pop = AbilityDetails::create(a);
        addChild(pop, 1, 101);
        pop->setPosition(m_winFormBorder->getPosition() + Point(m_winFormBorder->getContentSize().width * 0.5 + pop->getContentSize().width * 0.5 + 10, m_winFormBorder->getContentSize().height * 0.5 - pop->getContentSize().height * 0.5));
        pop->setCascadeOpacityEnabled(true);
        pop->setOpacity(0);
        pop->runAction(FadeIn::create(0.2f));
    }
    else if (pop->getUserData() != ai)
    {
        pop->updateContent(a);
    }

    if (pop->getUserData() != ai)
    {
        pop->setUserData(ai);
    }
    else
    {
        // 装备状态变更
        if (ai->isEquipped())
        {
            ai->setEquipped(false);
        }
        else
        {
            if (CUserData::instance()->getHeroSelected()->energy - m_energyCost >= a->getCost())
            {
                ai->setEquipped(true);
                m_energy->stopAllActions();
                m_energy->setScale(1.0f);
                m_energy->setColor(Color3B(255, 255, 255));
                m_energy->runAction(Sequence::create(ScaleTo::create(0.1f, m_energy->getScale() * 1.3f), ScaleTo::create(0.1f, m_energy->getScale() * 1.0f), nullptr));
            }
            else
            {
                m_energy->stopAllActions();
                m_energy->setScale(1.0f);
                m_energy->setColor(Color3B(255, 255, 255));
                m_energy->runAction(Repeat::create(Sequence::create(TintTo::create(0.1f, 255, 0, 0), TintTo::create(0.1f, 255, 255, 255), nullptr), 3));
                ai->m_aicost2->stopAllActions();
                ai->m_aicost2->setColor(Color3B(78, 43, 7));
                ai->m_aicost2->runAction(Repeat::create(Sequence::create(TintTo::create(0.1f, 255, 255, 255), TintTo::create(0.1f, 78, 43, 7), nullptr), 3));
            }
        }
    }
}

void AbilitySceneLayer::onClickAbilityItemsEquipped(Ref* ref)
{
    auto btn = DCAST(ref, ButtonNormal*);
    auto ai = (AbilityItem*)btn->getUserData();
    ai->setEquipped(false);
}

void AbilitySceneLayer::onChangeAbilityItemEquipped(AbilityItem* ai, bool equipped)
{
    if (equipped == true)
    {
        auto btn = ButtonNormal::create(Sprite::create(ai->getAbility()->getImageName()), nullptr, nullptr, nullptr, nullptr, 0.0f, CC_CALLBACK_1(AbilitySceneLayer::onClickAbilityItemsEquipped, this), nullptr);
        m_abilityEquippedPanel->addButtonEx(btn, ButtonPanel::kTopToBottom);
        ai->setUserData(btn);
        btn->setUserData(ai);
        m_energyCost += ai->getAbility()->getCost();
    }
    else
    {
        auto btn = (ButtonNormal*)ai->getUserData();
        m_abilityEquippedPanel->delButton(btn);
        m_abilityEquippedPanel->clearUpSlot(ButtonPanel::kTopToBottom);
        ai->setUserData(nullptr); 
        m_energyCost -= ai->getAbility()->getCost();
    }

    char str[1024];
    char sz[1024];
    sprintf(str, "精力: %d/%d", CUserData::instance()->getHeroSelected()->energy - m_energyCost, CUserData::instance()->getHeroSelected()->energy);
    gbk_to_utf8(str, sz);
    m_energy->setString(sz);

    m_energy->stopAllActions();
    m_energy->setScale(1.0f);
    m_energy->setColor(Color3B(255, 255, 255));
    m_energy->runAction(Sequence::create(ScaleTo::create(0.1f, m_energy->getScale() * 1.3f), ScaleTo::create(0.1f, m_energy->getScale() * 1.0f), nullptr));
}
