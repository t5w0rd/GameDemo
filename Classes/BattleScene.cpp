#include "CommHeader.h"

#include "BattleScene.h"
#include "GameControl.h"
#include "DrawForCC.h"
#include "Logic/Unit.h"


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

    // 创建Draw
    CUnitDrawForCC* ud = new CUnitDrawForCC("Malik");
    ud->prepareFrame(CUnitDrawForCC::kFrmDefault, "default");
    ud->prepareAnimation(CUnitDrawForCC::kActMove, "move", -1);

    // 创建Unit
    CUnit* u = new CUnit("CUnit");
    m_iHeroUnit = u->getId();
    u->setDraw(ud);
    u->setName("Malik");
    u->setMaxHp(1000.0001f);
    u->setForceByIndex(1);

    addUnit(u);

    ud->setBaseMoveSpeed(80.0f);
    ud->setPosition(CPoint(vs.width * 0.5, vs.height * 0.5));

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

    CUnitDraw2D::UNIT_MOVE_PARAMS mp;
    mp.bAutoFlipX = true;
    d->move(CPoint(pos.x, pos.y), mp);
}


