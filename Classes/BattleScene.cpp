#include "CommHeader.h"

#include "BattleScene.h"
#include "GameControl.h"
#include "DrawForCC.h"
#include "Logic/Unit.h"
#include "Logic/Ability.h"


// CBattleWorld
CBattleWorld::CBattleWorld()
    : m_iHeroUnit(0)
{
}

CBattleWorld::~CBattleWorld()
{

}

void CBattleWorld::onInit()
{
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint org = CCDirector::sharedDirector()->getVisibleOrigin();

    M_DEF_GC(gc);
    gc->loadTexture("Global");
    gc->loadAnimation("Units/Malik/move", "Units/Malik/move", 0.08f);
    gc->loadAnimation("Units/Malik/act1", "Units/Malik/act1", 0.08f);
    gc->loadAnimation("Units/Malik/act2", "Units/Malik/act2", 0.08f);
    gc->loadAnimation("Units/Matchstick/move", "Units/Matchstick/move", 0.08f);
    gc->loadAnimation("Units/Matchstick/act1", "Units/Matchstick/act1", 0.08f);
    gc->loadAnimation("Units/Matchstick/act2", "Units/Matchstick/act2", 0.08f);

    // 创建Draw
    CUnitDrawForCC* ud = new CUnitDrawForCC("Malik");
    ud->prepareFrame(CUnitDrawForCC::kFrmDefault, "default");
    ud->prepareAnimation(CUnitDrawForCC::kAniMove, "move", -1);
    ud->prepareAnimation(CUnitDrawForCC::kAniAct1, "act1", 4);
    ud->prepareAnimation(CUnitDrawForCC::kAniAct2, "act2", 4);

    ud->setSpriteProperty(7.0f, 10.0f, ccp(0.5f, 0.1125f), ccp(10.0f, 10.0f));

    // 创建hero
    CUnit* u = new CUnit("CUnit");
    m_iHeroUnit = u->getId();
    u->setDraw(ud);
    u->setName("Malik");
    u->setMaxHp(1000.0001f);
    u->setForceByIndex(1);

    addUnit(u);

    CAttackAct* atk = new CAttackAct("NormalAttack",
                                     "攻击",
                                     3.25,
                                     CAttackValue(1,
                                     CAttackValue::kPhysical,
                                     30.0),
                                     0.5);
    atk->setCastMinRange(-3.0f);
    atk->setCastRange(15.0f);
    atk->setCastHorizontal();
    atk->addCastAnimation(CUnitDraw::kAniAct1);
    atk->addCastAnimation(CUnitDraw::kAniAct2);
    int id = addTemplateAbility(atk);
    u->addActiveAbility(id);

    ud->setBaseMoveSpeed(80.0f);
    ud->setPosition(CPoint(vs.width * 0.5, vs.height * 0.5));

    // create other units
    ud = new CUnitDrawForCC("Matchstick");
    ud->prepareFrame(CUnitDrawForCC::kFrmDefault, "default");
    ud->prepareAnimation(CUnitDrawForCC::kAniMove, "move", -1);
    ud->prepareAnimation(CUnitDrawForCC::kAniAct1, "act1", 3);
    ud->prepareAnimation(CUnitDrawForCC::kAniAct2, "act2", 2);
    ud->setSpriteProperty(24.0f, 27.0f, ccp(69.0 / 128, 6.0 / 128), ccp(41.0f, 29.0f));

    u = new CUnit("CUnit");
    u->setDraw(ud);
    u->setName("Matchstick");
    u->setMaxHp(1000.0001f);
    u->setForceByIndex(2);

    addUnit(u);

    ud->setBaseMoveSpeed(80.0f);
    ud->setPosition(CPoint(vs.width * 0.7, vs.height * 0.5));

}

// CCBattleScene
CCBattleScene::CCBattleScene()
    : m_pWorld(NULL)
{
}

CCBattleScene::~CCBattleScene()
{
    if (m_pWorld != NULL)
    {
        m_pWorld->release();
    }
}

bool CCBattleScene::init()
{
    m_pWorld = new CBattleWorld;
    m_pWorld->retain();

    return CCScene::init();
}


// CCBattleSceneLayer
CCBattleSceneLayer::CCBattleSceneLayer()
{
}

CCBattleSceneLayer::~CCBattleSceneLayer()
{
}

CCScene* CCBattleSceneLayer::scene()
{
    // 'scene' is an autorelease object
    CCBattleScene* pScene = CCBattleScene::create();
    
    CUnitWorldForCC* pWorld = pScene->getWorld();
    // 'layer' is an autorelease object
    CCUnitLayer* layer = CCBattleSceneLayer::create();
    pWorld->setLayer(layer);

    // add layer as a child to scene
    pScene->addChild(layer);

    pWorld->init();

    // return the scene
    return pScene;
}

// on "init" you need to initialize your instance
bool CCBattleSceneLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    setBackGroundSprite(CCSprite::create("BackgroundHD.png"));
    setBufferEffectParam(0.9f, 10.0f, 0.1f);
    
    CCSize vs = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint org = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(org.x + vs.width/2,
                            org.y + vs.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    setWorldInterval(0.1f);
    
    
    return true;
}

void CCBattleSceneLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    CCWinUnitLayer::ccTouchEnded(pTouch, pEvent);
    if (!isClickAction())
    {
        return;
    }

    CCPoint pos = ccpSub(pTouch->getLocation(), getPosition());

    CBattleWorld* w = DCAST(getWorld(), CBattleWorld*);
    CUnit* hero = w->getUnit(w->getHeroUnit());
    if (hero == NULL)
    {
        return;
    }

    CUnitDraw2D* d = DCAST(hero->getDraw(), CUnitDraw2D*);
    if (d == NULL)
    {
        return;
    }

    CPoint p(pos.x, pos.y);
    CUnit* t = CUnitGroup::getNearestUnitInRange(w, p, 50.0f);

    if (t != NULL && hero != t)
    {
        d->setCastTarget(CCommandTarget(t->getId()));
        d->cmdCastSpell(hero->getAttackAbilityId());
        
    }
    else
    {
        CUnitDraw2D::UNIT_MOVE_PARAMS mp;
        mp.bAutoFlipX = true;
        d->move(p, mp);
    }
}


