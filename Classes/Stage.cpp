#include "CommHeader.h"

#include "Stage.h"
#include "ComponentForCC.h"
#include "GameControl.h"


// CStage
CStage::CStage()
: m_iIndex(0)
, m_eStatus(kLocked)
, m_pNode(nullptr)
, m_iGrade(0)
{
    memset(m_apStar, 0, sizeof(m_apStar));
}

void CStage::setGrade(int iGrade)
{
    m_iGrade = iGrade;
    M_DEF_GC(gc);
    for (int i = 0; i < 3; ++i)
    {
        DCAST(m_apStar[i], Sprite*)->setSpriteFrame(gc->getfc()->getSpriteFrameByName(i < m_iGrade ? getStarName() : getUnstarName()));
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

void CStage::setStarNode(int iIndex, Node* pNode)
{
    m_apStar[iIndex] = pNode;
}

Node* CStage::getStarNode(int iIndex)
{
    return m_apStar[iIndex];
}

void CStage::setStarNodesVisible(bool bVisible /*= true*/)
{
    m_apStar[0]->setVisible(bVisible);
    m_apStar[1]->setVisible(bVisible);
    m_apStar[2]->setVisible(bVisible);
}
void CStage::onInit()
{
}

void CStage::onChangeStatus(STAGE_STATUS eOldStatus)
{
    MenuItemImage* mi = DCAST(getNode(), MenuItemImage*);
    if (mi == nullptr)
    {
        return;
    }

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
: m_pPanel(nullptr)
{
}

CStageMap::~CStageMap()
{
}

CStage* CStageMap::getStage(int iIndex)
{
    return m_stages[iIndex].pStage;
}

void CStageMap::setPanel(Menu* pPanel, const ccMenuCallback& onClickStage)
{
    m_pPanel = pPanel;
    m_onClickStage = onClickStage;
}

void CStageMap::addStage(CStage* pStage, const VEC_INDEXES& vecPrev)
{
    pStage->setIndex(m_stages.size());
    
    //MenuItemImage* mi = MenuItemImage::create(pStage->getNormalName(), pStage->getSelectedName(), pStage->getDisabledName(), m_pSender, m_pHandler);
    Sprite* sel = Sprite::create(pStage->getNormalName());
    Sprite* sel2 = Sprite::create(pStage->getSelectedName());
    sel->addChild(sel2);
    sel2->setPosition(sel->getAnchorPointInPoints());
    ButtonNormal* btn = ButtonNormal::create(
        Sprite::create(pStage->getNormalName()),
        sel,
        Sprite::create(pStage->getDisabledName()),
        nullptr,
        nullptr,
        0.0f,
        m_onClickStage,
        nullptr);
    btn->setTag(pStage->getIndex());
    m_pPanel->addChild(btn);
    btn->setPosition(pStage->getPosition());
    pStage->setNode(btn);
    pStage->setStatus(pStage->getStatus());
    
    M_DEF_GC(gc);
    // set star
    for (int i = 0; i < 3; ++i)
    {
        Sprite* nd = Sprite::createWithSpriteFrameName(pStage->getUnstarName());
        pStage->setStarNode(i, nd);
        btn->addChild(nd);
        const Size& oStageSz = btn->getContentSize();
        const Size& oStarSz = nd->getContentSize();
        float bx = oStarSz.width + 0;
        float by = oStageSz.height * 0.5 + oStarSz.height * 0.5 + 0;
        nd->setPosition(btn->getAnchorPointInPoints() + Point(bx * (i - 1), -by));
    }
    pStage->setStarNodesVisible(false);

    STAGE_INFO stageInfo = {};
    stageInfo.pStage = pStage;
    m_stages.push_back(stageInfo);
    for (auto it = vecPrev.begin(); it != vecPrev.end(); ++it)
    {
        addStageNextInfos(*it, pStage->getIndex());
    }

    pStage->onInit();
}

void CStageMap::addStageNextInfos(int iIndex, int iNextIndex)
{
    // create path
    Sprite* pth = Sprite::createWithSpriteFrameName(getPathName());
    pth->setColor(Color3B(123, 123, 123));
    const Size& pthSz = pth->getContentSize();
    const Point& from = m_stages[iIndex].pStage->getPosition();
    const Point& to = m_stages[iNextIndex].pStage->getPosition();
    m_pPanel->getParent()->addChild(pth);
    Sprite* sp = Sprite::createWithSpriteFrameName(getPathName());
    sp->setColor(Color3B(248, 168, 1));
    ProgressBar* pth2 = ProgressBar::create(pth->getContentSize(), sp, nullptr, 0.0f, 0.0f, true);
    pth->addChild(pth2, 1, 2);
    pth2->setPosition(pth->getAnchorPointInPoints());
    pth2->setPercentage(0.0f);

    // scale path
    Point oDelta = to - from;
    float fR = CC_RADIANS_TO_DEGREES(-oDelta.getAngle());
    float fScale = (sqrt(oDelta.x * oDelta.x + oDelta.y * oDelta.y) - 200) / pthSz.width;
    pth->setPosition(Point((from.x + to.x) / 2, (from.y + to.y) / 2));
    pth->setRotation(fR);
    pth->setScaleX(fScale);

    STAGE_NEXT_INFO nextInfo = {};
    nextInfo.iIndex = iNextIndex;
    nextInfo.pPath = pth;
    m_stages[iIndex].vecNextInfos.push_back(nextInfo);
}

void CStageMap::setStageStatus(int iIndex, CStage::STAGE_STATUS eStatus)
{
    CStage* s = m_stages[iIndex].pStage;
    switch (eStatus)
    {
    case CStage::kLocked:
        break;

    case CStage::kUnlocked:
        break;

    case CStage::kConquered:
        if (s->getStatus() != CStage::kUnlocked)
        {
            setStageStatus(iIndex, CStage::kUnlocked);
        }

        for (auto it = m_stages[iIndex].vecNextInfos.begin(); it != m_stages[iIndex].vecNextInfos.end(); ++it)
        {
            setStageStatus(it->iIndex, CStage::kUnlocked);
            //DCAST(it->pPath, Sprite*)->setColor(Color3B(248, 168, 1));
            ProgressBar* pth2 = DCAST(it->pPath->getChildByTag(2), ProgressBar*);
            pth2->runActionForTimer(EaseExponentialOut::create(pth2->setPercentageAction(100.0f, 0.3f)));
        }
        break;
    }
    s->setStatus(eStatus);
}
