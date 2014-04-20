#include "CommHeader.h"

#include "Stage.h"
#include "ComponentForCC.h"
#include "GameControl.h"


// CStage
CStage::CStage()
    : m_iIndex(0)
    , m_eStatus(kLocked)
    , m_pNode(NULL)
    , m_iGrade(0)
{
    memset(m_apStar, 0, sizeof(m_apStar));
}

void CStage::setGrade( int iGrade )
{
    m_iGrade = iGrade;
    M_DEF_GC(gc);
    for (int i = 0; i < 3; ++i)
    {
        DCAST(m_apStar[i], CCSprite*)->setDisplayFrame(gc->getfc()->spriteFrameByName(i <= m_iGrade ? getStarName() : getUnstarName()));
    }
}

void CStage::setStatus(STAGE_STATUS eStatus)
{
    if (m_eStatus != eStatus)
    {
        m_eStatus = eStatus;
    }

    STAGE_STATUS eOldStatus = m_eStatus;
    onChangeStatus(eOldStatus);
}

void CStage::setStarNode( int iIndex, CCNode* pNode )
{
    m_apStar[iIndex] = pNode;
}

CCNode* CStage::getStarNode( int iIndex )
{
    return m_apStar[iIndex];
}

void CStage::setStarNodesVisible( bool bVisible /*= true*/ )
{
    m_apStar[0]->setVisible(bVisible);
    m_apStar[1]->setVisible(bVisible);
    m_apStar[2]->setVisible(bVisible);
}
void CStage::onInit()
{
}

void CStage::onChangeStatus( STAGE_STATUS eOldStatus )
{
    CCMenuItemImage* mi = DCAST(getNode(), CCMenuItemImage*);
    switch (m_eStatus)
    {
    case kLocked:
        mi->setEnabled(false);
        break;

    case kUnlocked:
        mi->setEnabled(true);
        break;

    case kConquered:
        setStarNodesVisible(true);
        break;
    }
}

// CStageMap
CStageMap::CStageMap()
    : m_pPanel(NULL)
    , m_pSender(NULL)
    , m_pHandler(NULL)
{
}

CStageMap::~CStageMap()
{
}

void CStageMap::setPanel( CCMenu* pPanel, CCObject* pSender, SEL_MenuHandler pHandler )
{
    m_pPanel = pPanel;
    m_pSender = pSender;
    m_pHandler = pHandler;
}

void CStageMap::addStage(CStage* pStage, const VEC_INDEXES& vecPrev)
{
    pStage->setIndex(m_vecStages.size());
    
    //CCMenuItemImage* mi = CCMenuItemImage::create(pStage->getNormalName(), pStage->getSelectedName(), pStage->getDisabledName(), m_pSender, m_pHandler);
    CCSprite* sel = CCSprite::create(pStage->getNormalName());
    CCSprite* sel2 = CCSprite::create(pStage->getSelectedName());
    sel->addChild(sel2);
    sel2->setPosition(sel->getAnchorPointInPoints());
    CCButtonNormal* btn = CCButtonNormal::create(
        CCSprite::create(pStage->getNormalName()),
        sel,
        CCSprite::create(pStage->getDisabledName()),
        NULL,
        NULL,
        0.0f,
        m_pSender,
        m_pHandler,
        NULL);
    btn->setTag(pStage->getIndex());
    m_pPanel->addChild(btn);
    btn->setPosition(pStage->getPosition());
    pStage->setNode(btn);
    
    M_DEF_GC(gc);
    // set star
    for (int i = 0; i < 3; ++i)
    {
        CCSprite* nd = CCSprite::createWithSpriteFrameName(pStage->getUnstarName());
        pStage->setStarNode(i, nd);
        btn->addChild(nd);
        const CCSize& oStageSz = btn->getContentSize();
        const CCSize& oStarSz = nd->getContentSize();
        float bx = oStarSz.width + 0;
        float by = oStageSz.height * 0.5 + oStarSz.height * 0.5 + 0;
        nd->setPosition(ccpAdd(btn->getAnchorPointInPoints(), ccp(bx * (i - 1), -by)));
    }
    pStage->setStarNodesVisible(false);

    STAGE_INFO stageInfo = {};
    stageInfo.pStage = pStage;
    m_vecStages.push_back(stageInfo);
    for (auto it = vecPrev.begin(); it != vecPrev.end(); ++it)
    {
        addStageNextInfos(*it, pStage->getIndex());
    }

    pStage->onInit();
}

void CStageMap::addStageNextInfos( int iIndex, int iNextIndex )
{
    // create path
    CCSprite* pth = CCSprite::createWithSpriteFrameName(getPathName());
    pth->setColor(ccc3(123, 123, 123));
    const CCSize& pthSz = pth->getContentSize();
    const CCPoint& from = m_vecStages[iIndex].pStage->getPosition();
    const CCPoint& to = m_vecStages[iNextIndex].pStage->getPosition();
    m_pPanel->getParent()->addChild(pth);
    CCSprite* sp = CCSprite::createWithSpriteFrameName(getPathName());
    sp->setColor(ccc3(248, 168, 1));
    CCProgressBar* pth2 = CCProgressBar::create(pth->getContentSize(), sp, NULL, 0.0f, 0.0f, true);
    pth->addChild(pth2, 1, 2);
    pth2->setPosition(pth->getAnchorPointInPoints());
    pth2->setPercentage(0.0f);

    // scale path
    CCPoint oDelta = ccpSub(to, from);
    float fR = CC_RADIANS_TO_DEGREES(-ccpToAngle(oDelta));
    float fScale = (sqrt(oDelta.x * oDelta.x + oDelta.y * oDelta.y) - 200) / pthSz.width;
    pth->setPosition(ccp((from.x + to.x) / 2, (from.y + to.y) / 2));
    pth->setRotation(fR);
    pth->setScaleX(fScale);

    STAGE_NEXT_INFO nextInfo = {};
    nextInfo.iIndex = iNextIndex;
    nextInfo.pPath = pth;
    m_vecStages[iIndex].vecNextInfos.push_back(nextInfo);
}

void CStageMap::changeStageStatus( int iIndex, CStage::STAGE_STATUS eStatus )
{
    CStage* s = m_vecStages[iIndex].pStage;
    switch (eStatus)
    {
    case CStage::kLocked:
        break;

    case CStage::kUnlocked:
        break;

    case CStage::kConquered:
        for (auto it = m_vecStages[iIndex].vecNextInfos.begin(); it != m_vecStages[iIndex].vecNextInfos.end(); ++it)
        {
            changeStageStatus(it->iIndex, CStage::kUnlocked);
            //DCAST(it->pPath, CCSprite*)->setColor(ccc3(248, 168, 1));
            CCProgressBar* pth2 = DCAST(it->pPath->getChildByTag(2), CCProgressBar*);
            pth2->runActionForTimer(CCEaseExponentialOut::create(pth2->setPercentageAction(100.0f, 0.3f)));
        }
        s->setGrade(rand() % 3);
        break;
    }
    s->setStatus(eStatus);
}
